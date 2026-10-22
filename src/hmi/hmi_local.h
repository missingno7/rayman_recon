/* INVENTED field names. Only DOS-observed fields are identified. */
typedef struct hmi_voice_state {
    unsigned char unknown_00[0x30];
    short flags;
} hmi_voice_state;
extern hmi_voice_state __far *hmi_voice_slots[][32];
