![Betaflight](https://raw.githubusercontent.com/betaflight/.github/main/profile/images/bf_logo.svg#gh-light-mode-only)
![Betaflight](https://raw.githubusercontent.com/betaflight/.github/main/profile/images/bf_logo_dark.svg#gh-dark-mode-only)

[![Latest version](https://img.shields.io/github/v/release/betaflight/betaflight)](https://github.com/betaflight/betaflight/releases) [![Build](https://img.shields.io/github/actions/workflow/status/betaflight/betaflight/push.yml?branch=master)](https://github.com/betaflight/betaflight/actions/workflows/push.yml) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Join us on Discord!](https://img.shields.io/discord/868013470023548938)](https://discord.gg/n4E6ak4u3c)

Betaflight is flight controller software (firmware) used to fly multi-rotor craft and fixed wing craft. Betaflight focuses on flight performance, leading-edge feature additions, and wide target support.


## Release Schedule

| Date  | Release | Stage | Status |
| - | - | - | - |
| 01-10-2025 | 2025.12 | Beta | Completed |
| 01-10-2025 | 2025.12 | Release Candidate | Underway |
| 01-12-2025 | 2025.12 | Release | Pending |
| 01-04-2026 | 2026.6 | Beta | |
| 01-05-2026 | 2026.6 | Release Candidate | |
| 01-06-2026 | 2026.6 | Release | |
| 01-10-2026 | 2026.12 | Beta | |
| 01-11-2026 | 2026.12 | Release Candidate | |
| 01-12-2026 | 2026.12 | Release | |


## News

### 📣 Announcement: New Versioning Scheme & Release Cadence 📣

To create a more predictable release schedule, we're moving to a new versioning system and development cycle, starting with the next release.

**New Format**: `YYYY.M.PATCH` (e.g., `2025.12.1`)

**Release Cadence**: Two major releases per year.

**Target Months**: June and December.

This means the successor to our current `4.x` series will be Betaflight `2025.12.x`, followed by Betaflight `2026.6.x`. We will also align the Betaflight App and Firmware to the same `YYYY.M.PATCH` releases (and cadence).

**Our New Release Cycle**

To support this schedule, our development phases will be structured as follows:

**Alpha**: For new feature development. Alpha builds for the next version will be available shortly after a stable release is published.

**Beta**: A one-month feature freeze for bug fixes only, and existing pull requests currently being reviewed, starting approximately two months before a release.

**Release Candidate (RC)**: A one-month period for final stabilization and testing before the official release.

⚠️ **Important Note for the `2025.12` Release** ⚠️

For the `2025.12` release, due to the timing since the last release, we are extending the RC period to two months. The Release Candidate phase will begin in October 2025 and until the end of November 2025.

### Requirements for the submission of new and updated targets

The following new requirements for pull requests adding new targets or modifying existing targets are put in place from now on:

1. Read the [hardware specification](https://www.betaflight.com/docs/development/manufacturer/manufacturer-design-guidelines)

2. No new F3 based targets will be accepted;

3. For any new target that is to be added, only a Unified Target config into https://github.com/betaflight/unified-targets/tree/master/configs/default needs to be submitted. See the [instructions](https://www.betaflight.com/docs/manufacturer/creating-an-unified-target) for how to create a Unified Target configuration. If there is no Unified Target for the MCU type of the new target (see instructions above), then a 'legacy' format target definition into `src/main/target/` has to be submitted as well;

4. For changes to existing targets, the change needs to be applied to the Unified Target config in https://github.com/betaflight/unified-targets/tree/master/configs/default. If no Unified Target configuration for the target exists, a new Unified Target configuration will have to be created and submitted. If there is no Unified Target for the MCU type of the new target (see instructions above), then an update to the 'legacy' format target definition in `src/main/target/` has to be submitted alongside the update to the Unified Target configuration.


## Features

Betaflight has the following features:

* Multi-color RGB LED strip support (each LED can be a different color using variable length WS2811 Addressable RGB strips - use for Orientation Indicators, Low Battery Warning, Flight Mode Status, Initialization Troubleshooting, etc)
* DShot (150, 300 and 600), Multishot, Oneshot (125 and 42) and Proshot1000 motor protocol support
* Blackbox flight recorder logging (to onboard flash or external microSD card where equipped)
* Support for targets that use the STM32 F4, G4, F7 and H7 processors
* PWM, PPM, SPI, and Serial (SBus, SumH, SumD, Spektrum 1024/2048, XBus, etc) RX connection with failsafe detection
* Multiple telemetry protocols (CRSF, FrSky, HoTT smart-port, MSP, etc)
* RSSI via ADC - Uses ADC to read PWM RSSI signals, tested with FrSky D4R-II, X8R, X4R-SB, & XSR
* OSD support & configuration without needing third-party OSD software/firmware/comm devices
* OLED Displays - Display information on: Battery voltage/current/mAh, profile, rate profile, mode, version, sensors, etc
* In-flight manual PID tuning and rate adjustment
* PID and filter tuning using sliders
* Rate profiles and in-flight selection of them
* Configurable serial ports for Serial RX, Telemetry, ESC telemetry, MSP, GPS, OSD, Sonar, etc - Use most devices on any port, softserial included
* VTX support for Unify Pro and IRC Tramp
* and MUCH, MUCH more.


## Installation & Documentation

See: https://betaflight.com/docs/wiki


## Support and Developers Channel

There's a dedicated [Discord server](https://discord.gg/n4E6ak4u3c) for help, support and general community.


## Betaflight Application

To configure Betaflight you should use the [Betaflight App](https://app.betaflight.com). It is a progressive web app, so should always be the latest version.


## Contributing

Contributions are welcome and encouraged. You can contribute in many ways:

* implement a new feature in the firmware or in the app (see [below](#Developers));
* documentation updates and corrections;
* How-To guides - received help? Help others!
* bug reporting & fixes;
* new feature ideas & suggestions;
* provide a new translation for the app, or help us maintain the existing ones (see [below](#Translators)).

The best place to start is the Betaflight Discord (registration [here](https://discord.gg/n4E6ak4u3c)). Next place is the github issue tracker:

https://github.com/betaflight/betaflight/issues
https://github.com/betaflight/betaflight-configurator/issues

Before creating new issues please check to see if there is an existing one, search first otherwise you waste people's time when they could be coding instead!

If you want to contribute to our efforts financially, please consider making a donation to us through [PayPal](https://paypal.me/betaflight).

If you want to contribute financially on an ongoing basis, you should consider becoming a patron for us on [Patreon](https://www.patreon.com/betaflight).


## Developers

Contribution of bugfixes and new features is encouraged. Please be aware that we have a thorough review process for pull requests, and be prepared to explain what you want to achieve with your pull request.
Before starting to write code, please read our [development guidelines](https://www.betaflight.com/docs/development) and [coding style definition](https://www.betaflight.com/docs/development/CodingStyle).

GitHub actions are used to run automatic builds


## Translators

We want to make Betaflight accessible for pilots who are not fluent in English, and for this reason we are currently maintaining translations into 21 languages for Betaflight Configurator: Català, Dansk, Deutsch, Español, Euskera, Français, Galego, Hrvatski, Bahasa Indonesia, Italiano, 日本語, 한국어, Latviešu, Português, Português Brasileiro, polski, Русский язык, Svenska, 简体中文, 繁體中文.
We have got a team of volunteer translators who do this work, but additional translators are always welcome to share the workload, and we are keen to add additional languages. If you would like to help us with translations, you have got the following options:
- if you help by suggesting some updates or improvements to translations in a language you are familiar with, head to [crowdin](https://crowdin.com/project/betaflight-configurator) and add your suggested translations there;
- if you would like to start working on the translation for a new language, or take on responsibility for proof-reading the translation for a language you are very familiar with, please head to the Betaflight Discord chat (registration [here](https://discord.gg/n4E6ak4u3c)), and join the ['translation'](https://discord.com/channels/868013470023548938/1057773726915100702) channel - the people in there can help you to get a new language added, or set you up as a proof reader.


## Hardware Issues

Betaflight does not manufacture or distribute their own hardware. While we are collaborating with and supported by a number of manufacturers, we do not do any kind of hardware support.

If you encounter any hardware issues with your flight controller or another component, please contact the manufacturer or supplier of your hardware, or check [Discord](https://discord.gg/n4E6ak4u3c) to see if others with the same problem have found a solution.


## Betaflight Releases

You can find our release [here](https://github.com/betaflight/betaflight/releases) on Github and we also have more detailed [release notes](https://www.betaflight.com/docs/category/release-notes) at [betaflight.com](https://www.betaflight.com).


## Open Source / Contributors

Betaflight is software that is **open source** and is available free of charge without warranty to all users.

For a complete list of contributors (past and present) see [Github](https://github.com/betaflight/betaflight/graphs/contributors).


---

## 本地编译指南 (Local Build Guide)

本文档记录了在本地环境中编译 Betaflight 固件的完整步骤和环境配置要求。

### 环境要求

| 组件 | 版本要求 | 说明 |
| --- | --- | --- |
| **操作系统** | Linux (推荐 Ubuntu/Debian/WSL2) | Windows 用户推荐使用 WSL2 |
| **ARM GCC 工具链** | 13.3.1 | arm-none-eabi-gcc |
| **Git** | 2.x+ | 版本控制 |
| **Make** | 4.x+ | 构建工具 |
| **Python** | 3.x | 部分脚本依赖 |

### 快速开始

#### 1. 克隆仓库

```bash
git clone https://github.com/betaflight/betaflight.git
cd betaflight
```

#### 2. 切换到目标版本

```bash
# 查看可用版本
git fetch origin --tags
git tag --sort=-v:refname | head -10

# 切换到最新稳定版本
git checkout 2025.12.1
```

#### 3. 安装 ARM 工具链

Betaflight 提供了自动安装脚本，会将工具链安装到 `tools/` 目录：

```bash
make arm_sdk_install
```

> **注意**: 如果在中国大陆网络环境下载缓慢，可设置代理：
> ```bash
> export http_proxy="http://127.0.0.1:10808"
> export https_proxy="http://127.0.0.1:10808"
> make arm_sdk_install
> ```

#### 4. 初始化配置

```bash
make configs
```

此命令会拉取 `src/config` 子模块，包含所有目标板的配置文件。

#### 5. 验证工具链

```bash
make arm_sdk_version
```

预期输出：
```
arm-none-eabi-gcc (Arm GNU Toolchain 13.3.Rel1) 13.3.1 20240614
```

### 编译固件

#### 查看可用目标

```bash
make targets
```

主要输出信息：
- **Platforms**: 支持的平台 (APM32, AT32, PICO, SIMULATOR, STM32)
- **Valid targets**: 所有有效的目标板
- **Built targets**: CI 编译的目标板

#### 编译指定目标

使用 `CONFIG` 参数指定目标板配置：

```bash
# 编译 MATEKF405TE (基于 STM32F405)
make CONFIG=MATEKF405TE

# 编译其他 F405 目标板
make CONFIG=MATEKF405SE
make CONFIG=BETAFPVF405
```

#### 常用 STM32F405 目标板

| 目标板 | 制造商 | 说明 |
| --- | --- | --- |
| MATEKF405TE | Matek | 支持 SD 卡和 GPS |
| MATEKF405SE | Matek | 标准版 |
| BETAFPVF405 | BetaFPV | 微型飞控 |
| SPEEDYBEEF405V4 | SpeedyBee | 集成 4in1 ESC |
| FOXEERF405V2 | Foxeer | 竞速飞控 |

#### 编译输出

编译成功后，固件文件位于 `obj/` 目录：

```
obj/betaflight_2025.12.1_STM32F405_MATEKF405TE.hex
```

### 编译选项

| 命令 | 说明 |
| --- | --- |
| `make CONFIG=<target>` | 编译指定目标的 HEX 文件 |
| `make CONFIG=<target> binary` | 编译 BIN 文件 |
| `make CONFIG=<target> hex` | 编译 HEX 文件 |
| `make clean` | 清理编译产物 |
| `make help` | 查看帮助信息 |

### 常见问题

#### Q: 提示 arm-none-eabi-gcc 版本不匹配

运行 `make arm_sdk_install` 安装正确版本的工具链。

#### Q: 提示 src/config 不存在

运行 `make configs` 初始化配置子模块。

#### Q: 编译时内存不足

尝试限制并行编译数量：
```bash
make CONFIG=<target> -j4
```

### 编译验证记录

- **日期**: 2026-01-24
- **版本**: 2025.12.1
- **目标板**: MATEKF405TE (STM32F405)
- **工具链**: arm-none-eabi-gcc 13.3.1
- **状态**: ✅ 编译成功
- **固件大小**: FLASH1 使用率 49.91%, RAM 使用率 65.99%
