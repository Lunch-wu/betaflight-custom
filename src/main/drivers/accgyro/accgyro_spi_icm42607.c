/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * ICM-42607-P driver, ported from INAV ICM-42670-P driver.
 * ICM-42607-P and ICM-42670-P share the same register map but
 * differ in WHO_AM_I (0x60 vs 0x67).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"

#if defined(USE_ACCGYRO_ICM42607)

#include "common/axis.h"
#include "common/utils.h"
#include "build/debug.h"

#include "drivers/accgyro/accgyro.h"
#include "drivers/accgyro/accgyro_mpu.h"
#include "drivers/accgyro/accgyro_spi_icm42607.h"
#include "drivers/bus_spi.h"
#include "drivers/exti.h"
#include "drivers/io.h"
#include "drivers/sensor.h"
#include "drivers/time.h"

#include "sensors/gyro.h"

#define ICM42607_MAX_SPI_CLK_HZ 24000000

/* ===== Bank 0 register addresses ===== */
#define ICM42607_RA_MCLK_RDY                        0x00
#define ICM42607_RA_SIGNAL_PATH_RESET                0x02
#define ICM42607_RA_INT_CONFIG                       0x06
#define ICM42607_RA_TEMP_DATA1                       0x09
#define ICM42607_RA_ACCEL_DATA_X1                    0x0B
#define ICM42607_RA_GYRO_DATA_X1                     0x11
#define ICM42607_RA_PWR_MGMT0                        0x1F
#define ICM42607_RA_GYRO_CONFIG0                     0x20
#define ICM42607_RA_ACCEL_CONFIG0                    0x21
#define ICM42607_RA_GYRO_CONFIG1                     0x23
#define ICM42607_RA_ACCEL_CONFIG1                    0x24
#define ICM42607_RA_INT_SOURCE0                      0x2B
#define ICM42607_RA_INTF_CONFIG0                     0x35
#define ICM42607_RA_INTF_CONFIG1                     0x36

/* MREG (Multi-bank Register) access registers */
#define ICM42607_RA_BLK_SEL_W                        0x79
#define ICM42607_RA_MADDR_W                          0x7A
#define ICM42607_RA_M_W                              0x7B

/* MREG1 register addresses */
#define ICM42607_MREG1_SENSOR_CONFIG3                0x06

/* SIGNAL_PATH_RESET bits */
#define ICM42607_SOFT_RESET_DEVICE_CONFIG            (1 << 4)

/* MCLK_RDY bit */
#define ICM42607_MCLK_RDY_BIT                        (1 << 3)

/* PWR_MGMT0 bits */
#define ICM42607_PWR_MGMT0_GYRO_MODE_LN             (3 << 2)
#define ICM42607_PWR_MGMT0_ACCEL_MODE_LN            (3 << 0)

/* INT_CONFIG bits */
#define ICM42607_INT1_MODE_PULSED                    (0 << 2)
#define ICM42607_INT1_DRIVE_CIRCUIT_PP               (1 << 1)
#define ICM42607_INT1_POLARITY_ACTIVE_HIGH           (1 << 0)

/* INT_SOURCE0 bits */
#define ICM42607_DRDY_INT1_EN                        (1 << 3)

/* GYRO_CONFIG0: bits [6:5] = FS_SEL, bits [3:0] = ODR */
#define ICM42607_GYRO_FS_2000DPS                     (0x00 << 5)

/* ACCEL_CONFIG0: bits [6:5] = FS_SEL, bits [3:0] = ODR */
#define ICM42607_ACCEL_FS_16G                        (0x00 << 5)

/* ODR values (bits [3:0] of CONFIG0 registers) */
#define ICM42607_ODR_1600HZ                          0x05
#define ICM42607_ODR_800HZ                           0x06
#define ICM42607_ODR_400HZ                           0x07
#define ICM42607_ODR_200HZ                           0x08

/* GYRO_CONFIG1 / ACCEL_CONFIG1 LPF bandwidth */
#define ICM42607_FILT_BW_BYPASSED                    0x00
#define ICM42607_FILT_BW_180HZ                       0x01
#define ICM42607_FILT_BW_121HZ                       0x02
#define ICM42607_FILT_BW_73HZ                        0x03
#define ICM42607_FILT_BW_53HZ                        0x04
#define ICM42607_FILT_BW_34HZ                        0x05
#define ICM42607_FILT_BW_25HZ                        0x06
#define ICM42607_FILT_BW_16HZ                        0x07

