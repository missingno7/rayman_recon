/* objinit.c (1:4e160..1:50350). Names RAYVERSE; seeded by tools/seed.py, worker tu_objupd3; finishing pass d_objinit. */
#include "obj.h"
#include "game.h"
extern obj_type_flags_t flags[262];
#include "map.h"
#include <string.h>

#define TYPE_108_DARK2_PINK_FLY 108
#define TYPE_10_FISH 10
#define TYPE_115_BB1_PLAT 115
#define TYPE_130_BLACKTOON_EYES 130
#define TYPE_136_DEMI_RAYMAN 136
#define TYPE_139_MARACAS_BAS 139
#define TYPE_13_BALLE1 13
#define TYPE_145_KILLING_EYES 145
#define TYPE_15_BALLE2 15
#define TYPE_163_INDICATOR 163
#define TYPE_165_BADGUY3 165
#define TYPE_16_FALLPLAT 16
#define TYPE_179_HERSE_BAS_NEXT 179
#define TYPE_17_LIFTPLAT 17
#define TYPE_186_POELLE_ALWAYS 186
#define TYPE_192_SMA_GRAND_LASER 192
#define TYPE_193_SMA_BOMB 193
#define TYPE_194_SMA_BOMB_CHIP 194
#define TYPE_196_DARD_PLAFOND 196
#define TYPE_202_RAY_ETOILES 202
#define TYPE_205_BLACK_FIST 205
#define TYPE_206_PIEDS_RAYMAN 206
#define TYPE_209_FIRE_LEFT 209
#define TYPE_210_FIRE_RIGHT 210
#define TYPE_215_DARK_SORT 215
#define TYPE_218_HYBRIDE_MOSAMS 218
#define TYPE_219_CORDE 219
#define TYPE_223_HYBRIDE_STOSKO 223
#define TYPE_231_HYB_BBF2_D 231
#define TYPE_232_HYB_BBF2_G 232
#define TYPE_233_HYB_BBF2_LAS 233
#define TYPE_23_RAYMAN 23
#define TYPE_242_HERSE_HAUT_NEXT 242
#define TYPE_244_SMA_PETIT_LASER 244
#define TYPE_24_INTERACTPLT 24
#define TYPE_257_BREAKOUT_GAME 257
#define TYPE_33_DARK2_SORT 33
#define TYPE_36_STONEBOMB 36
#define TYPE_37_TARZAN 37
#define TYPE_38_GRAINE 38
#define TYPE_39_NEN_GRAINE 39
#define TYPE_3_LIDOLPINK 3
#define TYPE_51_MST_FRUIT1 51
#define TYPE_52_MST_FRUIT2 52
#define TYPE_62_DROP 62
#define TYPE_64_SCROLL 64
#define TYPE_71_NOTE 71
#define TYPE_99_RAY_POS 99
#define flags0_1_always (1 << 0)
#define flags1_2_is_collectible (1 << 9)
#define flags1_8_ray_dist_multispr_cantchange (1 << 11)
#define world_1_jungle 1
#define world_3_mountain 3
#define world_4_image 4
#define world_5_cave 5
#ifndef COUNT
#define COUNT(x) (sizeof(x) / sizeof((x)[0]))
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 200
#endif


