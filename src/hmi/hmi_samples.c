/* INVENTED names. DOS-derived HMI sample voice operations, 1:743e2..74e4a. */
#include "hmi_local.h"

void sosDIGIStart_Start(void) {}

int sosDIGIStartSample(unsigned driver, _SOS_START_SAMPLE __far *sample)
{
    unsigned voice;
    unsigned char __far *driver_address;
    unsigned char __far *sample_address;
    unsigned long sample_first_word;

    driver_address = _lpSOSDriver[driver].pointer;
    if (_wSOSDriverType[driver] >= 0xe106) {
        sample_address = sample->pointer_00.pointer;
        sample_first_word = sample->dword_08;
    } else {
        for (voice = 0; voice < 32; voice++) {
            if (!(_lpSOSSampleList[driver][voice]->flags.raw & 0x8000)) {
                _lpSOSSampleList[driver][voice]->pointer_00.pointer = sample->pointer_00.pointer;
                _lpSOSSampleList[driver][voice]->pointer_08.pointer = sample->pointer_00.pointer;
                _lpSOSSampleList[driver][voice]->pointer_10.pointer = sample->pointer_00.pointer + sample->dword_30;

                if (sample->flags_28.bytes.low & 0x40) {
                    _lpSOSSampleList[driver][voice]->dword_18 = sample->dword_30;
                    _lpSOSSampleList[driver][voice]->dword_1c = sample->dword_30;
                    _lpSOSSampleList[driver][voice]->dword_20 = sample->dword_34;
                    _lpSOSSampleList[driver][voice]->dword_2c =
                        sample->dword_2c - (sample->dword_30 + sample->dword_34);
                } else {
                    _lpSOSSampleList[driver][voice]->dword_18 = sample->dword_08;
                    _lpSOSSampleList[driver][voice]->dword_1c = sample->dword_08;
                }

                _lpSOSSampleList[driver][voice]->word_32 = sample->word_14;
                _lpSOSSampleList[driver][voice]->word_34 = sample->word_18;
                _lpSOSSampleList[driver][voice]->flags.raw = sample->flags_28.raw | 0xa000;
                _lpSOSSampleList[driver][voice]->word_36 = sample->word_10;
                _lpSOSSampleList[driver][voice]->pointer_3c.pointer = sample->pointer_1c.pointer;
                _lpSOSSampleList[driver][voice]->word_38 = sample->word_0c;
                _lpSOSSampleList[driver][voice]->word_4a = sample->word_24;
                _lpSOSSampleList[driver][voice]->word_3a = 0;
                _lpSOSSampleList[driver][voice]->dword_44 = sample->dword_38;
                _lpSOSSampleList[driver][voice]->word_48 = 0;
                _lpSOSSampleList[driver][voice]->dword_4c = 0;
                _lpSOSSampleList[driver][voice]->dword_50 = sample->dword_08;
                _lpSOSSampleList[driver][voice]->word_54 = sample->word_40;
                _lpSOSSampleList[driver][voice]->word_56 = sample->word_44;
                _lpSOSSampleList[driver][voice]->word_58 = sample->word_48;
                _lpSOSSampleList[driver][voice]->word_5a = sample->word_4c;
                _lpSOSSampleList[driver][voice]->word_5c = sample->word_50;
                _lpSOSSampleList[driver][voice]->word_5e = sample->word_54;
                _lpSOSSampleList[driver][voice]->word_60 = sample->word_58;
                _lpSOSSampleList[driver][voice]->dword_64 = 0;
                _lpSOSSampleList[driver][voice]->word_68 = 0;
                return voice;
            } else {
                continue;
            }
        }
        return -1;
    }
    return 0;
}

int sosDIGIStopSample(unsigned driver, unsigned voice)
{
    if (voice < 32) {
        if ((_lpSOSSampleList[driver][voice]->flags.raw & 0x8000) &&
            !(_lpSOSSampleList[driver][voice]->flags.bytes.high & 0x10)) {
            _lpSOSSampleList[driver][voice]->flags.bytes.high &= 0x7f;
            _lpSOSSampleList[driver][voice]->word_34 = 0;
            return 0;
        } else {
            return 0;
        }
    }
    return 10;
}

int sosDIGIContinueSample(unsigned driver, unsigned voice,
                               _SOS_START_SAMPLE __far *sample)
{
    unsigned char __far *driver_address;
    unsigned char __far *sample_address;
    unsigned long sample_first_word;

    driver_address = _lpSOSDriver[driver].pointer;
    if (_wSOSDriverType[driver] >= 0xe106) {
        sample_address = sample->pointer_00.pointer;
        sample_first_word = sample->dword_08;
    } else {
        _lpSOSSampleList[driver][voice]->pointer_00.pointer = sample->pointer_00.pointer;
        _lpSOSSampleList[driver][voice]->pointer_08.pointer = sample->pointer_00.pointer;
        _lpSOSSampleList[driver][voice]->pointer_10.pointer = sample->pointer_00.pointer + sample->dword_30;
        _lpSOSSampleList[driver][voice]->dword_50 = sample->dword_08;
        if (sample->flags_28.bytes.low & 0x40) {
            _lpSOSSampleList[driver][voice]->dword_18 = sample->dword_30;
            _lpSOSSampleList[driver][voice]->dword_1c = sample->dword_30;
            _lpSOSSampleList[driver][voice]->dword_20 = sample->dword_34;
            _lpSOSSampleList[driver][voice]->dword_2c =
                sample->dword_2c - (sample->dword_30 + sample->dword_34);
        } else {
            _lpSOSSampleList[driver][voice]->dword_18 = sample->dword_08;
            _lpSOSSampleList[driver][voice]->dword_1c = sample->dword_08;
        }
        _lpSOSSampleList[driver][voice]->word_34 = sample->word_18;
        _lpSOSSampleList[driver][voice]->flags.raw = sample->flags_28.raw | 0x8800;
        _lpSOSSampleList[driver][voice]->pointer_3c.pointer = sample->pointer_1c.pointer;
        _lpSOSSampleList[driver][voice]->word_38 = sample->word_0c;
        _lpSOSSampleList[driver][voice]->word_4a = sample->word_24;
        return 0;
    }
    return 0;
}

void sosDIGIStart_End(void) {}