/* INTF_CONFIG0 bits */
#define ICM42607_SENSOR_DATA_ENDIAN_BIG              (1 << 4)
#define ICM42607_FIFO_COUNT_ENDIAN_BIG               (1 << 5)

/* INTF_CONFIG1 AFSR bits */
#define ICM42607_INTF_CONFIG1_AFSR_MASK              0xC0
#define ICM42607_INTF_CONFIG1_AFSR_DISABLE           0x40

static void icm42607WriteMREG(const extDevice_t *dev, uint8_t bank, uint8_t reg, uint8_t val)
{
    for (uint8_t i = 0; i < 10; i++) {
        uint8_t mclkRdy = spiReadRegMsk(dev, ICM42607_RA_MCLK_RDY);
        if ((mclkRdy & ICM42607_MCLK_RDY_BIT) != 0) {
            break;
        }
        delayMicroseconds(100);
    }

    spiWriteReg(dev, ICM42607_RA_BLK_SEL_W, bank);
    spiWriteReg(dev, ICM42607_RA_MADDR_W, reg);
    spiWriteReg(dev, ICM42607_RA_M_W, val);
    delayMicroseconds(10);
    spiWriteReg(dev, ICM42607_RA_BLK_SEL_W, 0x00);
}

uint8_t icm42607SpiDetect(const extDevice_t *dev)
{
    spiWriteReg(dev, ICM42607_RA_SIGNAL_PATH_RESET, ICM42607_SOFT_RESET_DEVICE_CONFIG);
    delay(2);

    spiWriteReg(dev, ICM42607_RA_PWR_MGMT0,
                ICM42607_PWR_MGMT0_GYRO_MODE_LN | ICM42607_PWR_MGMT0_ACCEL_MODE_LN);
    delay(1);

    for (uint8_t i = 0; i < 20; i++) {
        uint8_t mclk = spiReadRegMsk(dev, ICM42607_RA_MCLK_RDY);
        if (mclk & ICM42607_MCLK_RDY_BIT) {
            break;
        }
        delay(1);
    }

    uint8_t attemptsRemaining = 20;
    do {
        delay(1);
        const uint8_t whoAmI = spiReadRegMsk(dev, MPU_RA_WHO_AM_I);
        if (whoAmI == ICM42607_WHO_AM_I_CONST) {
            return ICM_42607_SPI;
        }
        if (!attemptsRemaining) {
            return MPU_NONE;
        }
    } while (attemptsRemaining--);

    return MPU_NONE;
}

void icm42607AccInit(accDev_t *acc)
{
    acc->acc_1G = 512 * 4;
}

bool icm42607SpiAccDetect(accDev_t *acc)
{
    if (acc->mpuDetectionResult.sensor != ICM_42607_SPI) {
        return false;
    }

    acc->initFn = icm42607AccInit;
    acc->readFn = mpuAccReadSPI;

    return true;
}

