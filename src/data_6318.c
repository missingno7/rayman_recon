/* GOG v1.21 _DATA 2:06318. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "rtypes.h"

extern char dt_str_differential_method[];

u8 sav_error_status = 0;
char *sav_codec_method = dt_str_differential_method;
u8 sav_codec_error = 0;
u8 sav_codec_key = 0x53; /* DOS initial value, reset to 0x57 at 1:21b36. */
s32 sav_encoded_count = 0;
s32 sav_raw_count = 0;
