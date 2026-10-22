/* SDK public names; DOS-derived HMIDET load/detection group 1:7233a..73023. */
#include "hmi_local.h"
#include <i86.h>

extern hmi_s32 close(hmi_s32 handle);
extern hmi_s32 __cdecl sub_713E7(const char *path, hmi_s32 mode);
extern void sub_7165A(hmi_s32 handle, void __far *memory,
    hmi_u32 count, hmi_s32 *value);
extern hmi_s32 sub_716AB(hmi_s32 handle, hmi_s32 position,
    hmi_s32 origin);
extern void sub_7560B(hmi_s32 handle, void *buffer, hmi_u32 size);
extern void sub_7875F(char *destination, const char *source);
extern void sub_7877E(char *destination, const char *source);
extern hmi_s32 __cdecl sosDRVLockMemory(hmi_u32 address, hmi_u32 size);
extern hmi_s32 __cdecl sosDRVUnLockMemory(hmi_u32 address, hmi_u32 size);
extern void __far *__cdecl sosAllocateFarMem(hmi_u32 size, hmi_u32 *address,
    hmi_u32 *selector);
extern hmi_s32 __cdecl sosFreeSelector(void __far *memory, hmi_u32 size);
extern void __far *__cdecl sosCreateAliasCS(void __far *memory);
extern void __cdecl sosDetDRVGetCapsInfo(void __far *memory_b, void __far *memory_a,
    void __far *third);
extern hmi_s32 __cdecl sosDetDRVExist(void __far *memory_b,
    void __far *memory_a);
extern void __cdecl sosDetDRVGetSettings(void __far *memory_b,
    void __far *memory_a);
extern hmi_s32 __cdecl sosDetDRVVerifySettings(void __far *memory_b, hmi_s32 value_00,
    hmi_s32 value_04, hmi_s32 value_08, void __far *memory_a);
extern void __cdecl sosDIGIInitForWindows(hmi_s32 value);
extern void __cdecl sosDIGIUnInitForWindows(hmi_s32 value);
extern void __cdecl sosDetDRVEnvStringInit(void __far *memory_b,
    void __far *memory_a);
extern hmi_s32 __cdecl sosDetDRVGetEnvString(hmi_s32 index);
extern void __cdecl sosDetDRVSetEnvString(hmi_s32 index, hmi_s32 value);
extern hmi_s32 __cdecl sub_79038(void);

void sosDIGIDetec_Start(void) {}

hmi_s32 sosDIGIDetectInit(const char __far *prefix)
{
    hmi_u32 file_length;
    hmi_s32 size;

    if (_wDETInitialized != 0) {
        return 3;
    } else {
        size = 0x2c;
        if (prefix != 0) {
            sub_7875F(_pSOSTempDriverPath, (const char *)prefix);
            sub_7877E(_pSOSTempDriverPath, "hmidet.386");
        } else {
            sub_7875F(_pSOSTempDriverPath, "hmidet.386");
        }

        _hDETFile = sub_713E7(_pSOSTempDriverPath, 0x200);
        if (_hDETFile == -1) {
            return 0x0f;
        } else {
            sub_7560B(_hDETFile, &_sDETFileHeader, size);
            file_length = _sDETFileHeader.lFileSize;
            _lpDETDriverBuffer = (void __far *)sosAllocateFarMem(0x1000,
                &_wDETMemHandle, &_wDETLinear);
            _lpDETDriverBufferCS = (void __far *)sosCreateAliasCS(_lpDETDriverBuffer);
            if (sosDRVLockMemory(_wDETLinear, 0x1000) != 0) {
                return 5;
            } else {
                sub_716AB(_hDETFile, 0, 0);
                _wDETInitialized = 1;
                return 0;
            }
        }
    }
}

hmi_s32 sosDIGIDetectUnInit(void)
{

    _wDETInitialized = 0;
    close(_hDETFile);
    if (sosDRVUnLockMemory(_wDETLinear, 0x1000) != 0) {
        return 5;
    } else {
        sosFreeSelector(_lpDETDriverBuffer, _wDETMemHandle);
        sosFreeSelector(_lpDETDriverBufferCS, _wDETMemHandle);
        return 0;
    }
}

