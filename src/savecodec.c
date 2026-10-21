/* DOS save stream codec, object 1:21840..1:21f00.
   Public names without Android symbols are worker-invented. */
#include <stdio.h>

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;

extern u8 sav_error_status;
extern u8 sav_codec_error;
extern u8 sav_codec_key;
extern s32 sav_encoded_count;
extern s32 sav_raw_count;
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
void write_save_encoded_byte(s32 value, FILE *stream);
s32 read_save_encoded_byte(FILE *stream);
void write_save_raw_byte(s32 value, FILE *stream);
void reset_save_codec(void);
void sub_21b48(FILE *source, FILE *destination);
void sub_21d28(FILE *source, FILE *destination);
void __cdecl report_save_codec_error(const char *message);

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
        reversed |= (u8)(((value >> bit) & 1) << (7 - bit));
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

void write_save_encoded_byte(s32 value, FILE *stream)
{
    value ^= reverse_save_byte(sav_codec_key++) ^ 0xB9;
    sav_checksum += (u8)value;
    sav_encoded_count++;
    putc(value, stream);
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

void write_save_raw_byte(s32 value, FILE *stream)
{
    sav_raw_count--;
    if (sav_raw_count < 0)
        sav_codec_error = 1;
    putc(value, stream);
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
    s32 i;
    s32 j;
    s32 occurrences;
    s32 max_occurrences;
    s32 best_window;
    s32 update_bits;
    s32 finished;

    sav_error_status = 0;
    reset_save_codec();
    sav_checksum = 0;
    write_save_header_byte(0, destination);
    write_save_header_size(0, destination);
    finished = sav_codec_error != 0;
    while (!finished) {
        bytes_to_compress = 0;
        for (i = 0; i < 8; i++) {
            s32 value;
            value = read_save_raw_byte(source);
            if (value == -1)
                break;
            bytes_to_compress++;
            sav_window[i] = (u8)value;
        }
        if (bytes_to_compress > 0) {
            max_occurrences = -1;
            best_window = 0;
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
            write_save_encoded_byte((u8)best_window, destination);
            update_bits = 0;
            for (i = 0; i < bytes_to_compress; i++) {
                if (sav_big_window[best_window][i] != sav_window[i])
                    update_bits |= 1 << i;
            }
            write_save_encoded_byte((u8)update_bits, destination);
            for (i = 0; i < 8; i++) {
                if (update_bits & 1)
                    write_save_encoded_byte(sav_window[i], destination);
                update_bits >>= 1;
            }
            if (bytes_to_compress >= 8) {
                for (i = 0; i < 8; i++)
                    sav_big_window[best_window][i] = sav_window[i];
            } else {
                write_save_encoded_byte((u8)bytes_to_compress, destination);
                finished = 1;
            }
        } else {
            finished = 1;
        }
        if (sav_codec_error)
            finished = 1;
    }
    write_save_header_byte(sav_checksum, destination);
    write_save_header_size((u32)sav_raw_count, destination);
}

void sub_21d28(FILE *source, FILE *destination)
{
    s32 big_window_index;
    s32 update_bits;
    s32 i;
    s32 value;

    sav_error_status = 0;
    reset_save_codec();
    sav_checksum = read_save_header_byte(source);
    sav_raw_count = (s32)read_save_header_size(source);
    while (!sav_codec_error) {
        if (destination == (FILE *)-1) {
            big_window_index = read_save_encoded_byte(source);
            if (big_window_index == -1) {
                sav_codec_error = 1;
                break;
            }
            update_bits = read_save_encoded_byte(source);
            if (update_bits == -1) {
                sav_codec_error = 1;
                break;
            }
        } else {
            big_window_index = read_save_encoded_byte(source);
            if (big_window_index == -1) {
                for (i = 0; i < 8; i++)
                    write_save_raw_byte(sav_window[i], destination);
                sav_codec_error = 1;
                break;
            }
            update_bits = read_save_encoded_byte(source);
            if (update_bits == -1) {
                for (i = 0; i < big_window_index; i++)
                    write_save_raw_byte(sav_window[i], destination);
                sav_codec_error = 1;
                break;
            } else {
                for (i = 0; i < 8; i++)
                    write_save_raw_byte(sav_window[i], destination);
            }
        }
        for (i = 0; i < 8; i++) {
            if (update_bits & 1) {
                value = read_save_encoded_byte(source);
                sav_big_window[big_window_index][i] = (u8)value;
            }
            sav_window[i] = sav_big_window[big_window_index][i];
            update_bits >>= 1;
        }
    }
    if (sav_raw_count != 0)
        report_save_codec_error("Corrupted source file\n");
    else if (sav_checksum != 0)
        report_save_codec_error("Source file checksum error\n");
}

void __cdecl report_save_codec_error(const char *message)
{
    printf("Fatal error: ");
    fprintf(stderr, message);
    sav_error_status = 1;
}
