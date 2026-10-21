/* sound.c (1:61b80..1:633e0: sound events, voices, volume/pan, music). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker w2_dispsave; measured status in NOTES.md. sub_2748 pragma declares an assembly routine's register ABI only. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "sound_types.h"
#include <stdlib.h>

typedef u8 bool;

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define Abs(x) ((x) >= 0 ? (x) : -(x))

extern u8 CarteSonAutorisee;
extern s16 Volume_Snd;
extern u8 voice_is_working[32];
extern s16 was_in_freeze;
extern s16 stk_obj[20];
extern s16 stk_snd[20];
extern s16 pt_pile_snd;
extern s16 indice_snd_wiz;
extern s16 indice_ray_wait;
extern s16 indice_trz_wait;
extern pile_snd_t pile_snd[10];
extern voice_t voice_table[32];
extern u8 sound_table[256];
extern u8 frame_snd[256];
extern u8 frame_snd_bis[256];
extern u16 snd_bis[256];
extern sound_table_entry_t hard_sound_table[256];
extern u8 bank_to_use[256];
extern u8 not_snd_wiz[8];
extern s16 num_level_choice;
extern s16 num_world_choice;
extern s16 num_level;
extern s16 num_world;
extern s16 rayman_obj_id;
extern s16 reduced_rayman_id;
extern u8 dead_time;
extern u8 dans_la_map_monde;
extern s32 map_time;
extern obj_t ray;
extern level_t level;
extern options_jeu_t options_jeu;
extern u8 MusicCdActive;
extern u8 mapCd[128];
extern s32 timeCd[6][22];
extern s32 cdTime;
extern s32 cdTimeStart;
extern s32 cdTimeEnd;
extern u8 flagCDPlay;
extern u8 phaseCd;

void GET_ANIM_POS(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
void KeyOff(s16 voice_id, u8 bank, u8 prog, u8 tone, u8 note);
void KeyVol(s16 voice_id, s16 volume, s16 pan);
s16 KeyOn(u8 bank, s32 prog, s16 tone, s16 note, s16 volume, s16 pan);
void PlayTchatch(s32 snd, s32 volume);
void start_cd_bbdead(void);
u8 sub_19790(void);
u8 AudioStatusByte(void);
u8 sub_1a360(s32 track);
void sub_1a6bc(void);
void sub_1a3b0(s32 track);
void sub_1a384(s32 track);
void sub_19768(void);
void sub_2748(s32 *out);
void AudioTrackCommand(u8 track);
void sub_1a6c4(void);
void sub_1a654(u8 track);
/* ESI parameter ABI, also used by the STRICT effect1 callers. */
#pragma aux sub_2748 parm [esi]

u8 snd_sqrt_table[128] = {
    40,46,51,55,60,64,67,70,72,74,77,80,83,86,89,91,
    93,95,96,98,101,104,107,109,111,113,115,117,120,122,124,125,
    127,129,131,133,135,137,138,140,142,144,145,147,149,150,152,153,
    155,156,158,160,161,163,164,166,167,168,170,171,173,174,176,177,
    178,180,181,182,184,185,186,187,189,190,191,193,194,195,196,198,
    199,200,201,202,203,205,206,207,208,210,211,212,213,214,215,217,
    218,219,220,222,223,224,225,226,227,229,230,231,232,233,234,235,
    236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251
};

void manage_snd_event(void) {
    //nullsub
}

void SetVolumeSound(s16 volume) {
    Volume_Snd = volume;
}

/* Android order and one-byte bodies identify the two empty mode hooks.
 * Watcom merges their aligned return at DOS 1:61c0c. */
void set_stereo(void) {}
void set_mono(void) {}

void stop_all_snd(void) {
    s16 i;
    if (CarteSonAutorisee) {
        for (i = 0; i <= 32; ++i)
            KeyOff(i, 0, 0, 0, 0);
    }
}

void stop_ray_snd(void) {
    s16 i;
    if (CarteSonAutorisee) {
        for (i = 0; i < 32; ++i) {
            if (voice_table[i].snd == 6 || voice_table[i].snd == 245 || voice_table[i].snd == 2) {
                KeyVol(i, 0, 0);
                voice_table[i].obj = -2;
                voice_table[i].snd = -1;
            }
        }
    }
}

