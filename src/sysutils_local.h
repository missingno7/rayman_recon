/* DOS allocator table: stride 12, capacity +0, data +4, allocated +8.
 * Android temp_mem has size 360 (30 entries). */
#pragma pack(push,4)
typedef struct temp_mem_entry_t {
    u32 capacity;
    u8 *data;
    u8 allocated;
} temp_mem_entry_t;
#pragma pack(pop)
/* DPMI 0500h writes a 48-byte memory information record. */
typedef struct dpmi_memory_info_t {
    u32 largest_free_block;
    u32 remaining_info[11];
} dpmi_memory_info_t;