typedef unsigned char bool;
#define ANIMMODE_NONE 0
#define TYPE_117_CLOWN_TNT2 117
#define TYPE_118_CLOWN_TNT3 118
#define TYPE_144_LIDOLPINK2 144
#define TYPE_152_CAISSE_CLAIRE 152
#define TYPE_175_PUNAISE1 175
#define TYPE_176_CRAYON_BAS 176
#define TYPE_178_HERSE_BAS 178
#define TYPE_191_FALLING_CRAYON 191
#define TYPE_228_PRI 228
#define TYPE_229_PUNAISE2 229
#define TYPE_230_PUNAISE3 230
#define TYPE_235_PUNAISE4 235
#define TYPE_237_GOMME 237
#define TYPE_240_CRAYON_HAUT 240
#define TYPE_241_HERSE_HAUT 241
#define TYPE_243_MARK_AUTOJUMP_PLAT 243
#define TYPE_250_PUNAISE5 250
#define TYPE_25_INST_PLAT 25
#define TYPE_26_CRUMBLE_PLAT 26
#define TYPE_29_AUTOJUMP_PLAT 29
#define TYPE_34_MOVE_AUTOJUMP_PLAT 34
#define TYPE_53_MST_SHAKY_FRUIT 53
#define TYPE_0_BADGUY1 0
#define TYPE_100_MITE 100
#define TYPE_102_BNOTE 102
#define TYPE_109_PI 109
#define TYPE_110_PI_BOUM 110
#define TYPE_111_PI_MUS 111
#define TYPE_112_WASHING_MACHINE 112
#define TYPE_113_BAG1 113
#define TYPE_116_CLOWN_TNT 116
#define TYPE_119_TNT_BOMB 119
#define TYPE_11_BOUM 11
#define TYPE_120_BATTEUR_FOU 120
#define TYPE_121_ECLAIR 121
#define TYPE_122_STONEDOG2 122
#define TYPE_123_BLACKTOON1 123
#define TYPE_12_CHASSEUR1 12
#define TYPE_133_SUPERHELICO 133
#define TYPE_135_ETINC 135
#define TYPE_137_REDUCTEUR 137
#define TYPE_138_ROULETTE 138
#define TYPE_141_NEIGE 141
#define TYPE_142_ONEUP 142
#define TYPE_143_NOVA2 143
#define TYPE_146_FLASH 146
#define TYPE_147_MST_SCROLL 147
#define TYPE_148_GRAP_BONUS 148
#define TYPE_149_CLE_SOL 149
#define TYPE_14_CHASSEUR2 14
#define TYPE_150_SCORPION 150
#define TYPE_153_FEE 153
#define TYPE_154_ROULETTE2 154
#define TYPE_155_ROULETTE3 155
#define TYPE_157_EAU 157
#define TYPE_158_PALETTE_SWAPPER 158
#define TYPE_161_WIZ 161
#define TYPE_164_GENERATING_DOOR 164
#define TYPE_168_CYMBAL1 168
#define TYPE_169_CYMBAL2 169
#define TYPE_170_RAYON 170
#define TYPE_172_STONEWOMAN 172
#define TYPE_173_BATEAU 173
#define TYPE_174_PIRATE_POELLE 174
#define TYPE_180_SAXO2 180
#define TYPE_181_SCROLL_SAX 181
#define TYPE_183_SAXO3 183
#define TYPE_184_PIRATE_POELLE_D 184
#define TYPE_187_MAMA_PIRATE 187
#define TYPE_190_COUTEAU 190
#define TYPE_197_MEDAILLON_TOON 197
#define TYPE_198_BB12 198
#define TYPE_199_BB1_VIT 199
#define TYPE_19_SPLASH 19
#define TYPE_200_BB13 200
#define TYPE_201_BB14 201
#define TYPE_203_SMA_WEAPON 203
#define TYPE_204_BLACK_RAY 204
#define TYPE_20_GENEBADGUY 20
#define TYPE_211_BOUT_TOTEM 211
#define TYPE_212_DARK 212
#define TYPE_213_SPACE_MAMA2 213
#define TYPE_217_MITE2 217
#define TYPE_21_PHOTOGRAPHE 21
#define TYPE_220_PIERREACORDE 220
#define TYPE_221_CFUMEE 221
#define TYPE_224_STOSKO_PINCE 224
#define TYPE_225_PIRATE_P_45 225
#define TYPE_226_PIRATE_P_D_45 226
#define TYPE_227_MOSKITO2 227
#define TYPE_234_LAVE 234
#define TYPE_236_ANNULE_SORT_DARK 236
#define TYPE_238_POING_FEE 238
#define TYPE_239_PIRATE_GUETTEUR2 239
#define TYPE_245_DUNE 245
#define TYPE_246_CORDE_DARK 246
#define TYPE_248_SKO_PINCE 248
#define TYPE_249_RIDEAU 249
#define TYPE_251_VAGUE_DEVANT 251
#define TYPE_252_VAGUE_DERRIERE 252
#define TYPE_253_PLANCHES 253
#define TYPE_28_ONOFF_PLAT 28
#define TYPE_2_POWERUP 2
#define TYPE_30_AUDIOSTART 30
#define TYPE_31_ONEUP_ALWAYS 31
#define TYPE_32_DARK_PHASE2 32
#define TYPE_35_STONEMAN1 35
#define TYPE_40_STONEDOG 40
#define TYPE_41_OUYE 41
#define TYPE_42_SIGNPOST 42
#define TYPE_43_STONEMAN2 43
#define TYPE_44_CLASH 44
#define TYPE_45_MOVE_OUYE 45
#define TYPE_46_BB1 46
#define TYPE_48_FLAMME2 48
#define TYPE_4_NEUTRAL 4
#define TYPE_50_MOSKITO 50
#define TYPE_55_MUS_WAIT 55
#define TYPE_56_STONEWOMAN2 56
#define TYPE_57_STALAG 57
#define TYPE_58_CAGE 58
#define TYPE_59_CAGE2 59
#define TYPE_5_WIZARD1 5
#define TYPE_60_BIG_CLOWN 60
#define TYPE_61_WAT_CLOWN 61
#define TYPE_65_SPIDER 65
#define TYPE_66_DARD 66
#define TYPE_70_TROMPETTE 70
#define TYPE_72_PIRATE_NGAWE 72
#define TYPE_73_RING 73
#define TYPE_74_SAXO 74
#define TYPE_77_PIRATE_GUETTEUR 77
#define TYPE_78_PIRATE_BOMB 78
#define TYPE_79_STONECHIP 79
#define TYPE_7_MORNINGSTAR 7
#define TYPE_81_CYMBALE 81
#define TYPE_82_JAUGEUP 82
#define TYPE_83_EXPLOSION 83
#define TYPE_86_MARACAS 86
#define TYPE_88_TAMBOUR2 88
#define TYPE_90_NOTE0 90
#define TYPE_91_NOTE1 91
#define TYPE_92_NOTE2 92
#define TYPE_93_BONNE_NOTE 93
#define TYPE_94_POING 94
#define TYPE_95_POING_POWERUP 95
#define TYPE_96_TOTEM 96
#define TYPE_97_BBL 97
#define TYPE_98_SPACE_MAMA 98
#define TYPE_9_BADGUY2 9
extern s16 bateau_obj_id;
extern s16 num_level;
extern level_t level;
extern level_obj_t level_alw;
extern level_obj_t level_obj;

extern s32 Bloc_lim_H2;
extern s32 nb_wiz_save;
extern s16 cymbal_obj_id[2];
extern s16 scroll_obj_id[50];
extern obj_t ray;
extern obj_t* star_ray_dev;
extern obj_t* star_ray_der;
extern obj_t* poing_obj;
extern poing_t poing;
extern save_state_t save1;
extern status_bar_t status_bar;
extern s16 bonus_map;
extern s16 mama_pirate_obj_id;
extern s16 cb_ball_obj_id;
extern rayevts_t RayEvts;
extern finbosslevel_t finBosslevel;
extern s16 id_Cling_1up;
extern s16 id_Cling_Pow;
extern s16 mst_scroll_obj_id;
extern s16 NumScrollObj;
extern s16 sbar_obj_id;
extern s16 eau_obj_id;
extern s16 ymapmax;
extern s16 reduced_rayman_id;
extern s16 fee_obj_id;
extern s16 poing_obj_id;
extern s16 pierreAcorde_obj_id;
extern s16 rayman_obj_id;
extern s16 Mus_obj_id;
extern s16 xmap;
extern s16 black_ray_obj_id;
extern s16 black_fist_obj_id;
extern s16 moskitomama_gauche_obj_id;
extern s16 rideau_obj_id;
extern s16 corde_dark_obj_id;
extern s16 stosko_obj_id;
extern s16 moskitosaxo_obj_id;
extern s16 moskitomama_droite_obj_id;
extern u8 ray_on_poelle;
extern u8 nb_wiz;
extern u8 world_index;
extern u8 nb_cymbal_in_map;
extern u8 IsBossThere;
extern world_info_t t_world_info[24];
extern s16 zonediffx[262];