void icm42607GyroInit(gyroDev_t *gyro)
{
    const extDevice_t *dev = &gyro->dev;

    spiSetClkDivisor(dev, spiCalculateDivider(ICM42607_MAX_SPI_CLK_HZ));

    mpuGyroInit(gyro);
    gyro->accDataReg = ICM42607_RA_ACCEL_DATA_X1;
    gyro->gyroDataReg = ICM42607_RA_GYRO_DATA_X1;

    /* Software reset */
    spiWriteReg(dev, ICM42607_RA_SIGNAL_PATH_RESET, ICM42607_SOFT_RESET_DEVICE_CONFIG);
    delay(2);

    for (uint8_t tries = 0; tries < 50; tries++) {
        uint8_t whoami = spiReadRegMsk(dev, MPU_RA_WHO_AM_I);
        uint8_t sigPathReset = spiReadRegMsk(dev, ICM42607_RA_SIGNAL_PATH_RESET);
        if (whoami == ICM42607_WHO_AM_I_CONST &&
            (sigPathReset & ICM42607_SOFT_RESET_DEVICE_CONFIG) == 0) {
            break;
        }
        delay(10);
    }

    /* Wait for MCLK */
    for (uint8_t tries = 0; tries < 50; tries++) {
        uint8_t mclkRdy = spiReadRegMsk(dev, ICM42607_RA_MCLK_RDY);
        if ((mclkRdy & ICM42607_MCLK_RDY_BIT) != 0) {
            break;
        }
        delay(5);
    }

    /* Set big-endian data output */
    uint8_t intfConfig0 = spiReadRegMsk(dev, ICM42607_RA_INTF_CONFIG0);
    intfConfig0 |= ICM42607_FIFO_COUNT_ENDIAN_BIG | ICM42607_SENSOR_DATA_ENDIAN_BIG;
    spiWriteReg(dev, ICM42607_RA_INTF_CONFIG0, intfConfig0);
    delay(1);

    /* Power on gyro and accel in Low Noise mode */
    spiWriteReg(dev, ICM42607_RA_PWR_MGMT0,
                ICM42607_PWR_MGMT0_GYRO_MODE_LN | ICM42607_PWR_MGMT0_ACCEL_MODE_LN);
    delay(50);

    /* Disable APEX features via MREG1 SENSOR_CONFIG3 */
    icm42607WriteMREG(dev, 0x00, ICM42607_MREG1_SENSOR_CONFIG3, 0x40);

    /* ICM42607 max ODR is 1.6 kHz */
    const uint8_t odrConfig = ICM42607_ODR_1600HZ;
    gyro->gyroRateKHz = GYRO_RATE_1600_Hz;
    gyro->gyroSampleRateHz = 1600;

    /* Configure gyro: ±2000 dps */
    spiWriteReg(dev, ICM42607_RA_GYRO_CONFIG0, ICM42607_GYRO_FS_2000DPS | (odrConfig & 0x0F));
    delay(15);

    /* Configure accel: ±16g, same ODR */
    spiWriteReg(dev, ICM42607_RA_ACCEL_CONFIG0, ICM42607_ACCEL_FS_16G | (odrConfig & 0x0F));
    delay(15);

    /* Configure gyro LPF */
    uint8_t gyroConfig1 = spiReadRegMsk(dev, ICM42607_RA_GYRO_CONFIG1);
    gyroConfig1 = (gyroConfig1 & 0xF8) | ICM42607_FILT_BW_53HZ;
    spiWriteReg(dev, ICM42607_RA_GYRO_CONFIG1, gyroConfig1);
    delay(15);

    /* Configure accel LPF */
    uint8_t accelConfig1 = spiReadRegMsk(dev, ICM42607_RA_ACCEL_CONFIG1);
    accelConfig1 = (accelConfig1 & 0xF8) | ICM42607_FILT_BW_53HZ;
    spiWriteReg(dev, ICM42607_RA_ACCEL_CONFIG1, accelConfig1);
    delay(15);

    /* Configure INT1: pulsed, push-pull, active high */
    spiWriteReg(dev, ICM42607_RA_INT_CONFIG,
                ICM42607_INT1_MODE_PULSED | ICM42607_INT1_DRIVE_CIRCUIT_PP | ICM42607_INT1_POLARITY_ACTIVE_HIGH);
    delay(15);

    /* Enable data ready interrupt on INT1 */
    spiWriteReg(dev, ICM42607_RA_INT_SOURCE0, ICM42607_DRDY_INT1_EN);
    delay(100);

    /* Disable AFSR */
    uint8_t intfConfig1 = spiReadRegMsk(dev, ICM42607_RA_INTF_CONFIG1);
    intfConfig1 &= ~ICM42607_INTF_CONFIG1_AFSR_MASK;
    intfConfig1 |= ICM42607_INTF_CONFIG1_AFSR_DISABLE;
    spiWriteReg(dev, ICM42607_RA_INTF_CONFIG1, intfConfig1);
    delay(15);
}

bool icm42607SpiGyroDetect(gyroDev_t *gyro)
{
    if (gyro->mpuDetectionResult.sensor != ICM_42607_SPI) {
        return false;
    }

    gyro->initFn = icm42607GyroInit;
    gyro->readFn = mpuGyroReadSPI;
    gyro->scale = GYRO_SCALE_2000DPS;

    return true;
}

#endif // USE_ACCGYRO_ICM42607