u8 get_pan_snd(obj_t *obj) {
    s16 screen_x;
    s16 res;
    screen_x = obj->offset_bx + obj->screen_x;
    if (screen_x < -96)
        res = 0;
    else if (screen_x > 416)
        res = 127;
    else
        res = (screen_x + 96) >> 2;
    if (res > 127)
        res = 127;
    return res;
}

s16 get_vol_snd(obj_t *obj) {
    s16 h, x, w, y;
    s16 diff_x, diff_y;
    s16 res;
    GET_ANIM_POS(obj, &x, &y, &w, &h);
    diff_x = abs((x + (w >> 1)) - (ray.x + ray.offset_bx));
    diff_y = abs((y + (h >> 1)) - ((ray.y + ray.offset_by) - 40));
    res = 127 - ((s16)diff_x + (s16)diff_y >> 2);
    if (res < 0)
        res = 0;
    return res;
}

void raj_env_sound(s16 volume) {
    SetVolumeSound((volume * 127) / 20);
}

void raj_env_audio(s16 enabled) {
    //nullsub
}

s16 raj_env_stereo(s16 stereo) {
    if (stereo == 0) return 0;
    return stereo;
}

void InitSnd(void) {
    s32 i;
    if (CarteSonAutorisee) {
        for (i = 0; i < 20; ++i) {
            stk_snd[i] = 0;
            stk_obj[i] = -2;
        }
        pt_pile_snd = 0;
        indice_snd_wiz = 0;
        indice_ray_wait = 0;
        indice_trz_wait = 2;
        for (i = 0; i < 32; ++i)
            voice_table[i].obj = -2;
        SetVolumeSound(options_jeu.sound_volume * 127 / 20);
        raj_env_stereo(options_jeu.is_stereo);
    }
    return;
}

/* Android EndSound is empty and follows InitSnd; DOS shared RET 1:61e40. */
void EndSound(void) {}

s16 last_snd(s16 obj_id) {
    s16 i;
    s16 res;
    i = 0;
    do {
        if (stk_obj[i] == obj_id)
            break;
        ++i;
    } while (i != 20);
    if (i == 20) {
        do {
            --i;
        } while (i != -1 && stk_snd[i] != 0);
        if (i == -1)
            i = 0;
    }
    if (i == -1)
        res = -1;
    else
        res = stk_snd[i];
    return res;
}

s16 get_pile_obj(s16 obj_id) {
    s16 index;
    pile_snd_t *cur_pile_snd;
    index = -1;
    if (pt_pile_snd != 0) {
        cur_pile_snd = pile_snd;
        index = 0;
        while (cur_pile_snd->obj != obj_id && index < pt_pile_snd) {
            ++index;
            ++cur_pile_snd;
        }
        if (index == pt_pile_snd)
            index = -1;
    }
    return index;
}

s16 get_voice_obj(s16 obj_id) {
    s16 i;
    voice_t *voice;
    i = 0;
    voice = voice_table;
    if (voice->obj != obj_id) {
        do {
            if (i >= 32)
                break;
            ++i;
            ++voice;
        } while (voice->obj != obj_id);
    }
    if (i == 32)
        return -1;
    return i;
}

s16 get_voice_snd(s16 snd) {
    s16 i;
    s16 *cur_snd;
    i = 0;
    cur_snd = &voice_table[0].snd;
    if (*cur_snd != snd) {
        do {
            if (i >= 32)
                break;
            ++i;
            cur_snd += 6;
        } while (*cur_snd != snd);
    }
    if (i == 32)
        return -1;
    return i;
}

s16 get_voice_obj_snd(s16 obj_id, s16 snd) {
    s16 index;
    s16 *cur_obj;
    s16 *cur_snd;
    index = 0;
    cur_snd = &voice_table[0].snd;
    cur_obj = &voice_table[0].obj;
    if (snd == 48) {
        do {
            if (*cur_snd == snd && *cur_obj == obj_id)
                break;
            if (index >= 32)
                break;
            ++index;
            cur_snd += 6;
            cur_obj += 6;
        } while (1);
    } else {
        do {
            if (*cur_snd == snd || *cur_obj == obj_id)
                break;
            if (index >= 32)
                break;
            ++index;
            cur_snd += 6;
            cur_obj += 6;
        } while (1);
    }
    if (index == 32)
        return -1;
    return index;
}