hmi_s32 sosDIGIDetectFindHardware(hmi_u32 port, void __far *source,
    hmi_s32 __far *output)
{
    hmi_s32 record;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 position;
    hmi_s32 value;
    hmi_s32 index;
    hmi_s32 error;

    size = 0x30;
    index = 0;
    error = 0;
    if (source == 0) {
        return 2;
    }
    if (port < 0xe000 || port > 0xe200) {
        return 6;
    }

    found = 0;
    status = 0;
    _wDETDriverIndexCur = 0;
    sub_716AB(_hDETFile, 0, 0);
    sub_7560B(_hDETFile, &_sDETFileHeader, 0x2c);
    while (_wDETDriverIndexCur <= _sDETFileHeader.wDrivers
            && found == 0) {
        position = sub_716AB(_hDETFile, 0, 1);
        sub_7560B(_hDETFile, &_sDETDriverHeader, size);
        record = _sDETDriverHeader.wSize;
        if (_sDETDriverHeader.wDeviceID == port
                && ((*((unsigned char *)&_sDETDriverHeader.wExtenderType + 1)) & 0x80) != 0) {
            found = 1;
            sub_7165A(_hDETFile, _lpDETDriverBuffer, record,
                &value);
        } else {
            sub_716AB(_hDETFile, record, 1);
        }
        _wDETDriverIndexCur++;
    }

    if (found == 1) {
        sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer,
            (void __far *)&_sDETCaps);
        if (((*((unsigned char *)&_sDETCaps.wFlags + 1)) & 1) != 0) {
            sosDetDRVEnvStringInit(_lpDETDriverBufferCS, _lpDETDriverBuffer);
            index = 0;
            while (sosDetDRVGetEnvString(index) != 0) {
                sosDetDRVGetEnvString(index);
                error = sub_79038();
                sosDetDRVSetEnvString(index, error);
                index++;
            }
        }
        status = sosDetDRVExist(_lpDETDriverBufferCS, _lpDETDriverBuffer);
        if (status != 0) {
            sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer, source);
            _lpDETDeviceCaps = source;
            *output = status;
            _dwDETDriverIndex = position;
            return 0;
        } else {
            return 8;
        }
    } else {
        return 7;
    }

}

hmi_s32 sosDIGIDetectGetCaps(hmi_u32 port, void __far *source)
{
    hmi_s32 value;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 position;
    hmi_s32 read_result;

    size = 0x30;
    if (source == 0) {
        return 2;
    }
    if (port < 0xe000 || port > 0xe200) {
        return 6;
    }

    found = 0;
    status = 0;
    _wDETDriverIndexCur = 0;
    sub_716AB(_hDETFile, 0, 0);
    sub_7560B(_hDETFile, &_sDETFileHeader, 0x2c);
    while (_wDETDriverIndexCur <= _sDETFileHeader.wDrivers
            && found == 0) {
        position = sub_716AB(_hDETFile, 0, 1);
        sub_7560B(_hDETFile, &_sDETDriverHeader, size);
        value = _sDETDriverHeader.wSize;
        if (_sDETDriverHeader.wDeviceID == port
                && ((*((unsigned char *)&_sDETDriverHeader.wExtenderType + 1)) & 0x80) != 0) {
            found = 1;
            sub_7165A(_hDETFile, _lpDETDriverBuffer, value,
                &read_result);
        } else {
            sub_716AB(_hDETFile, value, 1);
        }
        _wDETDriverIndexCur++;
    }

    if (found == 1) {
        sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer, source);
        _lpDETDeviceCaps = source;
        _dwDETDriverIndex = position;
        return 0;
    } else {
        return 7;
    }

}

hmi_s32 sosDIGIDetectFindFirst(const char __far *name, void __far *output)
{
    hmi_s32 value_24;
    hmi_s32 value_28;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 file_position;
    hmi_s32 result_value;
    hmi_s32 index;
    hmi_s32 error;

    value_28 = -1;
    size = 0x30;
    index = 0;
    error = 0;
    if (name == 0)
        return 2;
    found = 0;
    status = 0;
    _wDETDriverIndexCur = 0;
    sub_716AB(_hDETFile, 0, 0);
    sub_7560B(_hDETFile, &_sDETFileHeader, 0x2c);
    while (_wDETDriverIndexCur <= _sDETFileHeader.wDrivers
            && found == 0) {
        file_position = sub_716AB(_hDETFile, 0, 1);
        sub_7560B(_hDETFile, &_sDETDriverHeader, size);
        value_24 = _sDETDriverHeader.wSize;
        value_28 = _sDETDriverHeader.wDeviceID;
        if (((*((unsigned char *)&_sDETDriverHeader.wExtenderType + 1)) & 0x80) != 0) {
            sub_7165A(_hDETFile, _lpDETDriverBuffer, value_24,
                &result_value);
            sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer,
                (void __far *)&_sDETCaps);
            if (((*((unsigned char *)&_sDETCaps.wFlags + 1)) & 1) != 0) {
                sosDetDRVEnvStringInit(_lpDETDriverBufferCS, _lpDETDriverBuffer);
                index = 0;
                while (sosDetDRVGetEnvString(index) != 0) {
                    sosDetDRVGetEnvString(index);
                    error = sub_79038();
                    sosDetDRVSetEnvString(index, error);
                    index++;
                }
            }
            status = sosDetDRVExist(_lpDETDriverBufferCS, _lpDETDriverBuffer);
            if (status != 0) {
                _dwDETDriverIndex = file_position;
                sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer,
                    (void __far *)name);
                _lpDETDeviceCaps = (void __far *)name;
                *(hmi_s32 __far *)output = status;
                return 0;
            }
        } else {
            sub_716AB(_hDETFile, value_24, 1);
        }
        _wDETDriverIndexCur++;
    }
    return 7;
}

