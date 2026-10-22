/* INVENTED descriptive names; DOS-derived HMI system group, 1:73023..743e2. */
#include "hmi_local.h"
#include <i86.h>

void sosDIGIInit_Start(void) {}
void sosDIGIInit_End(void);

extern void sosDIGIErr_Start(void);
extern void sosDIGIErr_End(void);
extern void sosDIGICaps_Start(void);
extern void sosDIGICaps_End(void);
extern void sosDIGIDone_Start(void);
extern void sosDIGIDone_End(void);
extern void sosDIGILoad_Start(void);
extern void sosDIGILoad_End(void);
extern void sosDIGIPlyng_Start(void);
extern void sosDIGIPlyng_End(void);
extern void sosDIGIRate_Start(void);
extern void sosDIGIRate_End(void);
extern void sosDIGIStart_Start(void);
extern void sosDIGIStart_End(void);
extern void sosDIGITmr_Start(void);
extern void sosDIGITmr_End(void);
extern void sosDIGICntl_Start(void);
extern void sosDIGICntl_End(void);
extern void sosDIGIDetec_Start(void);
extern void sosDIGIDetec_End(void);
extern void sosDIGITimer_Start(void);
extern void sosDIGITimer_End(void);
extern void sosDIGIDrv_Start(void);
extern void sosDIGIDrv_End(void);

extern char _pSOSDriverPath[0xff];
extern BOOL _wSOSSystemInitialized;
extern VDS_STRUCT _sSOSVDSInfo;
extern int __cdecl sosRealFree(unsigned);
extern void __cdecl sosDRVStop(void __far *, void __far *);
extern void __cdecl sosDRVUnInit(void __far *, void __far *);
extern void __cdecl sosDIGIUnInitForWindows(unsigned);
extern int __cdecl sosDRVLockMemory(unsigned, unsigned);
extern int __cdecl sosDRVUnLockMemory(unsigned, unsigned);
extern void sub_797F5(char __far *, const char __far *);
extern unsigned sosDRVIsWindowsActive(void);
extern int __cdecl sosRealAlloc(unsigned, unsigned *, unsigned *);
extern int __cdecl sosDRVAllocVDSStruct(unsigned, unsigned *, unsigned *);
extern unsigned __cdecl sosDRVVDSGetBuffer(unsigned);
extern void sub_79824(void __far *, void __far *, unsigned);
extern void __cdecl sosDRVFreeVDSStruct(unsigned, unsigned);
extern unsigned __cdecl sosDRVMakeDMASelector(unsigned);
extern unsigned __cdecl sosDRVFreeDMASelector(unsigned);
extern void __cdecl sosDRVVDSFreeBuffer(unsigned);
extern void sosDIGIUnLoadDriver(unsigned);
extern void sosDIGIUnLoadTimer(unsigned);
extern void sosFreeVDSPage(unsigned, unsigned, unsigned);
extern unsigned sosDIGILoadDriver(unsigned, unsigned, void __far *, void __far *,
                          unsigned *, unsigned *, unsigned *);
extern unsigned sosDIGIGetDeviceCaps(unsigned, unsigned, void __far *);
extern unsigned sosDIGILoadTimer(unsigned, void __far *, void __far *,
                          unsigned *, unsigned *, unsigned *);
extern void __cdecl sosDRVSetupCallFunctions(void __far *, void __far *, void __far *, void __far *);
extern void __cdecl sosDRVGetFillInfo(void __far *, void __far *, void __far *,
                              unsigned, unsigned, unsigned, unsigned);