void erase_pile_snd(s16 obj_id) {
    s16 i;
    s16 pile_obj_id;
    s32 next;
    pile_obj_id = get_pile_obj(obj_id);
    if (pile_obj_id != -1) {
        for (i = pile_obj_id; i < pt_pile_snd; ++i) {
            next = i + 1;
            pile_snd[i].obj = pile_snd[next].obj;
            pile_snd[i].snd = pile_snd[next].snd;
            pile_snd[i].prog = pile_snd[next].prog;
            pile_snd[i].tone = pile_snd[next].tone;
            pile_snd[i].note = pile_snd[next].note;
            pile_snd[i].vol = pile_snd[next].vol;
            pile_snd[i].field_C = pile_snd[next].field_C;
            pile_snd[i].pan = pile_snd[next].pan;
            pile_snd[i].end_time = pile_snd[next].end_time;
            pile_snd[i].field_14 = pile_snd[next].field_14;
        }
        if (pt_pile_snd > 0) {
            --pt_pile_snd;
        }
    }
}

void nettoie_pile_snd(void) {
    s16 index = 0;
    s16 i;
    s32 next;
    while (index < pt_pile_snd) {
        if ((u32)pile_snd[index].end_time < (u32)map_time && pile_snd[index].end_time != 0) {
            // erase entry
            for (i = index; i < pt_pile_snd; ++i) {
                next = i + 1;
                pile_snd[i].obj = pile_snd[next].obj;
                pile_snd[i].snd = pile_snd[next].snd;
                pile_snd[i].prog = pile_snd[next].prog;
                pile_snd[i].tone = pile_snd[next].tone;
                pile_snd[i].note = pile_snd[next].note;
                pile_snd[i].vol = pile_snd[next].vol;
                pile_snd[i].field_C = pile_snd[next].field_C;
                pile_snd[i].pan = pile_snd[next].pan;
                pile_snd[i].end_time = pile_snd[next].end_time;
                pile_snd[i].field_14 = pile_snd[next].field_14;
            }
            if (pt_pile_snd > 0) {
                --pt_pile_snd;
            }
        } else {
            ++index;
        }
    }
}

void erase_voice_table(s16 obj_id) {
    s16 i;
    voice_t *voice;
    i = 0;
    voice = voice_table;
    if (voice->obj != obj_id) {
        do {
            if (i >= 32)
                break;
            ++i;
            ++voice;
        } while (voice->obj != obj_id);
    }
    if (i != 32)
        voice->obj = -2;
}

u8 snd_in_pile_snd(s16 snd) {
    s16 index = 0;
    s16 *cur_snd;
    if (pt_pile_snd != 0) {
        cur_snd = &pile_snd[0].snd;
        while (*cur_snd != snd && index < pt_pile_snd) {
            ++index;
            cur_snd += 12;
        }
        if (index == pt_pile_snd)
            index = 0;
        else
            index = 1;
    }
    return index;
}

s32 vol_r(s16 a1, s16 a2) {
    s32 result = (a1 * snd_sqrt_table[a2]) >> 8;
    return result;
}

s32 vol_l(s16 a1, s16 a2) {
    s32 result = (a1 * snd_sqrt_table[127 - a2]) >> 8;
    return result;
}

