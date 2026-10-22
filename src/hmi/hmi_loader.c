/* SDK public names; DOS-derived HMIDRV loader group 1:791a8..79769. */
#include "hmi_local.h"

extern hmi_u32 __cdecl sosDRVUnLockMemory(hmi_u32 first, hmi_u32 second);
extern void __cdecl sosFreeSelector(void __far *memory,
                              hmi_u32 resource);
extern void sub_7875F(hmi_u8 *name, hmi_u8 *path);
extern void sub_7877E(hmi_u8 *name, const char *filename);
extern hmi_u32 __cdecl sub_713E7(hmi_u8 *name, hmi_u32 size);
extern void sub_7560B(hmi_u32 handle, void *buffer, hmi_u32 size);
extern hmi_far_address __far * __cdecl sosAllocateFarMem(
    hmi_u32 current, hmi_u32 *context, hmi_u32 *value);
extern hmi_far_address __far * __cdecl sosCreateAliasCS(
    hmi_u32 offset, hmi_u16 selector);
extern hmi_u32 __cdecl sosDRVLockMemory(hmi_u32 value, hmi_u32 current);
extern void sub_7165A(hmi_u32 handle, void __far *memory, hmi_u32 count,
                     hmi_u32 *value);
extern void sub_716AB(hmi_u32 handle, hmi_u32 current, hmi_u32 mode);
extern void close(hmi_u32 handle);

void sosDIGILoad_Start(void) {}

hmi_u32 sosDIGILoadDriver(hmi_u32 requested, hmi_u32 slot,
                        hmi_far_address __far *record,
                        hmi_far_address __far *scratch,
                        hmi_u32 *context, hmi_u8 *slot_out,
                        hmi_u32 *value_out)
{
    hmi_u32 file_handle;
    hmi_u32 current;
    hmi_u32 size_a;
    hmi_u32 size_b;
    hmi_u32 found;
    hmi_u32 i;
    hmi_far_value second;
    hmi_far_value first;
    hmi_u32 value;
    hmi_u32 local_value;
    hmi_u32 result;

    size_a = 0x2c;
    size_b = 0x30;
    found = 0;
    i = 0;
    if (slot > 5) {
        result = 10;
        goto done;
    }
    if (_wSOSDriverLoaded[slot] != 0) {
        result = 9;
        goto done;
    }
    if (requested < 0xe000 || requested > 0xe200) {
        result = 6;
        goto done;
    }

    sub_7875F(_pSOSTempDriverPath, _pSOSDriverPath);
    sub_7877E(_pSOSTempDriverPath, "hmidrv.386");
    file_handle = sub_713E7(_pSOSTempDriverPath, 0x200);
    if (file_handle == (hmi_u32)-1) {
        result = 15;
        goto done;
    }
    sub_7560B(file_handle, sLOADFileHeader.szName, size_a);
    while (i <= sLOADFileHeader.wDrivers && found == 0) {
        sub_7560B(file_handle, sLOADDriverHeader.szName, size_b);
        current = sLOADDriverHeader.wSize;
        if (sLOADDriverHeader.wDeviceID == requested &&
            ((*((unsigned char *)&sLOADDriverHeader.wExtenderType + 1)) & 0x80)) {
            found = 1;
            first.pointer = sosAllocateFarMem(current, context,
                                                            &value);
            second.pointer = sosCreateAliasCS(
                first.parts.offset, first.parts.selector);
            if (sosDRVLockMemory(value, current)) {
                result = 5;
                goto done;
            }
            sub_7165A(file_handle, first.pointer, current, &local_value);
            record->selector = second.parts.selector;
            record->offset = second.parts.offset;
            scratch->selector = first.parts.selector;
            scratch->offset = first.parts.offset;
            *slot_out = (hmi_u8)current;
            *value_out = value;
        } else {
            sub_716AB(file_handle, current, 1);
        }
        i++;
    }
    close(file_handle);
    _wSOSDriverType[slot] = sLOADDriverHeader.wDeviceID;
    _wSOSDriverLoaded[slot] = 1;
    result = 0;
done:
    return result;
}