hmi_s32 sosDIGIDetectFindNext(const char __far *name, void __far *output)
{
    hmi_s32 value_24;
    hmi_s32 value_28;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 file_position;
    hmi_s32 result_value;
    hmi_s32 index;
    hmi_s32 error;

    value_28 = -1;
    size = 0x30;
    index = 0;
    error = 0;
    if (name == 0)
        return 2;
    found = 0;
    status = 0;
    while (_wDETDriverIndexCur <= _sDETFileHeader.wDrivers
            && found == 0) {
        file_position = sub_716AB(_hDETFile, 0, 1);
        sub_7560B(_hDETFile, &_sDETDriverHeader, size);
        value_24 = _sDETDriverHeader.wSize;
        value_28 = _sDETDriverHeader.wDeviceID;
        if (((*((unsigned char *)&_sDETDriverHeader.wExtenderType + 1)) & 0x80) != 0) {
            sub_7165A(_hDETFile, _lpDETDriverBuffer, value_24,
                &result_value);
            sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer,
                (void __far *)&_sDETCaps);
            if (((*((unsigned char *)&_sDETCaps.wFlags + 1)) & 1) != 0) {
                sosDetDRVEnvStringInit(_lpDETDriverBufferCS, _lpDETDriverBuffer);
                index = 0;
                while (sosDetDRVGetEnvString(index) != 0) {
                    sosDetDRVGetEnvString(index);
                    error = sub_79038();
                    sosDetDRVSetEnvString(index, error);
                    index++;
                }
            }
            status = sosDetDRVExist(_lpDETDriverBufferCS, _lpDETDriverBuffer);
            if (status != 0) {
                _dwDETDriverIndex = file_position;
                sosDetDRVGetCapsInfo(_lpDETDriverBufferCS, _lpDETDriverBuffer,
                    (void __far *)name);
                _lpDETDeviceCaps = (void __far *)name;
                *(hmi_s32 __far *)output = status;
                return 0;
            }
        } else {
            sub_716AB(_hDETFile, value_24, 1);
        }
        _wDETDriverIndexCur++;
    }
    return 7;
}

hmi_s32 sosDIGIDetectGetSettings(hmi_s32 __far *output)
{
    hmi_s32 size;
    hmi_s32 index;

    /* PROVEN DOS store at 1:72dc9; purpose remains OPEN. */
    size = 0x30;
    if (output == 0)
        return 2;
    sub_716AB(_hDETFile, _dwDETDriverIndex, 0);
    index = 0;
    while (((hmi_detect_request __far *)_lpDETDeviceCaps)->resources[index] != -1) {
        sosDIGIInitForWindows(((hmi_detect_request __far *)_lpDETDeviceCaps)->resources[index]);
        index++;
    }
    sosDetDRVGetSettings(_lpDETDriverBufferCS, _lpDETDriverBuffer);
    index = 0;
    while (((hmi_detect_request __far *)_lpDETDeviceCaps)->resources[index] != -1) {
        sosDIGIUnInitForWindows(((hmi_detect_request __far *)_lpDETDeviceCaps)->resources[index]);
        index++;
    }
    output[0] = wDetectPort;
    output[2] = wDetectDMA;
    output[1] = wDetectIRQ;
    output[3] = wDetectParam;
    if (((*(unsigned char *)&_sDETCaps.wFlags) & 1) == 0) {
        if (wDetectIRQ > 1 && wDetectIRQ < 0x10)
            return 0;
        else
            return 0x11;
    }
    return 0;
}

hmi_s32 sosDIGIDetectVerifySettings(hmi_detect_results __far *input)
{
    hmi_u32 value_00;
    hmi_u32 value_04;
    hmi_u32 value_08;
    hmi_s32 size;
    hmi_s32 status;

    /* PROVEN DOS store at 1:72f51; purpose remains OPEN. */
    size = 0x30;
    if (input == 0)
        return 2;
    value_00 = input->values[0];
    value_08 = input->values[2];
    value_04 = input->values[1];
    if (((*(unsigned char *)&_sDETCaps.wFlags) & 1) == 0) {
        if (value_04 < 2 || value_04 > 0x0f)
            return 0x11;
    }
    sosDIGIInitForWindows(input->values[2]);
    status = sosDetDRVVerifySettings(_lpDETDriverBufferCS, value_00, value_04,
        value_08, _lpDETDriverBuffer);
    sosDIGIUnInitForWindows(input->values[2]);
    return status;
}

void sosDIGIDetec_End(void) {}