void PlaySnd(s16 snd, s16 obj_id) {
    s16 sound_to_interrupt;
    u8 pan_snd, vol_snd;
    s16 prog;
    s16 tone, note;
    s32 voll, volr;
    s16 i;
    if (CarteSonAutorisee) {
        if ((ray.scale != 0 && obj_id == reduced_rayman_id) || obj_id == rayman_obj_id) {
            obj_id = -1;
        }
        if (obj_id == -1 && snd != 15) {
            indice_ray_wait = 0;
        }
        sound_to_interrupt = last_snd(obj_id);
        if (snd != sound_to_interrupt && (sound_table[sound_to_interrupt] & 8) != 0) {
            s16 voice_id;
            erase_pile_snd(obj_id);
            voice_id = get_voice_obj_snd(obj_id, sound_to_interrupt);
            if (voice_id >= 0) {
                voice_t *voice;
                sound_table_entry_t *sound;
                voice = voice_table + voice_id;
                sound = hard_sound_table + voice->snd;
                KeyOff(voice_id, bank_to_use[voice->snd], sound->prog, sound->tone, sound->note);
                voice->obj = -2;
                voice->snd = -1;
                voice_is_working[voice_id] = 0;
            }
        }

        if (!dans_la_map_monde && snd != 0 && (snd != sound_to_interrupt || (sound_table[sound_to_interrupt] & 4) != 0)) {
            if (obj_id == -1 || (obj_id == rayman_obj_id && rayman_obj_id != -1)) {
                pan_snd = get_pan_snd(&ray);
                vol_snd = 127;
            } else {
                pan_snd = get_pan_snd(level.objects + obj_id);
                if (sound_table[snd] & 1) {
                    vol_snd = get_vol_snd(level.objects + obj_id);
                } else {
                    vol_snd = 127;
                }
            }

            prog = hard_sound_table[0].prog;
            tone = hard_sound_table[0].tone;

            switch(snd) {
                case 53:
                    if (level.objects[obj_id].type != 238) {
                        prog = hard_sound_table[53].prog;
                        tone = hard_sound_table[53].tone;
                        note = hard_sound_table[53].note;
                    } else {
                        note = hard_sound_table[0].note;
                    }
                    break;
                case 47:
                    if (num_world != 5) {
                        prog = hard_sound_table[47].prog;
                        tone = hard_sound_table[47].tone;
                        note = hard_sound_table[47].note;
                        vol_snd = 127;
                    } else {
                        prog = hard_sound_table[170].prog;
                        tone = hard_sound_table[170].tone;
                        note = hard_sound_table[170].note;
                        vol_snd = 127;
                    }
                    break;
                case 15:
                    if (indice_ray_wait++ > 2) {
                        prog = hard_sound_table[15].prog;
                        tone = hard_sound_table[15].tone;
                        note = hard_sound_table[15].note;
                        indice_ray_wait = 0;
                    }
                    erase_pile_snd(-1);
                    break;
                case 80:
                    if (indice_trz_wait++ > 1) {
                        prog = hard_sound_table[80].prog;
                        tone = hard_sound_table[80].tone;
                        note = hard_sound_table[80].note;
                        indice_trz_wait = 0;
                    }
                    break;
                case 14:
                    prog = hard_sound_table[14].prog;
                    tone = hard_sound_table[14].tone;
                    note = hard_sound_table[14].note + not_snd_wiz[level.objects[obj_id].sub_etat - 24]; // ting note depends on the sub_etat
                    break;
                case 19:
                    prog = hard_sound_table[19].prog;
                    tone = hard_sound_table[19].tone;
                    note = hard_sound_table[19].note;
                    break;
                case 245:
                    prog = hard_sound_table[245].prog;
                    tone = hard_sound_table[245].tone;
                    note = hard_sound_table[245].note;
                    break;
                case 55:
                case 57:
                case 103:
                    prog = -1;
                    break;
                default:
                    prog = hard_sound_table[snd].prog;
                    tone = hard_sound_table[snd].tone;
                    note = hard_sound_table[snd].note;
                    break;
            }

            if (prog != -1 && prog != 255) {
                erase_pile_snd(obj_id);
                voll = vol_l(Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd); // NOTE: has no effect?
                volr = vol_r(Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd); // NOTE: has no effect?

                if (frame_snd[snd] == 0) {
                    // NOTE: the PS1 version implements left/right directional sound here, using SsUtKeyOn
                    s16 voice_id;
                    voice_id = KeyOn(bank_to_use[snd], prog, tone, note, Volume_Snd * vol_snd * hard_sound_table[snd].volume >> 14, pan_snd);
                    if (voice_id != -1) {
                        erase_voice_table(obj_id);
                        voice_table[voice_id].vol = vol_snd;
                        voice_table[voice_id].obj = obj_id;
                        voice_table[voice_id].pan = pan_snd;
                        voice_table[voice_id].snd = snd;
                        if ((sound_table[snd] & 0x10) != 0) {
                            voice_is_working[voice_id] = 1;
                        }
                    }
                } else {

                    nettoie_pile_snd();
                    erase_pile_snd(obj_id);
                    pile_snd[pt_pile_snd].obj = obj_id;
                    pile_snd[pt_pile_snd].snd = snd;
                    pile_snd[pt_pile_snd].prog = prog;
                    pile_snd[pt_pile_snd].tone = tone;
                    pile_snd[pt_pile_snd].note = note;
                    pile_snd[pt_pile_snd].vol = hard_sound_table[snd].volume;
                    pile_snd[pt_pile_snd].field_C = vol_snd;
                    pile_snd[pt_pile_snd].pan = pan_snd;
                    pile_snd[pt_pile_snd].end_time = frame_snd[snd] + map_time;
                    if (snd_bis[snd] != 0) {
                        pile_snd[pt_pile_snd].field_14 = 1;
                    } else {
                        pile_snd[pt_pile_snd].field_14 = 0;
                    }
                    if (pt_pile_snd < 9) {
                        ++pt_pile_snd;
                    }
                }
            }
            if ((snd == 203 || snd == 237 || snd == 209 || snd == 225 || snd == 236) && (dead_time == 64 || dead_time == 128)) {
                start_cd_bbdead();
            }
        }
        i = 0;
        do {
            if (stk_obj[i] == obj_id || stk_obj[i] == -2)
                break;
            ++i;
        } while (i != 20);
        if (i == 20) {
            if (snd == 0)
                return;
            do {
                --i;
            } while (i != -1 && level.objects[stk_obj[i]].is_active);
            if (i == -1)
                i = 0;
        }
        stk_obj[i] = obj_id;
        stk_snd[i] = snd;
    }
}