hmi_u32 sosDIGIUnLoadDriver(hmi_u32 slot)
{
    if (slot > 5) {
        return 10;
    }
    if (_wSOSDriverLoaded[slot] == 0) {
        return 10;
    }
    _wSOSDriverLoaded[slot] = 0;
    if (sosDRVUnLockMemory(_wSOSDriverLinear[slot],
                        _wSOSDriverSize[slot])) {
        return 5;
    }
    sosFreeSelector(_lpSOSDriver[slot].pointer,
                                  _wSOSDriverMemHandle[slot]);
    sosFreeSelector(_lpSOSDriverCS[slot].pointer,
                                  _wSOSDriverMemHandle[slot]);
    return 0;
}

hmi_u32 sosDIGILoadTimer(hmi_u32 requested,
                            hmi_far_address __far *record,
                            hmi_far_address __far *scratch,
                            hmi_u32 *context, hmi_u8 *slot_out,
                            hmi_u32 *value_out)
{
    hmi_u32 file_handle;
    hmi_u32 current;
    hmi_u32 size_a;
    hmi_u32 size_b;
    hmi_u32 found;
    hmi_u32 i;
    hmi_far_value second;
    hmi_far_value first;
    hmi_u32 value;
    hmi_u32 local_value;
    hmi_u32 result;

    size_a = 0x2c;
    size_b = 0x30;
    found = 0;
    i = 0;
    if (requested < 0x1000 || requested > 0x1023) {
        result = 6;
        goto done;
    }

    sub_7875F(_pSOSTempDriverPath, _pSOSDriverPath);
    sub_7877E(_pSOSTempDriverPath, "hmidrv.386");
    file_handle = sub_713E7(_pSOSTempDriverPath, 0x200);
    if (file_handle == (hmi_u32)-1) {
        result = 15;
        goto done;
    }
    sub_7560B(file_handle, sLOADFileHeader.szName, size_a);
    while (i <= sLOADFileHeader.wDrivers && found == 0) {
        sub_7560B(file_handle, sLOADDriverHeader.szName, size_b);
        current = sLOADDriverHeader.wSize;
        if (sLOADDriverHeader.wDeviceID == requested &&
            ((*((unsigned char *)&sLOADDriverHeader.wExtenderType + 1)) & 0x40)) {
            found = 1;
            current += 0x4000;
            first.pointer = sosAllocateFarMem(current, context,
                                                            &value);
            second.pointer = sosCreateAliasCS(
                first.parts.offset, first.parts.selector);
            if (sosDRVLockMemory(value, current)) {
                result = 5;
                goto done;
            }
            sub_7165A(file_handle, first.pointer, current, &local_value);
            record->selector = second.parts.selector;
            record->offset = second.parts.offset;
            scratch->selector = first.parts.selector;
            scratch->offset = first.parts.offset;
            *slot_out = (hmi_u8)current;
            *value_out = value;
        } else {
            sub_716AB(file_handle, current, 1);
        }
        i++;
    }
    close(file_handle);
    result = 0;
done:
    return result;
}

hmi_u32 sosDIGIUnLoadTimer(hmi_u32 slot)
{
    if (sosDRVUnLockMemory(_wSOSTimerLinear[slot],
                        _wSOSTimerSize[slot])) {
        return 5;
    }
    sosFreeSelector(_lpSOSTimer[slot].pointer,
                                  _wSOSTimerMemHandle[slot]);
    sosFreeSelector(_lpSOSTimerCS[slot].pointer,
                                  _wSOSTimerMemHandle[slot]);
    return 0;
}

hmi_u32 sosDIGISetActiveTimer(hmi_u32 first,
                                hmi_far_address __far *record,
                                hmi_far_address __far *scratch)
{
    return 0;
}

void sosDIGILoad_End(void) {}
