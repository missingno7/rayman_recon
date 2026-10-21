/* DOS 1:6e380..1:6e415. Names INVENTED, inherited from the canonical unit.
 * Both functions STRICT with the source-first copy contract; w9_copy/REPORT.md. */
#include "inline_helpers.h"

void effect_copy_band(u8 *source, u8 *destination, u32 width)
{
    u8 *end;
    u8 *row_source;
    u8 *row_destination;

    row_source = source;
    row_destination = destination;
    end = destination + 0xc800;
    do {
        copy(row_source, row_destination, width);
        row_source += 320;
        row_destination += 256;
    } while (row_destination != end);
}

void effect_copy_pause_band(u8 *source, u8 *destination, u32 width)
{
    u8 *end;
    u8 *row_source;
    u8 *row_destination;

    row_source = source;
    row_destination = destination;
    end = source + 0xc800;
    do {
        copy(row_source, row_destination, width);
        row_source += 256;
        row_destination += 320;
    } while (row_source != end);
}