extern void __cdecl sosFillSampleStructs(unsigned, void __far *);
extern void __cdecl sosDIGIInitForWindows(unsigned);
extern void __cdecl sosDRVInit(void __far *, void __far *,
                              unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
extern void __cdecl sosDRVSetAction(void __far *, void __far *);
extern void __cdecl sosDRVSetRate(void __far *, void __far *, unsigned);
extern void __cdecl sosDRVStart(void __far *, void __far *, unsigned, unsigned);
extern unsigned long __far *sosDIGIAllocateBuffer(unsigned, unsigned *, unsigned *);

#define HMI_SYSTEM_BASE ((unsigned char *)&_sosDIGIData_Start)
#define HMI_SYSTEM_U32(offset) (*(unsigned *)(HMI_SYSTEM_BASE + (offset)))
#define HMI_SYSTEM_U16(offset) (*(unsigned short *)(HMI_SYSTEM_BASE + (offset)))
#define HMI_SYSTEM_U8(offset) (*(unsigned char *)(HMI_SYSTEM_BASE + (offset)))
#define HMI_SYSTEM_U32_PTR(offset, index) ((unsigned *)(HMI_SYSTEM_BASE + (offset)) + (index))
#define HMI_SYSTEM_FAR(offset, index) (((hmi_far_value *)(HMI_SYSTEM_BASE + (offset)))[index].pointer)
#define HMI_SYSTEM_FAR_SLOT(offset, index) ((hmi_far_slot *)(HMI_SYSTEM_BASE + (offset)) + (index))

unsigned sosDIGILockMemory(void)
{
    if (sosDRVLockMemory((unsigned)sosDIGIErr_Start,
                   (unsigned)sosDIGIErr_End - (unsigned)sosDIGIErr_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGICaps_Start,
                   (unsigned)sosDIGICaps_End - (unsigned)sosDIGICaps_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIDone_Start,
                   (unsigned)sosDIGIDone_End - (unsigned)sosDIGIDone_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIInit_Start,
                   (unsigned)sosDIGIInit_End - (unsigned)sosDIGIInit_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGILoad_Start,
                   (unsigned)sosDIGILoad_End - (unsigned)sosDIGILoad_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIPlyng_Start,
                   (unsigned)sosDIGIPlyng_End - (unsigned)sosDIGIPlyng_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIRate_Start,
                   (unsigned)sosDIGIRate_End - (unsigned)sosDIGIRate_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIStart_Start,
                   (unsigned)sosDIGIStart_End - (unsigned)sosDIGIStart_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGITmr_Start,
                   (unsigned)sosDIGITmr_End - (unsigned)sosDIGITmr_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGICntl_Start,
                   (unsigned)sosDIGICntl_End - (unsigned)sosDIGICntl_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIDetec_Start,
                   (unsigned)sosDIGIDetec_End - (unsigned)sosDIGIDetec_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGITimer_Start,
                   (unsigned)sosDIGITimer_End - (unsigned)sosDIGITimer_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)sosDIGIDrv_Start,
                   (unsigned)sosDIGIDrv_End - (unsigned)sosDIGIDrv_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)&_sosDIGIData_Start,
                   (unsigned)&_sosDIGIData_End - (unsigned)&_sosDIGIData_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)&_sosDIGIData1_Start,
                   (unsigned)&_sosDIGIData1_End - (unsigned)&_sosDIGIData1_Start))
        return 5;
    if (sosDRVLockMemory((unsigned)&_sosDIGIData2_Start,
                   (unsigned)&_sosDIGIData2_End - (unsigned)&_sosDIGIData2_Start))
        return 5;
    return 0;
}

unsigned sosDIGIUnLockMemory(void)
{
    if (sosDRVUnLockMemory((unsigned)sosDIGIErr_Start,
                   (unsigned)sosDIGIErr_End - (unsigned)sosDIGIErr_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGICaps_Start,
                   (unsigned)sosDIGICaps_End - (unsigned)sosDIGICaps_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIDone_Start,
                   (unsigned)sosDIGIDone_End - (unsigned)sosDIGIDone_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIInit_Start,
                   (unsigned)sosDIGIInit_End - (unsigned)sosDIGIInit_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGILoad_Start,
                   (unsigned)sosDIGILoad_End - (unsigned)sosDIGILoad_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIPlyng_Start,
                   (unsigned)sosDIGIPlyng_End - (unsigned)sosDIGIPlyng_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIRate_Start,
                   (unsigned)sosDIGIRate_End - (unsigned)sosDIGIRate_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIStart_Start,
                   (unsigned)sosDIGIStart_End - (unsigned)sosDIGIStart_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGITmr_Start,
                   (unsigned)sosDIGITmr_End - (unsigned)sosDIGITmr_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGICntl_Start,
                   (unsigned)sosDIGICntl_End - (unsigned)sosDIGICntl_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIDetec_Start,
                   (unsigned)sosDIGIDetec_End - (unsigned)sosDIGIDetec_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGITimer_Start,
                   (unsigned)sosDIGITimer_End - (unsigned)sosDIGITimer_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)sosDIGIDrv_Start,
                   (unsigned)sosDIGIDrv_End - (unsigned)sosDIGIDrv_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)&_sosDIGIData_Start,
                   (unsigned)&_sosDIGIData_End - (unsigned)&_sosDIGIData_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)&_sosDIGIData1_Start,
                   (unsigned)&_sosDIGIData1_End - (unsigned)&_sosDIGIData1_Start))
        return 5;
    if (sosDRVUnLockMemory((unsigned)&_sosDIGIData2_Start,
                   (unsigned)&_sosDIGIData2_End - (unsigned)&_sosDIGIData2_Start))
        return 5;
    return 0;
}

