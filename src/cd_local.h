#ifndef W2_GAP2_CD_LOCAL_H
#define W2_GAP2_CD_LOCAL_H
#include "rtypes.h"
typedef struct {
    u8 track_count;
    u8 first_track;
    u8 last_track;
    u32 leadout_lba;
    u32 track_start[100];
    u32 leadout;
    u32 track_end[100];
    u32 track_length[100];
    u8 track_flags[100];
} dos_cd_disc_info_t;
#endif
