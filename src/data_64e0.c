/* GOG v1.21 _DATA 2:064e0. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "rtypes.h"


s32 movie_audio_limits[5][4] = {
    {3840, 1, 215, 223},
    {7936, 223, 412, 468},
    {65832, 468, 890, 920},
    {70656, 920, 1170, 1195},
    {16777215, 1195, 1800, 1805},
};
u8 movie_saved_mode = 60;
u8 movie_palette_flag = 0;
u8 movie_playback_state = 0;
s32 movie_palette_xmin = 0;
s32 movie_palette_xmax = 256;
u8 *movie_draw_buffer = 0;