extern u16 type_zdc[262];
extern s16 prise_branchee;
extern s16 bossXToReach;
extern s16 bossYToReach;
extern s32 bossSpeedFactor;
extern s32 alternateBossSpeedFactor;
extern u8 laserSourceSprNumInAnim;
extern u8 scrollLocked;
extern u8 saveBossEncounter;
extern u8 currentBossAction;
extern u8 currentBossActionIsOver;
extern u8 bossEncounter;
extern s16 sko_phase;
extern s16 num_world;
s16 myRand(s16 max);
u8 bonus_taken(s16 id);
void calc_obj_pos(obj_t* obj);
void findMereDenisWeapon(void);
u8 Prio(obj_t* obj);
void first_obj_init(obj_t* obj);
void REINIT_OBJECT(obj_t* obj);
void obj_init(obj_t* obj);
void init_struct_level(void);
void INIT_OBJECTS(u8 first_init);
void special_flags_init(void);

u8 Prio(obj_t* obj) {

    switch (obj->type)
    {
        case TYPE_30_AUDIOSTART:
        case TYPE_98_SPACE_MAMA:
        case TYPE_204_BLACK_RAY:
        case TYPE_213_SPACE_MAMA2:
        case TYPE_236_ANNULE_SORT_DARK:
        case TYPE_238_POING_FEE:
        case TYPE_245_DUNE:
            // NOTE: these below are added in the PC version.
            // In the PS1 version, they are explicitly not drawn in DISPLAY_ALL_OBJECTS() instead
        case TYPE_141_NEIGE:
        case TYPE_158_PALETTE_SWAPPER:
        case TYPE_164_GENERATING_DOOR:
        case TYPE_181_SCROLL_SAX:
        case TYPE_199_BB1_VIT:
            return 0;
        case TYPE_149_CLE_SOL: // Changed from 2 to 1 in the PC version
        case TYPE_157_EAU:
        case TYPE_197_MEDAILLON_TOON:
            return 1;
        case TYPE_147_MST_SCROLL:
            if (obj->hit_points == 0)
                return 2;
            return 0;
        case TYPE_11_BOUM:
        case TYPE_19_SPLASH:
        case TYPE_21_PHOTOGRAPHE:
        case TYPE_41_OUYE:
        case TYPE_45_MOVE_OUYE:
        case TYPE_48_FLAMME2:
        case TYPE_57_STALAG:
        case 75: /* DOS: PAILLETTE priority 2. */
        case TYPE_78_PIRATE_BOMB: // Changed from 3 to 2 in the PC version
        case TYPE_79_STONECHIP:
        case TYPE_83_EXPLOSION:
        case TYPE_90_NOTE0:
        case TYPE_91_NOTE1:
        case TYPE_92_NOTE2:
        case TYPE_93_BONNE_NOTE:
        case TYPE_94_POING: // Added in PC version
        case TYPE_102_BNOTE:
        case TYPE_110_PI_BOUM:
        case TYPE_121_ECLAIR:
        case TYPE_135_ETINC:
        case TYPE_143_NOVA2:
        case TYPE_146_FLASH:
        case TYPE_150_SCORPION:
        case TYPE_161_WIZ:
        case TYPE_168_CYMBAL1:
        case TYPE_170_RAYON:
        case TYPE_220_PIERREACORDE:
        case TYPE_221_CFUMEE:
        case TYPE_224_STOSKO_PINCE:
        case TYPE_234_LAVE:
        case TYPE_248_SKO_PINCE:
            return 2;
        case TYPE_123_BLACKTOON1:
            if (num_world == 1 && num_level == 14)
                return 3;
            return 2; // Changed from always 2 in the PC version
        case TYPE_28_ONOFF_PLAT:
        case TYPE_44_CLASH:
        case TYPE_46_BB1:
        case TYPE_58_CAGE: // Changed from 2 to 3 in the PC version
        case TYPE_59_CAGE2: // Changed from 2 to 3 in the PC version
        case TYPE_66_DARD:
        case TYPE_72_PIRATE_NGAWE:
        case TYPE_73_RING:
        case TYPE_74_SAXO:
        case TYPE_77_PIRATE_GUETTEUR:
        case TYPE_86_MARACAS:
        case TYPE_97_BBL:
        case TYPE_119_TNT_BOMB:
        case TYPE_133_SUPERHELICO:
        case TYPE_138_ROULETTE:
        case TYPE_154_ROULETTE2:
        case TYPE_155_ROULETTE3:
        case TYPE_180_SAXO2:
        case TYPE_183_SAXO3:
        case TYPE_187_MAMA_PIRATE:
        case TYPE_190_COUTEAU:
        case TYPE_198_BB12:
        case TYPE_200_BB13:
        case TYPE_201_BB14:
        case TYPE_203_SMA_WEAPON:
        case TYPE_211_BOUT_TOTEM:
        case TYPE_239_PIRATE_GUETTEUR2:
        case TYPE_249_RIDEAU:
            return 3;
        case TYPE_253_PLANCHES:
            /* 1:4e612: load [bateau_obj_id - 2], SAR 16 (-5 short load). */
            return (bateau_obj_id != -1) ? 4 : 7;
        case TYPE_7_MORNINGSTAR:
        case TYPE_20_GENEBADGUY:
        case TYPE_96_TOTEM: // Added in PC version
        case TYPE_109_PI:
        case TYPE_111_PI_MUS:
        case TYPE_112_WASHING_MACHINE:
        case TYPE_246_CORDE_DARK: // Added in PC version
        case TYPE_251_VAGUE_DEVANT:
            return 5;
        case TYPE_2_POWERUP:
        case TYPE_31_ONEUP_ALWAYS: // Added in PC version
        case TYPE_55_MUS_WAIT:
        case TYPE_82_JAUGEUP:
        case TYPE_95_POING_POWERUP:
        case TYPE_137_REDUCTEUR:
        case TYPE_142_ONEUP:
        case TYPE_148_GRAP_BONUS:
        case TYPE_173_BATEAU:
            return 6;
        case TYPE_4_NEUTRAL:
        case TYPE_42_SIGNPOST:
        case TYPE_88_TAMBOUR2:
        case TYPE_252_VAGUE_DERRIERE:
            return 7;
        /* DOS range 192..194: 1:4e2d6..4e2e7. */
        case TYPE_192_SMA_GRAND_LASER:
        case TYPE_193_SMA_BOMB:
        case TYPE_194_SMA_BOMB_CHIP:
            return 4;
        /* Explicit dispatch boundaries: 1:4e177 and 1:4e1f1. */
        case TYPE_196_DARD_PLAFOND:
        case TYPE_244_SMA_PETIT_LASER:
            break;
        default:
            break;
    }
    return 4;
}

