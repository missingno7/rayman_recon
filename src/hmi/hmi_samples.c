/* INVENTED names. DOS-derived HMI sample voice operations, 1:743e2..74e4a. */
#include "hmi_local.h"

void hmi_samples_begin(void) {}

int sub_743F7(unsigned driver, hmi_sample_template __far *sample)
{
    unsigned voice;
    unsigned char __far *driver_address;
    unsigned char __far *sample_address;
    unsigned long sample_first_word;

    driver_address = hmi_driver_entrypoints[driver].pointer;
    if (hmi_driver_ids[driver] >= 0xe106) {
        sample_address = sample->pointer_00.pointer;
        sample_first_word = sample->dword_08;
    } else {
        for (voice = 0; voice < 32; voice++) {
            if (!(hmi_voice_slots[driver][voice]->flags.raw & 0x8000)) {
                hmi_voice_slots[driver][voice]->pointer_00.pointer = sample->pointer_00.pointer;
                hmi_voice_slots[driver][voice]->pointer_08.pointer = sample->pointer_00.pointer;
                hmi_voice_slots[driver][voice]->pointer_10.pointer = sample->pointer_00.pointer + sample->dword_30;

                if (sample->flags_28.bytes.low & 0x40) {
                    hmi_voice_slots[driver][voice]->dword_18 = sample->dword_30;
                    hmi_voice_slots[driver][voice]->dword_1c = sample->dword_30;
                    hmi_voice_slots[driver][voice]->dword_20 = sample->dword_34;
                    hmi_voice_slots[driver][voice]->dword_2c =
                        sample->dword_2c - (sample->dword_30 + sample->dword_34);
                } else {
                    hmi_voice_slots[driver][voice]->dword_18 = sample->dword_08;
                    hmi_voice_slots[driver][voice]->dword_1c = sample->dword_08;
                }

                hmi_voice_slots[driver][voice]->word_32 = sample->word_14;
                hmi_voice_slots[driver][voice]->word_34 = sample->word_18;
                hmi_voice_slots[driver][voice]->flags.raw = sample->flags_28.raw | 0xa000;
                hmi_voice_slots[driver][voice]->word_36 = sample->word_10;
                hmi_voice_slots[driver][voice]->pointer_3c.pointer = sample->pointer_1c.pointer;
                hmi_voice_slots[driver][voice]->word_38 = sample->word_0c;
                hmi_voice_slots[driver][voice]->word_4a = sample->word_24;
                hmi_voice_slots[driver][voice]->word_3a = 0;
                hmi_voice_slots[driver][voice]->dword_44 = sample->dword_38;
                hmi_voice_slots[driver][voice]->word_48 = 0;
                hmi_voice_slots[driver][voice]->dword_4c = 0;
                hmi_voice_slots[driver][voice]->dword_50 = sample->dword_08;
                hmi_voice_slots[driver][voice]->word_54 = sample->word_40;
                hmi_voice_slots[driver][voice]->word_56 = sample->word_44;
                hmi_voice_slots[driver][voice]->word_58 = sample->word_48;
                hmi_voice_slots[driver][voice]->word_5a = sample->word_4c;
                hmi_voice_slots[driver][voice]->word_5c = sample->word_50;
                hmi_voice_slots[driver][voice]->word_5e = sample->word_54;
                hmi_voice_slots[driver][voice]->word_60 = sample->word_58;
                hmi_voice_slots[driver][voice]->dword_64 = 0;
                hmi_voice_slots[driver][voice]->word_68 = 0;
                return voice;
            } else {
                continue;
            }
        }
        return -1;
    }
    return 0;
}

int sub_74A10(unsigned driver, unsigned voice)
{
    if (voice < 32) {
        if ((hmi_voice_slots[driver][voice]->flags.raw & 0x8000) &&
            !(hmi_voice_slots[driver][voice]->flags.bytes.high & 0x10)) {
            hmi_voice_slots[driver][voice]->flags.bytes.high &= 0x7f;
            hmi_voice_slots[driver][voice]->word_34 = 0;
            return 0;
        } else {
            return 0;
        }
    }
    return 10;
}

int hmi_configure_sample_voice(unsigned driver, unsigned voice,
                               hmi_sample_template __far *sample)
{
    unsigned char __far *driver_address;
    unsigned char __far *sample_address;
    unsigned long sample_first_word;

    driver_address = hmi_driver_entrypoints[driver].pointer;
    if (hmi_driver_ids[driver] >= 0xe106) {
        sample_address = sample->pointer_00.pointer;
        sample_first_word = sample->dword_08;
    } else {
        hmi_voice_slots[driver][voice]->pointer_00.pointer = sample->pointer_00.pointer;
        hmi_voice_slots[driver][voice]->pointer_08.pointer = sample->pointer_00.pointer;
        hmi_voice_slots[driver][voice]->pointer_10.pointer = sample->pointer_00.pointer + sample->dword_30;
        hmi_voice_slots[driver][voice]->dword_50 = sample->dword_08;
        if (sample->flags_28.bytes.low & 0x40) {
            hmi_voice_slots[driver][voice]->dword_18 = sample->dword_30;
            hmi_voice_slots[driver][voice]->dword_1c = sample->dword_30;
            hmi_voice_slots[driver][voice]->dword_20 = sample->dword_34;
            hmi_voice_slots[driver][voice]->dword_2c =
                sample->dword_2c - (sample->dword_30 + sample->dword_34);
        } else {
            hmi_voice_slots[driver][voice]->dword_18 = sample->dword_08;
            hmi_voice_slots[driver][voice]->dword_1c = sample->dword_08;
        }
        hmi_voice_slots[driver][voice]->word_34 = sample->word_18;
        hmi_voice_slots[driver][voice]->flags.raw = sample->flags_28.raw | 0x8800;
        hmi_voice_slots[driver][voice]->pointer_3c.pointer = sample->pointer_1c.pointer;
        hmi_voice_slots[driver][voice]->word_38 = sample->word_0c;
        hmi_voice_slots[driver][voice]->word_4a = sample->word_24;
        return 0;
    }
    return 0;
}

void hmi_samples_end(void) {}
