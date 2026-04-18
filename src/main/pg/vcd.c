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
 */

#include "platform.h"

#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "drivers/osd.h"

#include "vcd.h"

// no template required since defaults are zero
PG_REGISTER_WITH_RESET_FN(vcdProfile_t, vcdProfile, PG_VCD_CONFIG, 0);

void pgResetFn_vcdProfile(vcdProfile_t *vcdProfile)
{
    // Allow a target to override the default video system when its onboard
    // hardware needs a non-standard startup mode (e.g. AIO boards with both
    // analog MAX7456 and HD support compiled in, which must default to AUTO
    // so init.c doesn't force-switch to MSP and blank the analog OSD).
#if defined(DEFAULT_VIDEO_SYSTEM)
    vcdProfile->video_system = DEFAULT_VIDEO_SYSTEM;
#elif defined(USE_OSD_HD)
    vcdProfile->video_system = VIDEO_SYSTEM_HD;
#else
    vcdProfile->video_system = VIDEO_SYSTEM_AUTO;
#endif
}
