/* SDK public bindings. DOS timer group 1:71b83..7233a. */
#include "hmi_local.h"
#include <i86.h>

extern void __cdecl sosTIMERDRVSetRate(u32 divisor);
extern void __cdecl sosTIMERDRVInit(u32 vector, void (__far *handler)(void));
extern void sosTIMERDRVDisable(void);
extern void sosTIMERDRVEnable(void);
extern void sosTIMERDRVUnInit(void);
extern void sosTIMERDRVCallOld(void);

/* PROVEN contiguous timer state at 2:2e707..2:2e83c in object 2. */
WORD _wTIMERValue = 0xffffffffUL;
void __far *_lpTIMEREvents[16] = { 0 };
WORD _wTIMEREventRate[16] = { 0 };
WORD _dwTIMEREventFraction[16] = { 0 };
WORD _dwTIMEREventFractionCurrent[16] = { 0 };
BYTE _bSOSMIDITimerSongHandler[16] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
BYTE _bSOSMIDISongHandle = 0;

void __far sosTIMERHandler(void);
void __far sosTIMEROldHandler(void);

void sosDIGITmr_Start(void) {}

s32 sosTIMERInitSystem(u32 period, u32 flags)
{
    /* HYPOTHESIS: models the unused dword at EBP-4 in the DOS frame. */
    u32 stack_slot;

    if (flags & 2)
        _bTIMERDPMI = 1;
    else
        _bTIMERDPMI = 0;

    if (!(flags & 1)) {
        _bTIMERInstalled = 1;
        sosTIMERDRVDisable();
        sosTIMERDRVInit(0xffff, sosTIMERHandler);
        sosTIMERDRVEnable();
    } else {
        _bTIMERInstalled = 0;
    }

    if (period != 0 && !(flags & 1)) {
        if (period == 0xff00) {
            sosTIMERSetRate(0xffff);
            _wTIMEREventRate[15] = 0xff00;
        } else {
            sosTIMERSetRate(0x1234dc / period);
            _wTIMEREventRate[15] = period;
        }
        _lpTIMEREvents[15] = (void __far *)sosTIMEROldHandler;
        _dwTIMEREventFraction[15] = 0x10000;
    } else {
        _wTIMERValue = 0xffff;
    }

    return 0;
}

s32 sosTIMERUnInitSystem(u32 flags)
{
    if (_bTIMERInstalled) {
        sosTIMERDRVDisable();
        sosTIMERDRVUnInit();
        sosTIMERDRVEnable();
    }

    return 0;
}

s32 sosTIMERRegisterEvent(u32 period, void __far *callback, s32 __far *slot_out)
{
    u32 i;
    u32 slot;
    u32 factor;
    u32 value;

    factor = 0;
    for (i = 0; i < 16; i++) {
        if (_lpTIMEREvents[i] == 0)
            break;
    }
    if (i >= 16) {
        return 11;
    } else {
        if (_bTIMERInstalled)
            sosTIMERDRVDisable();
        slot = i;
        _lpTIMEREvents[i] = callback;
        _wTIMEREventRate[i] = period;

        if (0x1234dc / period < _wTIMERValue) {
            sosTIMERSetRate(0x1234dc / period);
            factor = (_wTIMERValue << 16) / (0x1234dc / period);
        }

        for (i = 0; i < 16; i++) {
            if (_lpTIMEREvents[i] != 0) {
                if (_wTIMEREventRate[i] == 0xff00) {
                    if (_wTIMERValue == 0xffff)
                        _dwTIMEREventFraction[i] = 0x10000;
                    else
                        _dwTIMEREventFraction[i] = 0x123333 / (0x1234dc / _wTIMERValue);
                } else {
                    _dwTIMEREventFraction[i] = (_wTIMEREventRate[i] << 16) /
                        (0x1234dc / _wTIMERValue);
                }

                if (factor != 0) {
                    value = _dwTIMEREventFractionCurrent[i] * (factor & 0xffff) >> 16;
                    value += _dwTIMEREventFractionCurrent[i] * (factor >> 16) >> 16;
                    _dwTIMEREventFractionCurrent[i] = value;
                }
            }
        }

        if (_bTIMERInstalled)
            sosTIMERDRVEnable();
        *slot_out = slot;
        return 0;
    }
}