void PlaySnd_old(s16 sound_id) {
    s16 voice_index = -1;
    if (CarteSonAutorisee) {
        s32 prog = hard_sound_table[sound_id].prog;
        if (prog != -1 && hard_sound_table[sound_id].prog != 255) {
            voice_index = KeyOn(bank_to_use[sound_id], prog,
                                hard_sound_table[sound_id].tone,
                                hard_sound_table[sound_id].note,
                                Volume_Snd * hard_sound_table[sound_id].volume >> 7, 64);
            if (voice_index != -1) {
                voice_table[voice_index].obj = -2;
                voice_table[voice_index].vol = 64;
                voice_table[voice_index].pan = 64;
                if (sound_table[sound_id] & 0x10) {
                    voice_is_working[voice_index] = 1;
                }
            }
        }
    }
}

void setvol(s16 obj_id, u8 unused) {
    s16 unk_pan;
    u8 unk_vol;
    s16 unk_pile;
    s32 unk_map_time;
    s16 voice_ind;
    s16 cur_pile;
    s32 next_pile;
    s32 vol_both;

    if (!CarteSonAutorisee)
        return;
    if ((ray.scale != 0 && obj_id == reduced_rayman_id) || obj_id == rayman_obj_id)
        obj_id = -1;

    unk_pile = get_pile_obj(obj_id);
    if (unk_pile != -1) {
        if (obj_id == -1 || (obj_id == rayman_obj_id && rayman_obj_id != -1)) {
            unk_vol = 127;
        } else {
            if (!(sound_table[pile_snd[unk_pile].snd] & 1) || !(sound_table[pile_snd[unk_pile].snd] & 2))
                unk_vol = 127;
            else
                unk_vol = get_vol_snd(level.objects + obj_id);
        }

        pile_snd[unk_pile].field_C = unk_vol;
        unk_map_time = map_time;
        if (pile_snd[unk_pile].end_time == unk_map_time) {
            voice_ind = KeyOn(bank_to_use[pile_snd[unk_pile].snd],
                              pile_snd[unk_pile].prog, pile_snd[unk_pile].tone,
                              pile_snd[unk_pile].note,
                              (Volume_Snd * unk_vol * pile_snd[unk_pile].vol) >> 14,
                              pile_snd[unk_pile].pan);
            if (voice_ind != -1) {
                erase_voice_table(obj_id);
                voice_table[voice_ind].obj = obj_id;
                voice_table[voice_ind].vol = unk_vol;
                voice_table[voice_ind].pan = pile_snd[unk_pile].pan;
                voice_table[voice_ind].snd = pile_snd[unk_pile].snd;
                if (sound_table[pile_snd[unk_pile].snd] & 0x10)
                    voice_is_working[voice_ind] = 1;
            }

            if (pile_snd[unk_pile].field_14 != 0) {
                pile_snd[unk_pile].end_time = unk_map_time + frame_snd_bis[pile_snd[unk_pile].snd];
                pile_snd[unk_pile].snd = snd_bis[pile_snd[unk_pile].snd];
                pile_snd[unk_pile].prog = hard_sound_table[pile_snd[unk_pile].snd].prog;
                pile_snd[pt_pile_snd].tone = hard_sound_table[pile_snd[unk_pile].snd].tone;
                pile_snd[pt_pile_snd].note = hard_sound_table[pile_snd[unk_pile].snd].note;
                pile_snd[pt_pile_snd].vol = hard_sound_table[pile_snd[unk_pile].snd].volume;
                pile_snd[unk_pile].field_14 = 0;
            } else {
                for (cur_pile = unk_pile; cur_pile < pt_pile_snd; ++cur_pile) {
                    next_pile = cur_pile + 1;
                    pile_snd[cur_pile].obj = pile_snd[next_pile].obj;
                    pile_snd[cur_pile].snd = pile_snd[next_pile].snd;
                    pile_snd[cur_pile].prog = pile_snd[next_pile].prog;
                    pile_snd[cur_pile].tone = pile_snd[next_pile].tone;
                    pile_snd[cur_pile].note = pile_snd[next_pile].note;
                    pile_snd[cur_pile].vol = pile_snd[next_pile].vol;
                    pile_snd[cur_pile].field_C = pile_snd[next_pile].field_C;
                    pile_snd[cur_pile].pan = pile_snd[next_pile].pan;
                    pile_snd[cur_pile].end_time = pile_snd[next_pile].end_time;
                    pile_snd[cur_pile].field_14 = pile_snd[next_pile].field_14;
                }
                if (pt_pile_snd > 0)
                    --pt_pile_snd;
            }
        }
    }

    voice_ind = get_voice_obj(obj_id);
    if (voice_ind != -1) {
        if (obj_id == -1 || (obj_id == rayman_obj_id && rayman_obj_id != -1)) {
            unk_vol = 127;
        } else {
            if (voice_table[voice_ind].snd == 204 ||
                !(sound_table[voice_table[voice_ind].snd] & 1) ||
                !(sound_table[voice_table[voice_ind].snd] & 2))
                unk_vol = 127;
            else
                unk_vol = get_vol_snd(level.objects + obj_id);
        }
        unk_pan = voice_table[voice_ind].pan;
        voice_table[voice_ind].vol = unk_vol;
        if (voice_is_working[voice_ind]) {
            vol_both = (Volume_Snd * unk_vol * hard_sound_table[voice_table[voice_ind].snd].volume) >> 14;
            KeyVol(voice_ind, vol_both, unk_pan);
        }
    }
}

