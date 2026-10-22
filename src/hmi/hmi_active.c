/* INVENTED names. DOS-derived HMI active-voice module, 1:71afa..71b83.
 * Compile separately from the game: Watcom -od -s -3r.
 * Empty begin/end routines recur around HMI modules; retain their real extents. */
typedef struct hmi_voice_state {
    unsigned char unknown_00[0x30];
    short flags;
} hmi_voice_state;
extern hmi_voice_state __far *hmi_voice_slots[][32];

void hmi_active_begin(void) {}
unsigned sub_71B0F(unsigned driver)
{
    unsigned voice;
    unsigned count;
    count = 0;
    for (voice = 0; voice < 32; voice++) {
        if (hmi_voice_slots[driver][voice]->flags & 0x8000)
            count++;
    }
    return count;
}
void hmi_active_end(void) {}