unsigned sosDIGIInitSystem(const char __far *path, unsigned unused)
{
    unsigned result;

    result = sosDIGILockMemory();
    if (result)
        return result;
    _wSOSSystemInitialized = 1;
    if (path)
        sub_797F5((char __far *)_pSOSDriverPath, path);
    else
        _pSOSDriverPath[0] = 0;
    return 0;
}

unsigned sosDIGIUnInitSystem(void)
{
    unsigned result;

    result = sosDIGIUnLockMemory();
    if (result)
        return 0;
    _wSOSSystemInitialized = 0;
    return 0;
}

unsigned sosDIGIInitDriver(unsigned driver_id,
                                       _SOS_HARDWARE __far *driver_record,
                                       _SOS_INIT_DRIVER __far *initial_state,
                                       unsigned __far *slot_out)
{
    unsigned value_00;
    unsigned value_0c;
    unsigned value_10_a;
    unsigned value_10_b;
    unsigned value_14;
    unsigned value_18;
    unsigned index;
    unsigned slot;
    unsigned status;
    unsigned linear;
    unsigned cached_driver_value;
    unsigned record_00;
    unsigned record_04;
    unsigned record_0c;
    unsigned fill_index;
    unsigned short value_38;
    unsigned short value_40;
    hmi_far_address field_04_08;
    hmi_far_address field_24_28;
    hmi_far_address field_2c_30;
    hmi_far_address field_34_38;
    hmi_far_address field_3c_40;
    unsigned long __far *dma_area;

    value_00 = initial_state->value_00;
    value_10_a = initial_state->value_10;
    field_04_08.selector = initial_state->value_08;
    field_04_08.offset = initial_state->value_04;
    value_0c = initial_state->value_0c;
    value_10_b = initial_state->value_10;
    value_14 = initial_state->value_14;
    value_18 = initial_state->value_18;

    slot = 6;
    index = 0;
    while (index < 5) {
        if (HMI_SYSTEM_U32(0x5b2 + index * 4) == 0) {
            slot = index;
            break;
        }
        ++index;
    }
    if (slot == 6)
        return 11;

    HMI_SYSTEM_U32(0x5b2 + slot * 4) = driver_id;
    if (initial_state->value_24 != 0 || initial_state->value_28 != 0) {
        field_24_28.selector = initial_state->value_28;
        field_24_28.offset = initial_state->value_24;
        field_2c_30.selector = initial_state->value_30;
        field_2c_30.offset = initial_state->value_2c;
    } else {
        status = sosDIGILoadDriver(driver_id, slot,
                           (void __far *)&field_2c_30,
                           (void __far *)&field_24_28,
                           HMI_SYSTEM_U32_PTR(0xcc, slot),
                           HMI_SYSTEM_U32_PTR(0x68, slot),
                           HMI_SYSTEM_U32_PTR(0x04, slot));
        if (status)
            return status;
        initial_state->value_28 = field_24_28.selector;
        initial_state->value_24 = field_24_28.offset;
        initial_state->value_30 = field_2c_30.selector;
        initial_state->value_2c = field_2c_30.offset;
    }

    *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x2c, slot)->selector_bytes = field_24_28.selector;
    *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x2c, slot)->offset_bytes = field_24_28.offset;
    *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x4a, slot)->selector_bytes = field_2c_30.selector;
    *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x4a, slot)->offset_bytes = field_2c_30.offset;

    status = sosDIGIGetDeviceCaps(driver_id, slot,
                       (void __far *)(HMI_SYSTEM_BASE + 0x5ca + slot * 0x6c));
    if (status)
        return status;

    if (HMI_SYSTEM_U32(0x5b2 + slot * 4) < 0xe106) {
        HMI_SYSTEM_U32(0x344 + slot * 4) = value_00;
        if (value_0c != 0) {
            HMI_SYSTEM_U32(0xe70 + slot * 4) = 1;
            dma_area = sosDIGIAllocateBuffer(value_00, &linear,
                                        HMI_SYSTEM_U32_PTR(0x18a, slot));
            if (dma_area == 0) {
                sosDIGIUnLoadDriver(slot);
                HMI_SYSTEM_U32(0x5b2 + slot * 4) = 0;
                return 5;
            }
            field_04_08.offset = FP_OFF(dma_area);
            field_04_08.selector = FP_SEG(dma_area);
            *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->selector_bytes = field_04_08.selector;
            *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->offset_bytes = field_04_08.offset;
            if (HMI_SYSTEM_U32(0x5ee + slot * 0x6c) == 8 &&
                (HMI_SYSTEM_U8(0x607 + slot * 0x6c) & 0x80) != 0) {
                for (fill_index = 0; fill_index < value_00; ++fill_index)
                    ((unsigned char __far *)dma_area)[fill_index] = 0x80;
            } else {
                for (fill_index = 0; fill_index < value_00; ++fill_index)
                    ((unsigned char __far *)dma_area)[fill_index] = 0;
            }
        } else {
            linear = initial_state->value_48;
            *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->selector_bytes = field_04_08.selector;
            *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->offset_bytes = field_04_08.offset;
        }
        initial_state->value_48 = linear;

        if (initial_state->value_34 == 0 && initial_state->value_38 == 0) {
            if (initial_state->value_44 != 0) {
                status = sosDIGILoadTimer(initial_state->value_44,
                                   (void __far *)&field_3c_40,
                                   (void __far *)&field_34_38,
                                   HMI_SYSTEM_U32_PTR(0xe0, slot),
                                   HMI_SYSTEM_U32_PTR(0x7c, slot),
                                   HMI_SYSTEM_U32_PTR(0x18, slot));
            } else {
                status = sosDIGILoadTimer(HMI_SYSTEM_U32(0x632 + slot * 0x6c),
                                   (void __far *)&field_3c_40,
                                   (void __far *)&field_34_38,
                                   HMI_SYSTEM_U32_PTR(0xe0, slot),
                                   HMI_SYSTEM_U32_PTR(0x7c, slot),
                                   HMI_SYSTEM_U32_PTR(0x18, slot));
            }
            if (status)
                return status;
        } else {
            field_34_38.selector = initial_state->value_38;
            field_34_38.offset = initial_state->value_34;
            field_3c_40.selector = initial_state->value_40;
            field_3c_40.offset = initial_state->value_3c;
        }

        if ((HMI_SYSTEM_U8(0x607 + slot * 0x6c) & 2) != 0)
            sosDRVSetupCallFunctions(MK_FP(field_3c_40.selector, field_3c_40.offset),
                      MK_FP(field_34_38.selector, field_34_38.offset),
                      MK_FP(field_2c_30.selector, field_2c_30.offset),
                      MK_FP(field_04_08.selector, field_04_08.offset));

        initial_state->value_38 = field_34_38.selector;
        initial_state->value_34 = field_34_38.offset;
        initial_state->value_40 = field_3c_40.selector;
        initial_state->value_3c = field_3c_40.offset;
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x90, slot)->selector_bytes = field_34_38.selector;
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x90, slot)->offset_bytes = field_34_38.offset;
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0xae, slot)->selector_bytes = field_3c_40.selector;
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0xae, slot)->offset_bytes = field_3c_40.offset;
        HMI_SYSTEM_U32(0x108 + slot * 4) = 1;

        cached_driver_value = HMI_SYSTEM_U32(0x852 + driver_record->wDMA * 4);
        sosDRVGetFillInfo(MK_FP(field_3c_40.selector, field_3c_40.offset),
                  MK_FP(field_34_38.selector, field_34_38.offset),
                  MK_FP(field_04_08.selector, field_04_08.offset),
                  value_00, cached_driver_value, slot,
                  (unsigned)(HMI_SYSTEM_BASE + 0xe98));

        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->selector_bytes = HMI_SYSTEM_U16(0xe9c);
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->offset_bytes = HMI_SYSTEM_U32(0xe98);
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x590, slot)->selector_bytes = HMI_SYSTEM_U16(0xea4);
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x590, slot)->offset_bytes = HMI_SYSTEM_U32(0xea0);
        sosFillSampleStructs((unsigned)&_lpSOSSampleList[slot][0],
                  MK_FP(HMI_SYSTEM_U16(0xeac), HMI_SYSTEM_U32(0xea8)));
    } else {
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->selector_bytes = 0;
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->offset_bytes = 0;
    }

    field_2c_30.offset = *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->offset_bytes;
    field_2c_30.selector = *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->selector_bytes;
    initial_state->value_1c = field_2c_30.offset;
    initial_state->value_20 = field_2c_30.selector;
    sosDIGIInitForWindows(driver_record->wDMA);
    HMI_SYSTEM_U32(0x176 + slot * 4) = initial_state->value_48;

    record_00 = driver_record->wPort;
    record_04 = driver_record->wIRQ;
    cached_driver_value = driver_record->wDMA;
    record_0c = driver_record->wParam;
    HMI_SYSTEM_U8(0x872 + slot) = (unsigned char)cached_driver_value;
    sosDRVInit(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset),
              record_00, record_04, cached_driver_value, value_14,
              value_18, record_0c);
    sosDRVSetAction(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset));
    sosDRVSetRate(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset), value_10_b);
    linear = HMI_SYSTEM_U32(0x176 + slot * 4);
    sosDRVStart(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset), linear, value_00);
    HMI_SYSTEM_U32(0xf4 + slot * 4) = 1;
    HMI_SYSTEM_U32(0x11c + slot * 4) = 1;

    HMI_SYSTEM_U32(0x877 + slot * 0x4c) = initial_state->value_00;
    HMI_SYSTEM_U16(0x87f + slot * 0x4c) = initial_state->value_08;
    HMI_SYSTEM_U32(0x87b + slot * 0x4c) = initial_state->value_04;
    HMI_SYSTEM_U32(0x883 + slot * 0x4c) = initial_state->value_0c;
    HMI_SYSTEM_U32(0x887 + slot * 0x4c) = value_10_a;
    HMI_SYSTEM_U32(0x88b + slot * 0x4c) = driver_id;
    HMI_SYSTEM_U32(0x88f + slot * 0x4c) = initial_state->value_18;
    HMI_SYSTEM_U16(0x897 + slot * 0x4c) = initial_state->value_20;
    HMI_SYSTEM_U32(0x893 + slot * 0x4c) = initial_state->value_1c;
    HMI_SYSTEM_U16(0x89f + slot * 0x4c) = initial_state->value_28;
    HMI_SYSTEM_U32(0x89b + slot * 0x4c) = initial_state->value_24;
    HMI_SYSTEM_U16(0x8af + slot * 0x4c) = initial_state->value_38;
    HMI_SYSTEM_U32(0x8ab + slot * 0x4c) = initial_state->value_34;
    HMI_SYSTEM_U16(0x8a7 + slot * 0x4c) = initial_state->value_30;
    HMI_SYSTEM_U32(0x8a3 + slot * 0x4c) = initial_state->value_2c;
    HMI_SYSTEM_U32(0x8b3 + slot * 0x4c) = initial_state->value_3c;
    HMI_SYSTEM_U16(0x8b7 + slot * 0x4c) = initial_state->value_40;
    HMI_SYSTEM_U32(0x8bb + slot * 0x4c) = initial_state->value_44;
    *slot_out = slot;
    return 0;
}