void setpan(s16 obj_id, u8 unused) {
    s16 voice_ind;
    s16 pile_ind;
    u8 pan;
    if (!CarteSonAutorisee)
        return;
    if (ray.scale != 0 && obj_id == reduced_rayman_id)
        obj_id = -1;
    voice_ind = get_voice_obj(obj_id);
    if (voice_ind != -1) {
        if (obj_id == -1)
            pan = get_pan_snd(&ray);
        else
            pan = get_pan_snd(level.objects + obj_id);
        voice_table[voice_ind].pan = pan;
    } else {
        if (obj_id == -1)
            pan = get_pan_snd(&ray);
        else
            pan = get_pan_snd(level.objects + obj_id);
        pile_ind = get_pile_obj(obj_id);
        if (pile_ind != -1)
            pile_snd[pile_ind].pan = pan;
    }
}

void manage_snd(void) {
    s32 i;
    s32 stk_index;
    s16 *cur_obj;
    s16 *cur_snd;
    if (CarteSonAutorisee) {
        for (cur_snd = &voice_table[0].snd, i = 0, cur_obj = &voice_table[0].obj;
             i < 32; ++i, cur_snd += 6, cur_obj += 6) {
            if ((*cur_obj >= 0 && !level.objects[*cur_obj].is_active) ||
                (*cur_obj == -2 && *cur_snd != -1) ||
                *cur_obj >= level.nb_objects) {
                if (sound_table[voice_table[i].snd] & 0x10) {
                    KeyOff(i, bank_to_use[*cur_snd],
                           hard_sound_table[*cur_snd].prog,
                           hard_sound_table[*cur_snd].tone,
                           hard_sound_table[*cur_snd].note);
                    voice_is_working[i] = 0;
                    stk_index = 0;
                    while (stk_index != 20 && stk_obj[stk_index] != *cur_obj)
                        ++stk_index;
                    if (stk_index < 20)
                        stk_snd[stk_index] = -1;
                }
                *cur_obj = -2;
                *cur_snd = -1;
            }
            if (*cur_obj >= 0 && level.objects[*cur_obj].is_active) {
                setvol(*cur_obj, 0);
                setpan(*cur_obj, 0);
            }
        }
        for (i = 0; i < pt_pile_snd; ++i) {
            if (pile_snd[i].obj < level.nb_objects && level.objects[pile_snd[i].obj].is_active) {
                setvol(pile_snd[i].obj, 0);
                setpan(pile_snd[i].obj, 0);
            }
        }
    }
}

