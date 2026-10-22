/* Structural BSS correction, GOG v1.21 2:32190..321c0 (48 bytes).
 * mem_32190 is the existing INVENTED binding; Android original: DPMIMemoryInfo.
 * DOS get_avail_memory 1:6f3d8..6f403 passes DS:32190 to INT 31h AX=0500h
 * via int386x. The API writes a 48-byte record, and the caller reads its +0
 * dword. Android 1.txt and 2.txt independently give DPMIMemoryInfo .bss size48.
 * Same type as the existing extern in src/sysutils.c / sysutils_local.h.
 * This replaces the 4-byte fragment; the following init_data_state is separate.
 */
#include "rtypes.h"

typedef struct dpmi_memory_info_t {
    u32 largest_free_block;
    u32 remaining_info[11]; /* eight further DPMI fields, then three reserved */
} dpmi_memory_info_t;

dpmi_memory_info_t mem_32190;