void first_obj_init(obj_t* obj) {
    if ((flags[obj->type].raw & 1) != 0 || ((obj->type == TYPE_0_BADGUY1 || obj->type == TYPE_9_BADGUY2 ) && obj->x < 0)) {
        obj->x = -32000;
        obj->y = -32000;
    }
    obj->scale = 0;
    obj->init_x = obj->x;
    obj->init_y = obj->y;
    obj->init_main_etat = obj->main_etat;
    obj->init_sub_etat = obj->sub_etat;
    obj->init_hit_points = obj->hit_points;
    obj->active_flag = 1;

    switch (obj->type) {
        case 98:
        case 213:
            findMereDenisWeapon();
            obj->link = 0;
            break;
        case 153:
            obj->detect_zone = 200;
            obj->link = 0;
            break;
        case 70:
        case 217:
            obj->detect_zone = 160;
            obj->link = 0;
            break;
        case 50:
        case 227:
        case 5:
        case 65:
            obj->detect_zone = 60;
            obj->link = 0;
            break;
        case 12:
        case 14:
        case 20:
        case 183:
            obj->detect_zone = 180;
            obj->link = 0;
            break;
        case 172:
        case 56:
        case 60:
        case 100:
            obj->detect_zone = 220;
            obj->link = 0;
            break;
        case 35:
        case 40:
        case 43:
        case 55:
        case 72:
        case 120:
        case 122:
            obj->detect_zone = 100;
            obj->link = 0;
            break;
        case 174:
        case 225:
            obj->detect_zone = 150;
            obj->link = 0;
            break;
        case 184:
        case 226:
            obj->detect_zone = 50;
            obj->link = 0;
            break;
        case 116:
            obj->detect_zone = 200;
            obj->link = 0;
            break;
        case 57:
            obj->detect_zone = 250;
            obj->link = 0;
            break;
        case 81:
        case 168:
        case 169:
            obj->detect_zone = 0;
            obj->link = 0;
            break;
        case 113:
            obj->param = 0;
            obj->link = 0;
            break;
        case 150:
            obj->detect_zone = 160;
            obj->link = 0;
            break;
        case 61:
        case 187:
        case 32:
        case 212:
            obj->detect_zone = 150;
            obj->link = 0;
            break;
        default:
            obj->detect_zone = 40;
            obj->link = 0;
            break;
    }
}

/* Pass4 local names (INVENTED): 1:4e9bc; Restore an instance from its captured reset state, reset common runtime members, then apply type-specific defaults.
   Evidence/widths: docs/semantics/objinit.json; defaults docs/port/OBJECT_DEFAULTS.md. */
