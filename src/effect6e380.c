/* DOS 1:6e380..1:6e420; names INVENTED. F3 copy idiom remains OPEN. */
#include <string.h>
#include "rtypes.h"

void effect_copy_band(u8 *source, u8 *destination, u32 width)
{
    u8 *row_source;
    u8 *row_destination;
    u8 *end;
    u32 row_width;

    row_source = source;
    row_destination = destination;
    row_width = width;
    end = destination + 0xc800;
    do {
        memcpy(row_destination, row_source, row_width);
        row_source += 320;
        row_destination += 256;
    } while (row_destination != end);
}

void effect_copy_pause_band(u8 *source, u8 *destination, u32 width)
{
    u32 row_width;
    u8 *row_source;
    u8 *row_destination;
    u8 *end;

    end = source + 0xc800;
    row_source = source;
    row_width = width;
    row_destination = destination;
    do {
        memcpy(row_destination, row_source, row_width);
        row_source += 256;
        row_destination += 320;
    } while (row_source != end);
}