unsigned sosDIGIUnInitDriver(unsigned driver, unsigned enabled, unsigned extra_cleanup)
{
    void __far *first_address;
    void __far *second_address;
    void __far *previous_dma;

    if (HMI_SYSTEM_U32(0xf4 + driver * 4) == 0)
        return 1;

    HMI_SYSTEM_U32(0x11c + driver * 4) = 0;
    if (HMI_SYSTEM_U32(0x19e + driver * 4) < 0xe106) {
        if (HMI_SYSTEM_U32(0xe70 + driver * 4) != 0 && enabled != 0) {
            previous_dma = HMI_SYSTEM_FAR(0x158, driver);
            if ((sosDRVIsWindowsActive() & 0xff) == 3) {
                sosFreeVDSPage(FP_SEG(previous_dma),
                             HMI_SYSTEM_U32(0x18a + driver * 4),
                             HMI_SYSTEM_U32(0x176 + driver * 4));
            } else {
                sosRealFree(FP_SEG(previous_dma));
            }
            HMI_SYSTEM_U32(0xe70 + driver * 4) = 0;
        }
    }

    first_address = _lpSOSDriver[driver].pointer;
    second_address = _lpSOSDriverCS[driver].pointer;
    sosDRVStop(second_address, first_address);
    sosDRVUnInit(second_address, first_address);
    sosDIGIUnInitForWindows(HMI_SYSTEM_U8(0x872 + driver));
    if (extra_cleanup != 0) {
        sosDIGIUnLoadDriver(driver);
        sosDIGIUnLoadTimer(driver);
    }

    HMI_SYSTEM_FAR(0x2c, driver) = 0;
    HMI_SYSTEM_FAR(0x4a, driver) = 0;
    HMI_SYSTEM_FAR(0x1b2, driver) = 0;
    HMI_SYSTEM_FAR(0x90, driver) = 0;
    HMI_SYSTEM_FAR(0xae, driver) = 0;
    HMI_SYSTEM_U32(0x5b2 + driver * 4) = 0;
    HMI_SYSTEM_U32(0xf4 + driver * 4) = 0;
    return 0;
}