void obj_init(obj_t* obj) {
    s16 unk_1;

    obj->nb_cmd = 0;
    obj->is_active = 0;
    obj->display_prio = Prio(obj);
    obj->zdc = type_zdc[obj->type];
    if (obj->type != TYPE_163_INDICATOR) {
        obj->main_etat = obj->init_main_etat;
        obj->sub_etat = obj->init_sub_etat;
    }
    obj->anim_frame = 0;
    obj->speed_x = 0;
    obj->speed_y = 0;
    obj->x = obj->init_x;
    obj->y = obj->init_y;
    obj->anim_index = obj->eta[obj->main_etat][obj->sub_etat].anim_index;
    obj->change_anim_mode = ANIMMODE_NONE;
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
    obj->iframes_timer = -1;
    obj->follow_id = -1;
    obj->detect_zone_flag = 0;
    obj->cmd_offset = -1;
    obj->flags.flip_x = 0;
    obj->flags.read_commands = (obj->cmds != NULL);
    memset(obj->btypes, 0, 5);
    GET_OBJ_CMD(obj);
    obj->test_block_index = 0;
    obj->cmd_context_depth = -1;
    obj->active_flag = 1; //ACTIVE_DEAD;
    obj->hit_points = obj->init_hit_points;
    obj->timer = 0;
    obj->config = 0;
    obj->flags.anim_changed = 0;
    obj->flags.flag_0x40 = 0;
    obj->flags.flag_0x100 = 0;

    switch (obj->type)
    {
        case TYPE_74_SAXO:
        case TYPE_180_SAXO2:
        case TYPE_183_SAXO3:
            INIT_SAXO(obj);
            break;
        case TYPE_228_PRI:
            if (finBosslevel.helped_joe_2)
            {
                prise_branchee = 1;
                set_main_and_sub_etat(obj, 0, 2);
                obj->init_sub_etat = 2;
                skipToLabel(obj, 1, 1);
                break;
            }
            break;
        case TYPE_251_VAGUE_DEVANT:
        case TYPE_252_VAGUE_DERRIERE:
            obj->x = obj->init_x = -15;
            break;
        case TYPE_223_HYBRIDE_STOSKO:
            break; /* DOS dispatch 1:4eafb..4eb05 explicitly tests this no-op case. */
        case TYPE_218_HYBRIDE_MOSAMS:
            bossXToReach = obj->x;
            bossYToReach = firstFloorBelow(&ray) - obj->offset_by + 16; // TODO
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            break;
        case TYPE_176_CRAYON_BAS:
        case TYPE_240_CRAYON_HAUT:
            {
                s16 frame_count = obj->animations[obj->anim_index].frames_count;
                obj->anim_frame = myRand(frame_count - 1);
            }
            break;
        case TYPE_191_FALLING_CRAYON:
        case TYPE_235_PUNAISE4:
            obj->anim_frame = obj->hit_points;
            break;
        case TYPE_175_PUNAISE1:
        case TYPE_229_PUNAISE2:
        case TYPE_230_PUNAISE3:
        case TYPE_250_PUNAISE5:
            obj->flags.flip_x = (obj->hit_points & 1);
            break;
        case TYPE_51_MST_FRUIT1:
        case TYPE_53_MST_SHAKY_FRUIT:
            obj->gravity_value_2 = 5;
            break;
        case TYPE_52_MST_FRUIT2:
            obj->gravity_value_2 = 10;
            break;
        case TYPE_98_SPACE_MAMA:
            obj->type = TYPE_213_SPACE_MAMA2;
        case TYPE_213_SPACE_MAMA2:
            swapMereDenisCollZones(obj, 1); // TODO
            setBossScrollLimits(obj); // TODO
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            laserSourceSprNumInAnim = -1;
            IsBossThere = 0;
            scrollLocked = 0;
            obj->timer = 180;
            saveBossEncounter = 255;
            currentBossAction = 0;
            currentBossActionIsOver = 0;
            bossEncounter = 0;
            break;
        case TYPE_227_MOSKITO2:
            bossEncounter = 2;
            goto init_moskito;
        case TYPE_50_MOSKITO:
            bossEncounter = 0;
        init_moskito:
            obj->scale = 255;
            setBossScrollLimits(obj);
            bossSpeedFactor = 0x4000;
            alternateBossSpeedFactor = 0;
            IsBossThere = 0;
            scrollLocked = 0;
            obj->timer = 100;
            saveBossEncounter = 255;
            currentBossAction = 0;
            currentBossActionIsOver = 0;
            break;
        case TYPE_0_BADGUY1:
            obj->offset_hy = 30;
            obj->flags.follow_enabled = 0;
            break;
        case TYPE_144_LIDOLPINK2:
            obj->iframes_timer = 0;
            break;
        case TYPE_123_BLACKTOON1:
            switch (obj->follow_sprite)
            {
                case 1:
                    obj->config = 2;
                    obj->detect_zone = 0;
                    break;
                case 2:
                    obj->config = 8;
                    obj->detect_zone = 60;
                    break;
                case 3:
                    obj->config = 24;
                    obj->detect_zone = 170;
                    break;
                case 4:
                    obj->config = 10;
                    obj->detect_zone = 100;
                    break;
                case 5:
                    obj->follow_id = 0;
                    obj->config = 1;
                    obj->detect_zone = 0;
                    break;
                case 6:
                    obj->config = 4;
                    obj->detect_zone = 60;
                    break;
                case 7:
                    obj->config = 36;
                    obj->detect_zone = 120;
                    break;
            }
            break;
        case TYPE_145_KILLING_EYES:
            obj->param = 40;
            obj->iframes_timer = 40;
            break;
        case TYPE_72_PIRATE_NGAWE:
            obj->config = 1;
            break;
        case TYPE_77_PIRATE_GUETTEUR:
        case TYPE_239_PIRATE_GUETTEUR2:
            swapGuetteurCollZones(obj, obj->init_sub_etat == 14); //TODO
            obj->iframes_timer = 0;
            obj->detect_zone = 80;
            break;
        case TYPE_28_ONOFF_PLAT:
            obj->iframes_timer = obj->follow_id = 100;
            obj->flags.alive = 1;
            break;
        case TYPE_26_CRUMBLE_PLAT:
            if (num_world != 1) {
                s16 animation_end_delay = (s16)(vblToEOA(obj, 1) - 2);
                obj->param = animation_end_delay;
                obj->iframes_timer = animation_end_delay;
            } else {
                obj->param = 40;
                obj->iframes_timer = 20;
            }
            obj->link = 20;
            obj->flags.alive = 1;
            obj->flags.follow_enabled = 1;
            break;
        case TYPE_25_INST_PLAT:
            obj->iframes_timer = obj->follow_id = 15;
            obj->flags.alive = 1;
            break;
        case TYPE_29_AUTOJUMP_PLAT:
        case TYPE_34_MOVE_AUTOJUMP_PLAT:
        case TYPE_152_CAISSE_CLAIRE:
        case TYPE_237_GOMME:
        case TYPE_243_MARK_AUTOJUMP_PLAT:
            obj->iframes_timer = obj->follow_id = 1;
            obj->flags.alive = 1;
            break;
        case TYPE_116_CLOWN_TNT:
        case TYPE_117_CLOWN_TNT2:
        case TYPE_118_CLOWN_TNT3:
            obj->config = 0; // this line is missing in the Android version
            obj->link = 0;
            break;
        case TYPE_168_CYMBAL1:
        case TYPE_169_CYMBAL2:
        case TYPE_178_HERSE_BAS:
        case TYPE_241_HERSE_HAUT:
            obj->link = 0;
            break;
        case TYPE_150_SCORPION:
            obj->detect_zone = 160;
            sko_phase = 0;
            break;
        case TYPE_21_PHOTOGRAPHE:
        case TYPE_7_MORNINGSTAR:
            obj->flags.alive = 1;
            break;
        case 75:
        case TYPE_170_RAYON:
        case TYPE_197_MEDAILLON_TOON:
            obj->flags.alive = 0;
            obj->is_active = 0;
            break;
        case TYPE_184_PIRATE_POELLE_D:
        case TYPE_226_PIRATE_P_D_45:
            obj->flags.flip_x = 1;
            break;
        case TYPE_212_DARK:
            obj->param = 0;
            break;
        case TYPE_217_MITE2:
            obj->eta[1][0].flags &= ~0x10;
            obj->eta[1][2].flags &= ~0x10;
            obj->follow_id = 0;
            break;
        case TYPE_202_RAY_ETOILES:
            if (obj->main_etat == 0 && obj->sub_etat == 57)
                star_ray_der = obj;
            else
                star_ray_dev = obj;
            obj->display_prio = 3;
            obj->flags.alive = 0;
            obj->is_active = 0;
            break;
    }
}

/* Pass4 local names (INVENTED): 1:4f1e8; Build always-object and ordinary object-index lists after initialization.
   Evidence/widths: docs/semantics/objinit.json; defaults docs/port/OBJECT_DEFAULTS.md. */
