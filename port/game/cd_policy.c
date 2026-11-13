/*
 * Playing without a CD (port policy, user decision 2026-11-13). The CD is the port's source of the CD music and, when
 * the game folder has none, of the movies (platform/files.c); without one the port runs the full game without CD
 * music, as a modern release of the game would. The original instead stops at "Please insert Rayman's cdrom"
 * (Escape quits, Space starts a limited mode: JeuCracker, the first three worlds only on the world map), and its
 * save-time disc check sets JeuCracker when no disc answers.
 *
 * Only when no CD source exists at all (no image, no disc in a host drive: port_cd_present() is false) these two
 * cross-unit calls are answered here (-Wl,--wrap, CMakeLists.txt RAY_PORT_POLICY); with a CD every call runs the
 * original unchanged:
 *   - verify_cdrom_response (cdsave.c, called by PcMain): the original's own "play rayman without music" outcome
 *     (its no-MSCDEX branch: OptionMusicCdActive = 0), without the prompt, and JeuCracker untouched;
 *   - check_cd_for_save (cdsave.c, called by load.c and worldmap.c): the disc check does not run (it would find no
 *     disc and set JeuCracker).
 * Compiled with the historical units (their globals and types).
 */
#include "rtypes.h"

#include "platform/cdrom.h"

extern u8 OptionMusicCdActive;

u8 __real_verify_cdrom_response(void);
u8 __wrap_verify_cdrom_response(void)
{
    if (!port_cd_present()) {
        OptionMusicCdActive = 0;
        return 1;   /* PcMain: MusicCdActive = OptionMusicCdActive */
    }
    return __real_verify_cdrom_response();
}

void __real_check_cd_for_save(void);
void __wrap_check_cd_for_save(void)
{
    if (port_cd_present())
        __real_check_cd_for_save();
}