unsigned sosAllocVDSPage(hmi_far_address *address, unsigned *selector_out,
                          unsigned *linear_out);

unsigned long __far *sosDIGIAllocateBuffer(unsigned size, unsigned *linear,
                                      unsigned *selector_out)
{
    unsigned base;
    unsigned selector;
    unsigned output_selector;
    unsigned output_linear;
    hmi_far_address address;

    selector = 0;
    if ((sosDRVIsWindowsActive() & 0xff) == 3) {
        if (sosAllocVDSPage(&address, &output_selector, &output_linear))
            return 0;
        *linear = output_linear;
        *selector_out = output_selector;
        return MK_FP(address.selector, address.offset);
    }
    sosRealAlloc((size >> 4) + 4, &base, &selector);
    *linear = base;
    while ((base & 0xffff) > (0xffff - size)) {
        sosRealAlloc((size >> 4) + 4, &base, &selector);
        *linear = base;
    }
    return MK_FP((unsigned short)selector, 0);
}

unsigned sosAllocVDSPage(hmi_far_address *address, unsigned *selector_out,
                         unsigned *linear_out)
{
    unsigned base;
    unsigned selector;
    unsigned status;
    unsigned temporary;
    unsigned result;
    unsigned __far *memory;

    status = 0;
    temporary = 0;
    address->selector = 0;
    address->offset = 0;
    *selector_out = 0;
    *linear_out = 0;
    if (sosDRVAllocVDSStruct(0x20, &selector, &base))
        return 0;
    memory = (unsigned __far *)MK_FP((unsigned short)selector, 0);
    *memory = 0x1000;
    status = sosDRVVDSGetBuffer(base);
    sub_79824((void __far *)&_sSOSVDSInfo, memory, 0x10);
    sosDRVFreeVDSStruct(base, selector);
    if (status)
        return 5;
    _sSOSVDSInfo.dos_physical.words.low &= 0xf000;
    temporary = _sSOSVDSInfo.dos_physical.physical;
    result = sosDRVMakeDMASelector(temporary);
    address->offset = 0;
    address->selector = (unsigned short)result;
    *linear_out = _sSOSVDSInfo.dos_physical.physical;
    *selector_out = result;
    return 0;
}

void sosFreeVDSPage(unsigned driver, unsigned unused, unsigned unused2)
{
    unsigned __far *memory;
    unsigned base;
    unsigned selector;

    sosDRVFreeDMASelector(driver);
    sosDRVAllocVDSStruct(0x20, &base, &selector);
    memory = (unsigned __far *)MK_FP((unsigned short)base, 0);
    sub_79824(memory, (void __far *)&_sSOSVDSInfo, 0x10);
    sosDRVVDSFreeBuffer(selector);
    sosDRVFreeVDSStruct(base, selector);
}

void sosDIGIInit_End(void) {}
