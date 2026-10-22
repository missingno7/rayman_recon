/* INVENTED names. DOS-derived HMI voice parameter accessors, 1:74e4a..754ce. */
#include "hmi_local.h"

void hmi_voice_params_begin(void) {}

unsigned long hmi_get_driver_buffer_offset(unsigned driver)
{
    unsigned long value;

    value = hmi_driver_buffers[driver]->offset;
    return value;
}

int sub_74E8B(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        previous = hmi_voice_slots[driver][voice]->word_32;
        hmi_voice_slots[driver][voice]->word_32 = value;
    } else {
        return 0;
    }
    return previous;
}

int hmi_get_voice_word_32(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->word_32;
    } else {
        return 0;
    }
    return value;
}

int hmi_set_voice_word_36(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        previous = hmi_voice_slots[driver][voice]->word_36;
        hmi_voice_slots[driver][voice]->word_36 = value;
    } else {
        return 0;
    }
    return previous;
}

int hmi_get_voice_word_36(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->word_36;
    } else {
        return 0;
    }
    return value;
}

int hmi_get_voice_dword_4c(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->dword_4c;
    } else {
        return 0;
    }
    return value;
}

int hmi_get_voice_word_38(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->word_38;
    } else {
        return 0;
    }
    return value;
}

int sub_7512E(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        previous = hmi_voice_slots[driver][voice]->word_54;
        hmi_voice_slots[driver][voice]->word_54 = value;
    } else {
        return 0;
    }
    return previous;
}

int hmi_get_voice_word_54(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->word_54;
    } else {
        return 0;
    }
    return value;
}

int hmi_set_voice_word_56(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        previous = hmi_voice_slots[driver][voice]->word_56;
        hmi_voice_slots[driver][voice]->word_56 = value;
    } else {
        return 0;
    }
    return previous;
}

int hmi_get_voice_word_56(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->word_56;
    } else {
        return 0;
    }
    return value;
}

int hmi_set_voice_dword_44(unsigned driver, unsigned voice, unsigned long value)
{
    unsigned long previous;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        previous = hmi_voice_slots[driver][voice]->dword_44;
        hmi_voice_slots[driver][voice]->dword_44 = value;
    } else {
        return 0;
    }
    return previous;
}

int hmi_get_voice_dword_44(unsigned driver, unsigned voice)
{
    int value;

    if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000) {
        value = hmi_voice_slots[driver][voice]->dword_44;
    } else {
        return 0;
    }
    return value;
}

int hmi_get_voice_word_34(unsigned driver, unsigned voice)
{
    return hmi_voice_slots[driver][voice]->word_34;
}

int hmi_find_voice_by_word_34(unsigned driver, int value)
{
    unsigned voice;

    for (voice = 0; voice < 32; voice++) {
        if (hmi_voice_slots[driver][voice]->word_34 == value)
            return voice;
    }
    return -1;
}

int hmi_set_driver_buffer_selector(unsigned driver, unsigned selector)
{
    hmi_driver_buffers[driver]->selector = selector;
    return 0;
}

void hmi_voice_params_end(void) {}
