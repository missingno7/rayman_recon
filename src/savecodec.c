/* DOS save stream codec, object 1:21840..1:21f00.
   Public names without Android symbols are worker-invented. */
#include <stdio.h>
#include <stdarg.h>

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;

enum {
    SAVE_BLOCK_BYTES = 8,
    SAVE_MASK_BITS = 8,
    SAVE_MASK_GROUPS = SAVE_BLOCK_BYTES / SAVE_MASK_BITS
};


/* DOS CONST 2:30bac, _DATA 2:06318. The method literal precedes
 * the code-referenced formats in this object. Both segments VERIFIED;
 * all 14 existing functions stay STRICT. See worker NOTES.md. */
extern u8 sav_error_status;
extern char *sav_codec_method;
extern u8 sav_codec_error;
extern u8 sav_codec_key;
extern s32 sav_encoded_count;
extern s32 sav_raw_count;
u8 sav_error_status = 0;
char *sav_codec_method = "Differential method";
u8 sav_codec_error = 0;
u8 sav_codec_key = 0x53;
s32 sav_encoded_count = 0;
s32 sav_raw_count = 0;

extern u8 sav_big_window[256][8];
extern u8 sav_window[8];
extern u8 sav_checksum;

u8 sub_21840(void);
u8 reverse_save_byte(u8 value);
u8 read_save_header_byte(FILE *stream);
void write_save_header_byte(u8 value, FILE *stream);
u32 read_save_header_size(FILE *stream);
void write_save_header_size(u32 size, FILE *stream);
s32 read_save_raw_byte(FILE *stream);
s32 write_save_encoded_byte(s32 value, FILE *stream);
s32 read_save_encoded_byte(FILE *stream);
s32 write_save_raw_byte(s32 value, FILE *stream);
void reset_save_codec(void);
void sub_21b48(FILE *source, FILE *destination);
void sub_21d28(FILE *source, FILE *destination);
void report_save_codec_error(const char *message, ...);

u8 sub_21840(void)
{
    return sav_error_status;
}

u8 reverse_save_byte(u8 value)
{
    u8 bit;
    u8 reversed;

    reversed = 0;
    for (bit = 0; bit < 8; bit++) {
        reversed |= ((value >> bit) & 1) << (7 - bit);
    }
    return reversed;
}

u8 read_save_header_byte(FILE *stream)
{
    u8 value;

    fread(&value, 1, 1, stream);
    value ^= 0x53;
    return value;
}

void write_save_header_byte(u8 value, FILE *stream)
{
    fseek(stream, 0, SEEK_SET);
    value ^= 0x53;
    fwrite(&value, 1, 1, stream);
}

u32 read_save_header_size(FILE *stream)
{
    u32 size;

    fread(&size, 4, 1, stream);
    size ^= 0x54555657UL;
    return size;
}

void write_save_header_size(u32 size, FILE *stream)
{
    size ^= 0x54555657UL;
    fwrite(&size, 4, 1, stream);
}

s32 read_save_raw_byte(FILE *stream)
{
    s32 value;

    value = getc(stream);
    if (value != -1)
        sav_raw_count++;
    return value;
}

s32 write_save_encoded_byte(s32 value, FILE *stream)
{
    s32 encoded;

    encoded = value ^ (u8)(reverse_save_byte(sav_codec_key++) ^ 0xB9);
    sav_checksum += (u8)value;
    sav_encoded_count++;
    return putc(encoded, stream);
}

s32 read_save_encoded_byte(FILE *stream)
{
    s32 value;
    u8 key;

    value = getc(stream);
    if (value != -1) {
        key = sav_codec_key++;
        value ^= reverse_save_byte(key) ^ 0xB9;
        sav_checksum -= (u8)value;
        sav_encoded_count++;
    }
    return value;
}

s32 write_save_raw_byte(s32 value, FILE *stream)
{
    sav_raw_count--;
    if (sav_raw_count < 0)
        sav_codec_error = 1;
    return putc(value, stream);
}

void reset_save_codec(void)
{
    s32 row;
    s32 col;

    for (row = 0; row < 256; row++) {
        for (col = 0; col < 8; col++)
            sav_big_window[row][col] = (u8)row;
    }
    sav_raw_count = 0;
    sav_encoded_count = 0;
    sav_codec_error = 0;
    sav_codec_key = 0x57;
}