void init_struct_level(void) {
    s16 i;
    obj_t *obj;
    s16 object_count;
    level_alw.nb_objects = 0;
    level_obj.nb_objects = 0;
    for (i = 0; i < level.nb_objects; ++i) {
        level_alw.obj_ids[i] = 0;
        level_obj.obj_ids[i] = 0;
    }
    i = 0;
    obj = &level.objects[i];
    object_count = level.nb_objects;
    while (i < object_count) {
        if (bonus_map || (flags[obj->type].flags1 & 2) == 0 || !bonus_taken(obj->id)) {
            if (flags[obj->type].flags0 & 1) {
                level_alw.obj_ids[level_alw.nb_objects++] = i;
            } else {
                level_obj.obj_ids[level_obj.nb_objects++] = i;
            }
        }
        ++i;
        ++obj;
    }
}

/* Pass4 local names (INVENTED): 1:4f2d4; Initialize all level instances and discover special-object IDs and lists.
   Evidence/widths: docs/semantics/objinit.json; defaults docs/port/OBJECT_DEFAULTS.md. */
void INIT_OBJECTS(u8 first_init) {
    s16 found_bbf2_droite;
    s16 found_bbf2_gauche;
    s16 i;
    obj_t *obj;
    s16 object_count;
    found_bbf2_droite = -1;
    found_bbf2_gauche = -1;
    ray_on_poelle = 0;
    nb_cymbal_in_map = 0;
    IsBossThere = 0;
    Mus_obj_id = found_bbf2_droite;
    NumScrollObj = 0;
    rayman_obj_id = found_bbf2_droite;
    reduced_rayman_id = found_bbf2_droite;
    id_Cling_1up = found_bbf2_droite;
    id_Cling_Pow = found_bbf2_droite;
    fee_obj_id = found_bbf2_droite;
    eau_obj_id = found_bbf2_droite;
    mst_scroll_obj_id = found_bbf2_droite;
    bateau_obj_id = found_bbf2_droite;
    mama_pirate_obj_id = found_bbf2_droite;
    black_ray_obj_id = found_bbf2_droite;
    pierreAcorde_obj_id = found_bbf2_droite;
    rideau_obj_id = found_bbf2_droite;
    cb_ball_obj_id = found_bbf2_droite;
    stosko_obj_id = found_bbf2_droite;
    moskitomama_droite_obj_id = found_bbf2_droite;
    moskitomama_gauche_obj_id = found_bbf2_droite;
    moskitosaxo_obj_id = found_bbf2_droite;
    corde_dark_obj_id = found_bbf2_droite;
    nb_wiz = 0;

    i = 0;
    obj = &level.objects[i];
    object_count = level.nb_objects;
    while (i < object_count) {
        obj->id = i;
        if (first_init) {
            first_obj_init(obj);
        }
        obj->active_timer = 0;
        if (obj->type == TYPE_163_INDICATOR) {
            obj->main_etat = obj->init_main_etat;
            obj->sub_etat = obj->init_sub_etat;
        }
        obj_init(obj);
        if (flags[obj->type].raw & flags0_1_always) {
            obj->x = -32000;
            obj->y = -32000;
            obj->flags.alive = 0;
        } else {
            obj->flags.alive = 1;
        }

        switch(obj->type) {
            case TYPE_37_TARZAN: {
                obj->flags.alive = !RayEvts.magicseed;
            } break;
            case TYPE_249_RIDEAU: {
                if (obj->init_sub_etat == 2) {
                    rideau_obj_id = obj->id;
                }
            } break;
            case TYPE_133_SUPERHELICO: {
                if (!RayEvts.super_helico) {
                    obj->flags.alive = finBosslevel.helped_musician;
                } else {
                    obj->flags.alive = 0;
                }
            } break;
            case TYPE_257_BREAKOUT_GAME: {
                cb_ball_obj_id = obj->id;
            } break;
            case TYPE_96_TOTEM: {
                obj->param = 0;
                obj->iframes_timer = 0;
            } break;
            case TYPE_246_CORDE_DARK: {
                corde_dark_obj_id = obj->id;
            } break;
            case TYPE_223_HYBRIDE_STOSKO: {
                obj->param = 0;
                stosko_obj_id = obj->id;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_218_HYBRIDE_MOSAMS: {
                obj->param = 0;
                moskitosaxo_obj_id = obj->id;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_231_HYB_BBF2_D: {
                obj->param = 0;
                moskitomama_droite_obj_id = obj->id;
                obj->flags.alive = 0;
                obj->is_active = 0;
                found_bbf2_droite = obj->id;
            } break;
            case TYPE_232_HYB_BBF2_G: {
                moskitomama_gauche_obj_id = obj->id;
                obj->param = 0;
                found_bbf2_gauche = obj->id;
                obj->flags.flip_x = 1;
                obj->flags.alive = 0;
                obj->is_active = 0;
            } break;
            case TYPE_220_PIERREACORDE: {
                pierreAcorde_obj_id = i;
                obj->iframes_timer = 0;
            } break;
            case TYPE_219_CORDE: {
                obj->hit_points = obj->init_hit_points >> 2;
                if (pierreAcorde_obj_id != -1) {
                    if (obj->x < level.objects[pierreAcorde_obj_id].x) {
                        obj->y = level.objects[pierreAcorde_obj_id].y + 112;
                    } else {
                        obj->y = level.objects[pierreAcorde_obj_id].y + 120;
                    }
                    obj->y = obj->init_y;
                }
            } break;
            case TYPE_179_HERSE_BAS_NEXT:
            case TYPE_242_HERSE_HAUT_NEXT: {
                if (obj->field_1C == 0) {
                    obj->flags.alive = 0;
                    obj->is_active = 0;
                }
            } break;
            case TYPE_157_EAU: {
                if (obj->hit_points != 0) {
                    obj->x = xmap - obj->offset_bx;
                    if (num_world == world_1_jungle) {
                        if (RayEvts.magicseed) {
                            obj->hit_points = 1;
                        }
                        obj->y = ymapmax + SCREEN_HEIGHT;
                    } else {
                        obj->param = obj->x;
                        obj->hit_points = 0;
                        if (num_world != world_3_mountain || num_level != 7) {
                            obj->y = ymapmax + (SCREEN_HEIGHT - 80);
                            obj->init_y = ymapmax + (SCREEN_HEIGHT - 80);
                            if (num_world == world_5_cave && num_level == 8) {
                                obj->y = ymapmax + Bloc_lim_H2 - 80;
                                obj->init_y = ymapmax + Bloc_lim_H2 - 80;
                            } else if (num_world == world_4_image) {
                                obj->y = ymapmax + (SCREEN_HEIGHT - 70);
                                obj->init_y = ymapmax + (SCREEN_HEIGHT - 70);
                            }
                        } else {
                            obj->sub_speed = 0;
                        }
                    }
                    obj->is_active = 1;
                    eau_obj_id = i;
                    obj->flags.alive = 1;
                    obj->iframes_timer = 0;
                    calc_obj_pos(obj);
                } else {
                    if (eau_obj_id != -1 && num_world != world_1_jungle) {
                        obj->flags.flip_x = 0;
                        obj->speed_y = 0;
                        obj->speed_x = 0;
                        obj->x = (level.objects[eau_obj_id].param += 101);
                        obj->y = level.objects[eau_obj_id].y;
                        if (num_world != world_3_mountain || num_level != 7) {
                            obj->init_y = ymapmax + (SCREEN_HEIGHT - 80);
                            if (num_world == world_4_image) {
                                obj->init_y = ymapmax + (SCREEN_HEIGHT - 70);
                            }
                            if (num_world == world_5_cave && num_level == 8) {
                                obj->y = eau_obj_id + level.objects[eau_obj_id].y - obj->id;
                                obj->init_y = obj->y;
                            }
                        } else {
                            obj->init_y = obj->y;
                            obj->sub_speed = 0;
                        }
                        calc_obj_pos(obj);
                        obj->is_active = 1;
                        obj->flags.alive = 1;
                        obj->sub_etat = ++level.objects[eau_obj_id].hit_points;
                        if (obj->sub_etat > 3) {
                            obj->sub_etat -= 4;
                        }
                        obj->iframes_timer = 0;
                    } else {
                        obj->flags.alive = 0;
                        obj->is_active = 0;
                    }
                }
            } break;
            case TYPE_153_FEE: {
                fee_obj_id = i;
            } break;
            case TYPE_147_MST_SCROLL: {
                if (obj->hit_points == 0) {
                    obj->init_x = ray.x - 200;
                    obj->x = obj->init_x;
                    obj->y = ray.y - 50;
                    set_main_and_sub_etat(obj, 0, 9);
                    obj->speed_x = 2;
                    obj->speed_y = 0;
                    obj->is_active = 1;
                    obj->param = 0;
                    obj->link = 0;
                    obj->timer = 0;
                    mst_scroll_obj_id = i;
                    obj->flags.flip_x = 1;
                }
            } break;
            case TYPE_136_DEMI_RAYMAN: {
                obj->flags.alive = 0;
                obj->is_active = 0;
                reduced_rayman_id = i;
            } break;
            case TYPE_99_RAY_POS: {
                if (save1.is_just_saved && !bonus_map) {
                    obj->flags.alive = 0;
                }
                switch (obj->main_etat) {
                    case 0:
                        obj->timer = 0;
                        obj->is_active = 0;
                        break;
                    case 5:
                        obj->timer = 30;
                        break;
                }
            } break;
            case TYPE_64_SCROLL: {
                scroll_obj_id[NumScrollObj] = i;
                obj->y = obj->init_y;
                NumScrollObj++;
                obj->x = obj->init_x;
                if (NumScrollObj >= 50) {
                    FatalError("Augmenter MAX_NB_SCROLL_PER_MAP");
                }
                obj->flags.alive = 0;
            } break;
            case TYPE_19_SPLASH:
                obj->flags.alive = 0;
                break;
            case TYPE_9_BADGUY2:
            case TYPE_165_BADGUY3: {
                obj->eta[2][2].sound_index = 0;
            } break;
            case TYPE_81_CYMBALE: {
                cymbal_obj_id[nb_cymbal_in_map++] = i;
            } break;
            case TYPE_23_RAYMAN: {
                obj->flags.alive = 0;
                rayman_obj_id = i;
            } break;
            case TYPE_205_BLACK_FIST: {
                obj->flags.alive = 0;
                black_fist_obj_id = obj->id;
            } break;
            case TYPE_94_POING: {
                obj->init_sub_etat = poing.sub_etat;
                poing_obj_id = i;
                obj->sub_etat = poing.sub_etat;
                poing_obj = &level.objects[poing_obj_id];
                obj->init_flag = 7;
                obj->flags.alive = 0;
            } break;
            case TYPE_11_BOUM: {
                sbar_obj_id = i;
                obj->init_flag = 7;
                obj->flags.alive = 0;
            } break;
            case TYPE_145_KILLING_EYES: {
                obj->iframes_timer = 40;
            } break;
            case TYPE_39_NEN_GRAINE: {
                obj->iframes_timer = 0;
            } /* falls through to clear alive */
            case TYPE_203_SMA_WEAPON:
            case TYPE_224_STOSKO_PINCE:
            case TYPE_238_POING_FEE:
            case TYPE_38_GRAINE: {
                obj->flags.alive = 0;
            } break;
            case TYPE_65_SPIDER: {
                obj->link = 0;
            } break;
            case TYPE_13_BALLE1:
            case TYPE_15_BALLE2:
            case TYPE_33_DARK2_SORT:
            case TYPE_36_STONEBOMB:
            case TYPE_44_CLASH:
            case TYPE_51_MST_FRUIT1:
            case TYPE_52_MST_FRUIT2:
            case TYPE_59_CAGE2:
            case TYPE_62_DROP:
            case TYPE_66_DARD:
            case TYPE_71_NOTE:
            case TYPE_73_RING:
            case TYPE_78_PIRATE_BOMB:
            case TYPE_79_STONECHIP:
            case TYPE_83_EXPLOSION:
            case TYPE_90_NOTE0:
            case TYPE_91_NOTE1:
            case TYPE_92_NOTE2:
            case TYPE_93_BONNE_NOTE:
            case TYPE_97_BBL:
            case TYPE_102_BNOTE:
            case TYPE_108_DARK2_PINK_FLY:
            case TYPE_110_PI_BOUM:
            case TYPE_112_WASHING_MACHINE:
            case TYPE_115_BB1_PLAT:
            case TYPE_119_TNT_BOMB:
            case TYPE_121_ECLAIR:
            case TYPE_122_STONEDOG2:
            case TYPE_130_BLACKTOON_EYES:
            case TYPE_135_ETINC:
            case TYPE_139_MARACAS_BAS:
            case TYPE_143_NOVA2:
            case TYPE_146_FLASH:
            case TYPE_186_POELLE_ALWAYS:
            case TYPE_192_SMA_GRAND_LASER:
            case TYPE_193_SMA_BOMB:
            case TYPE_194_SMA_BOMB_CHIP:
            case TYPE_196_DARD_PLAFOND:
            case TYPE_197_MEDAILLON_TOON:
            case TYPE_206_PIEDS_RAYMAN:
            case TYPE_209_FIRE_LEFT:
            case TYPE_210_FIRE_RIGHT:
            case TYPE_215_DARK_SORT:
            case TYPE_233_HYB_BBF2_LAS:
            case TYPE_244_SMA_PETIT_LASER: {
                obj->is_active = 0;
                obj->flags.alive = 0;
                obj->init_flag = 7;
            } break;
            case TYPE_3_LIDOLPINK: {
                obj->init_flag = 7;
                obj->flags.alive = 0;
            } break;
            case TYPE_20_GENEBADGUY: {
                obj->init_flag = 7;
                obj->timer = 0;
            } break;
            case TYPE_204_BLACK_RAY: {
                obj->is_active = 0;
                obj->flags.alive = 0;
                obj->init_flag = 7;
                obj->animations = ray.animations;
                black_ray_obj_id = obj->id;
            } break;
            case TYPE_190_COUTEAU: {
                obj->follow_id = 1;
                obj->is_active = 0;
                obj->init_flag = 7;
                obj->flags.alive = 0;
                obj->iframes_timer = obj->follow_id;
            } break;
            case TYPE_10_FISH: {
                if (i >= 1) {
                    obj_t* prev_obj = &level.objects[i - 1];
                    obj->flags.alive = 1;
                    if (prev_obj->type == TYPE_10_FISH && prev_obj->x == obj->x) {
                        obj->flags.alive = 0;
                        obj->is_active = 0;
                    }
                }
            } break;
            case TYPE_46_BB1:
            case TYPE_198_BB12:
            case TYPE_200_BB13: {
                INIT_BBMONT(obj);
            } break;
            case TYPE_111_PI_MUS: {
                if (finBosslevel.helped_musician) {
                    set_sub_etat(obj, 10);
                    obj->init_sub_etat = 10;
                }
            } /* falls through to init_flag */
            case TYPE_82_JAUGEUP:
            case TYPE_109_PI:
            case TYPE_142_ONEUP:
            case TYPE_148_GRAP_BONUS: {
                obj->init_flag = 7;
            } break;
            case TYPE_55_MUS_WAIT: {
                obj->init_flag = 7;
                Mus_obj_id = obj->id;
                if (finBosslevel.helped_musician) {
                    obj->init_sub_etat = 4;
                    set_main_and_sub_etat(obj, 0, 4);
                }
            } break;
            case TYPE_31_ONEUP_ALWAYS: {
                obj->flags.alive = 0;
                obj->init_flag = 7;
            } break;
            case TYPE_173_BATEAU: {
                obj->param = -1;
                obj->link = 0;
                bateau_obj_id = i;
                obj->timer = 0;
            } break;
            case TYPE_21_PHOTOGRAPHE: {
                obj->timer = 0;
            } break;
            case TYPE_187_MAMA_PIRATE: {
                mama_pirate_obj_id = i;
            } break;
            case TYPE_32_DARK_PHASE2: {
                obj->flags.alive = 0;
                obj->x = obj->init_x = 0;
                obj->y = obj->init_y = 0;
                obj->is_active = 0;
            } break;
        }

        // Take away any leftover cages if nb_cages has been set to 6 (e.g. through a cheat?)
        if (t_world_info[world_index].nb_cages == 6 && obj->type == TYPE_58_CAGE) {
            obj->flags.alive = 0;
            obj->is_active = 0;
            take_bonus(obj->id);
        }

        // Disable bonuses (collectibles) that were already taken
        if (!bonus_map && (flags[obj->type].raw & flags1_2_is_collectible) && bonus_taken(obj->id)) {
            obj->flags.alive = 0;
            obj->is_active = 0;
        }

        // Count the tings in bonus stages
        if (bonus_map && obj->type == TYPE_161_WIZ) {
            ++nb_wiz;
        }
        ++obj;
        ++i;
    }

    // Link moskitomama objects to each other
    if (found_bbf2_droite != -1 && found_bbf2_gauche != -1) {
        level.objects[found_bbf2_droite].link = found_bbf2_gauche;
        level.objects[found_bbf2_gauche].link = found_bbf2_droite;
    }

    if (bonus_map) {
        nb_wiz_save = status_bar.num_wiz;
        status_bar.num_wiz = 0;
    }

    init_struct_level();
}

void REINIT_OBJECT(obj_t* obj) {
    obj_init(obj);
    obj->flags.alive = 1;
    obj->active_flag = 2;
    calc_obj_pos(obj);
    obj->active_timer = obj->link_has_gendoor ? 0 : 120;
    obj->is_active = 0;
}

void special_flags_init(void) {
    s32 i;
    for (i = 0; i < 262; ++i) {
        if (i == TYPE_0_BADGUY1) {
            if (num_level == 3) {
                zonediffx[i] = -120;
            } else {
                zonediffx[i] = 0;
            }
        }
        if (i == TYPE_17_LIFTPLAT || i == TYPE_16_FALLPLAT || i == TYPE_24_INTERACTPLT) {
            if (num_world == world_1_jungle) {
                flags[i].flags1 |= 8;
            } else {
                flags[i].flags1 &= ~8;
            }
        }
    }
}
