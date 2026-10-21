/* DOS 1:6e380..1:6e415; names INVENTED, DOS_BINARY_DERIVED.
 * ORIGINAL_INLINE copy sites: 1:6e39e and 1:6e3e9.
 * Uses the shared helper unchanged. Both functions remain NEAR solely
 * because its argument setup reverses the DOS EDI/ESI move order. */
#include "inline_helpers.h"
#include "rtypes.h"

void effect_copy_band(u8 *source, u8 *destination, u32 width)
{
    u8 *end;
    u8 *row_source;
    u8 *row_destination;

    row_source = source;
    row_destination = destination;
    end = destination + 0xc800;
    do {
        copy(row_destination, row_source, width);
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
        copy(row_destination, row_source, width);
        row_source += 256;
        row_destination += 320;
    } while (row_source != end);
}