void sub_21b48(FILE *source, FILE *destination)
{
    s32 bytes_to_compress;
    s32 j;
    s32 occurrences;
    s32 max_occurrences;
    s32 best_window;
    s32 update_bits;
    s32 bit;
    s32 best_offset;
    s32 i;

    sav_error_status = 0;
    reset_save_codec();
    sav_checksum = 0;
    write_save_header_byte(0, destination);
    sav_raw_count = 0;
    write_save_header_size(0, destination);
    while (!sav_codec_error) {
        bytes_to_compress = 8;
        for (j = 0; j < 8; j++) {
            i = read_save_raw_byte(source);
            if (i == -1) {
                bytes_to_compress = j;
                break;
            }
            sav_window[j] = (u8)i;
        }
        if (bytes_to_compress == 0) {
            sav_codec_error = 1;
        } else {
            max_occurrences = -1;
            for (i = 0; i < 256; i++) {
                occurrences = 0;
                for (j = 0; j < bytes_to_compress; j++) {
                    if (sav_big_window[i][j] == sav_window[j])
                        occurrences++;
                }
                if (occurrences > max_occurrences) {
                    max_occurrences = occurrences;
                    best_window = i;
                    if (occurrences == bytes_to_compress)
                        break;
                }
            }
            write_save_encoded_byte(best_window, destination);
            best_offset = best_window * 8;
            /* Each mask describes one eight-byte group of the block.
               DOS 1:21c39..21cc7 retains these nested group/byte loops. */
            for (i = 0; i < SAVE_MASK_GROUPS; i++) {
                update_bits = 0;
                bit = 1;
                for (j = i * SAVE_MASK_BITS; j < (i + 1) * SAVE_MASK_BITS; j++) {
                    if (j >= bytes_to_compress)
                        break;
                    if (((u8 *)sav_big_window)[best_offset + j] != sav_window[j])
                        update_bits += bit;
                    bit *= 2;
                }
                write_save_encoded_byte(update_bits, destination);
                for (j = i * SAVE_MASK_BITS; j < (i + 1) * SAVE_MASK_BITS; j++) {
                    if (update_bits % 2 == 1)
                        write_save_encoded_byte(sav_window[j], destination);
                    update_bits /= 2;
                }
            }
            if (bytes_to_compress < 8) {
                write_save_encoded_byte(bytes_to_compress, destination);
                sav_codec_error = 1;
            } else {
                for (j = 0; j < 8; j++)
                    ((u8 *)sav_big_window)[best_offset + j] = sav_window[j];
            }
        }
    }
    write_save_header_byte(sav_checksum, destination);
    write_save_header_size((u32)sav_raw_count, destination);
}

void sub_21d28(FILE *source, FILE *destination)
{
    s32 big_window_index;
    s32 update_bits;
    s32 i;
    s32 j;

    sav_error_status = 0;
    reset_save_codec();
    big_window_index = -1;
    sav_checksum = read_save_header_byte(source);
    sav_raw_count = (s32)read_save_header_size(source);
    while (!sav_codec_error) {
        if (big_window_index == -1) {
            big_window_index = read_save_encoded_byte(source);
            if (big_window_index == -1)
                sav_codec_error = 1;
            else
                update_bits = read_save_encoded_byte(source);
        } else {
            big_window_index = read_save_encoded_byte(source);
            if (big_window_index == -1) {
                for (i = 0; i < 8; i++)
                    write_save_raw_byte(sav_window[i], destination);
                sav_codec_error = 1;
            } else {
                update_bits = read_save_encoded_byte(source);
                if (update_bits == -1) {
                    for (i = 0; i < big_window_index; i++)
                        write_save_raw_byte(sav_window[i], destination);
                    sav_codec_error = 1;
                } else {
                    for (i = 0; i < 8; i++)
                        write_save_raw_byte(sav_window[i], destination);
                }
            }
        }
        if (!sav_codec_error) {
            /* Same mask grouping as the encoder (DOS 1:21e29..21e7f). */
            for (j = 0; j < SAVE_MASK_GROUPS; j++) {
                for (i = j * SAVE_MASK_BITS; i < (j + 1) * SAVE_MASK_BITS; i++) {
                    if (update_bits % 2 == 1)
                        sav_big_window[big_window_index][i] = (u8)read_save_encoded_byte(source);
                    sav_window[i] = sav_big_window[big_window_index][i];
                    update_bits /= 2;
                }
            }
        }
    }
    if (sav_raw_count == 0) {
        if (sav_checksum != 0)
            report_save_codec_error("Source file checksum error\n");
    } else
        report_save_codec_error("Corrupted source file\n");
}

void report_save_codec_error(const char *message, ...)
{
    va_list args;

    va_start(args, message);
    printf("Fatal error: ");
    vprintf(message, args);
    va_end(args);
    sav_error_status = 1;
}
