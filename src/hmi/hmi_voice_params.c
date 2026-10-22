/* INVENTED names. DOS-derived HMI voice parameter accessors, 1:74e4a..754ce. */
#include "hmi_local.h"

void sosDIGICntl_Start(void) {}

unsigned long sosDIGIGetDMAPosition(unsigned driver)
{
    unsigned long value;

    value = _lpSOSDMAFillCount[driver]->offset;
    return value;
}

int sosDIGISetSampleVolume(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        previous = _lpSOSSampleList[driver][voice]->word_32;
        _lpSOSSampleList[driver][voice]->word_32 = value;
    } else {
        return 0;
    }
    return previous;
}

int sosDIGIGetSampleVolume(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->word_32;
    } else {
        return 0;
    }
    return value;
}

int sosDIGISetChannel(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        previous = _lpSOSSampleList[driver][voice]->word_36;
        _lpSOSSampleList[driver][voice]->word_36 = value;
    } else {
        return 0;
    }
    return previous;
}

int sosDIGIGetChannel(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->word_36;
    } else {
        return 0;
    }
    return value;
}

int sosDIGIGetBytesProcessed(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->dword_4c;
    } else {
        return 0;
    }
    return value;
}

int sosDIGIGetLoopCount(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->word_38;
    } else {
        return 0;
    }
    return value;
}

int sosDIGISetPanLocation(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        previous = _lpSOSSampleList[driver][voice]->word_54;
        _lpSOSSampleList[driver][voice]->word_54 = value;
    } else {
        return 0;
    }
    return previous;
}

int sosDIGIGetPanLocation(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->word_54;
    } else {
        return 0;
    }
    return value;
}

int sosDIGISetPanSpeed(unsigned driver, unsigned voice, int value)
{
    int previous;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        previous = _lpSOSSampleList[driver][voice]->word_56;
        _lpSOSSampleList[driver][voice]->word_56 = value;
    } else {
        return 0;
    }
    return previous;
}

int sosDIGIGetPanSpeed(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->word_56;
    } else {
        return 0;
    }
    return value;
}

int sosDIGISetPitch(unsigned driver, unsigned voice, unsigned long value)
{
    unsigned long previous;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        previous = _lpSOSSampleList[driver][voice]->dword_44;
        _lpSOSSampleList[driver][voice]->dword_44 = value;
    } else {
        return 0;
    }
    return previous;
}

int sosDIGIGetPitch(unsigned driver, unsigned voice)
{
    int value;

    if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) {
        value = _lpSOSSampleList[driver][voice]->dword_44;
    } else {
        return 0;
    }
    return value;
}

int sosDIGIGetSampleID(unsigned driver, unsigned voice)
{
    return _lpSOSSampleList[driver][voice]->word_34;
}

int sosDIGIGetSampleHandle(unsigned driver, int value)
{
    unsigned voice;

    for (voice = 0; voice < 32; voice++) {
        if (_lpSOSSampleList[driver][voice]->word_34 == value)
            return voice;
    }
    return -1;
}

int sosDIGISetMasterVolume(unsigned driver, unsigned selector)
{
    _lpSOSDMAFillCount[driver]->selector = selector;
    return 0;
}

void sosDIGICntl_End(void) {}