void mute_snd_bouclant(void) {
    s16 i;
    if (CarteSonAutorisee) {
        for (i = 0; i < 32; ++i) {
            if (voice_table[i].snd == 6 || voice_table[i].snd == 245 || voice_table[i].snd == 2 ||
                (sound_table[voice_table[i].snd] & 0x10) != 0)
                KeyVol(i, 0, 0);
        }
    }
}

void mute_snd(void) {
    s16 i;
    if (CarteSonAutorisee) {
        for (i = 0; i < 32; ++i)
            KeyVol(i, 0, 0);
    }
}

void start_freeze_snd(void) {
    mute_snd();
    was_in_freeze = 1;
}

void stop_freeze_snd(void) {
    was_in_freeze = 0;
}

void PlayTchatchVignette(s32 snd) {
    if (CarteSonAutorisee) {
        if (snd == 0) {
            switch(num_world_choice) {
                case 1: {
                    snd = 1;
                } break;
                case 3: {
                    snd = 2;
                } break;
                case 4: {
                    snd = 4;
                } break;
                case 5: {
                    if (num_level_choice != 3) {
                        return;
                    }
                    snd = 3;
                } break;
                case 6: {
                    snd = 7;
                } break;
                default: break;
            }
        }
        PlayTchatch(snd, Volume_Snd); //TODO
    }
}

void TestCdLoop(void) {
    if (cdTime == 0) {
        sub_2748(&cdTimeStart);
        AudioTrackCommand(((u8 *)mapCd - 1)[22 * (num_world - 1) + num_level]);
        phaseCd = 0;
        cdTime = 1;
    } else if (cdTime != -1) {
        sub_2748(&cdTimeEnd);
        cdTime = (10 * (u32)(cdTimeEnd - cdTimeStart)) / 182;
        cdTime += 2;
        if ((u32)cdTime < (u32)timeCd[num_world - 1][num_level - 1])
            return;
        if (flagCDPlay == 0) {
            ++flagCDPlay;
            sub_1a6c4();
            return;
        }
        if (flagCDPlay < 4) {
            ++flagCDPlay;
            return;
        }
        sub_2748(&cdTimeStart);
        sub_1a654(((u8 *)mapCd - 1)[22 * (num_world - 1) + num_level]);
        phaseCd = !phaseCd;
        flagCDPlay = 0;
        cdTime = 1;
    }
}

void InitMusic(void) {
    if (sub_19790()) {
        if (sub_1a360(AudioStatusByte())) {
            sub_1a6bc();
            sub_1a3b0(AudioStatusByte());
        } else {
            MusicCdActive = 0;
        }
    } else {
        MusicCdActive = 0;
    }
}

void DoneMusic(void) {
    sub_1a6bc();
    sub_1a384(AudioStatusByte());
    sub_19768();
}