s32 sosTIMERAlterEventRate(u32 slot, u32 period)
{
    u32 i;

    if (slot >= 16)
        return 10;

    if (_lpTIMEREvents[slot] != 0) {
        if (_bTIMERInstalled)
            sosTIMERDRVDisable();

        _wTIMEREventRate[slot] = period;
        if (0x1234dc / period < _wTIMERValue)
            sosTIMERSetRate(0x1234dc / period);

        for (i = 0; i < 16; i++) {
            if (_lpTIMEREvents[i] != 0) {
                if (_wTIMEREventRate[i] == 0xff00) {
                    if (_wTIMERValue == 0xffff)
                        _dwTIMEREventFraction[i] = 0x10000;
                    else
                        _dwTIMEREventFraction[i] = 0x123333 / (0x1234dc / _wTIMERValue);
                } else {
                    _dwTIMEREventFraction[i] = (_wTIMEREventRate[i] << 16) /
                        (0x1234dc / _wTIMERValue);
                }

                _dwTIMEREventFractionCurrent[i] = 0;
            }
        }

        if (_bTIMERInstalled)
            sosTIMERDRVEnable();
    } else {
        return 10;
    }
    return 0;
}

s32 sosTIMERRemoveEvent(u32 slot)
{
    u32 i;
    u32 period;

    period = 0;
    _lpTIMEREvents[slot] = 0;
    for (i = 0; i < 16; i++) {
        if (_lpTIMEREvents[i] != 0) {
            if (_wTIMEREventRate[i] > period && _wTIMEREventRate[i] != 0xff00)
                period = _wTIMEREventRate[i];
        }
    }

    if (period != 0)
        sosTIMERSetRate(0x1234dc / period);
    else
        sosTIMERSetRate(0xffff);

    if (_bTIMERInstalled)
        sosTIMERDRVDisable();

    for (i = 0; i < 16; i++) {
        if (_lpTIMEREvents[i] != 0) {
            if (_wTIMEREventRate[i] == 0xff00) {
                if (_wTIMERValue == 0xffff)
                    _dwTIMEREventFraction[i] = 0x10000;
                else
                    _dwTIMEREventFraction[i] = 0x123333 / (0x1234dc / _wTIMERValue);
            } else {
                _dwTIMEREventFraction[i] = (_wTIMEREventRate[i] << 16) /
                    (0x1234dc / _wTIMERValue);
            }
            _dwTIMEREventFractionCurrent[i] = 0;
        }
    }

    if (_bTIMERInstalled)
        sosTIMERDRVEnable();
    return 0;
}

u32 sosTIMERGetEventRate(u32 slot)
{
    return _wTIMEREventRate[slot];
}

s32 sosTIMERSetRate(u32 divisor)
{
    _wTIMERValue = divisor;
    sosTIMERDRVSetRate(divisor);
    return 0;
}

void __far sosTIMERHandler(void)
{
    _wSOSTimerEntered++;
    for (_wSOSTimerEventIndex = 0;
         _wSOSTimerEventIndex < 16; _wSOSTimerEventIndex++) {
        if (_lpTIMEREvents[_wSOSTimerEventIndex] != 0) {
            _dwTIMEREventFractionCurrent[_wSOSTimerEventIndex] +=
                _dwTIMEREventFraction[_wSOSTimerEventIndex];
            if (((u8 *)&_dwTIMEREventFractionCurrent[_wSOSTimerEventIndex])[2] & 1) {
                ((u16 *)&_dwTIMEREventFractionCurrent[_wSOSTimerEventIndex])[1] = 0;
                if (_bSOSMIDITimerSongHandler[_wSOSTimerEventIndex] != 0xff)
                    _bSOSMIDISongHandle = _bSOSMIDITimerSongHandler[_wSOSTimerEventIndex];
                ((void (__far *)(void))_lpTIMEREvents[_wSOSTimerEventIndex])();
            }
        }
    }
    _wSOSTimerEntered--;
}

void __far sosTIMEROldHandler(void)
{
    sosTIMERDRVCallOld();
}

void sosDIGITmr_End(void) {}
