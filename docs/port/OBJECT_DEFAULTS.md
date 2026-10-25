# Object initialization defaults

Numeric DOS types are authoritative. Existing TYPE_* macro names are corroborating labels, not new names. This transcription preserves conditions, calls, fallthrough and ordering. Generic pre/post operations are in each function contract and the full body retained in the JSON evidence. No C implementation is changed.

## Prio — 1:4e160

### 30 (`TYPE_30_AUDIOSTART`), 98 (`TYPE_98_SPACE_MAMA`), 204 (`TYPE_204_BLACK_RAY`), 213 (`TYPE_213_SPACE_MAMA2`), 236 (`TYPE_236_ANNULE_SORT_DARK`), 238 (`TYPE_238_POING_FEE`), 245 (`TYPE_245_DUNE`), 141 (`TYPE_141_NEIGE`), 158 (`TYPE_158_PALETTE_SWAPPER`), 164 (`TYPE_164_GENERATING_DOOR`), 181 (`TYPE_181_SCROLL_SAX`), 199 (`TYPE_199_BB1_VIT`)

```c
return 0;
```

### 149 (`TYPE_149_CLE_SOL`), 157 (`TYPE_157_EAU`), 197 (`TYPE_197_MEDAILLON_TOON`)

```c
return 1;
```

### 147 (`TYPE_147_MST_SCROLL`)

```c
if (obj->hit_points == 0)
  return 2;

return 0;
```

### 11 (`TYPE_11_BOUM`), 19 (`TYPE_19_SPLASH`), 21 (`TYPE_21_PHOTOGRAPHE`), 41 (`TYPE_41_OUYE`), 45 (`TYPE_45_MOVE_OUYE`), 48 (`TYPE_48_FLAMME2`), 57 (`TYPE_57_STALAG`), 75, 78 (`TYPE_78_PIRATE_BOMB`), 79 (`TYPE_79_STONECHIP`), 83 (`TYPE_83_EXPLOSION`), 90 (`TYPE_90_NOTE0`), 91 (`TYPE_91_NOTE1`), 92 (`TYPE_92_NOTE2`), 93 (`TYPE_93_BONNE_NOTE`), 94 (`TYPE_94_POING`), 102 (`TYPE_102_BNOTE`), 110 (`TYPE_110_PI_BOUM`), 121 (`TYPE_121_ECLAIR`), 135 (`TYPE_135_ETINC`), 143 (`TYPE_143_NOVA2`), 146 (`TYPE_146_FLASH`), 150 (`TYPE_150_SCORPION`), 161 (`TYPE_161_WIZ`), 168 (`TYPE_168_CYMBAL1`), 170 (`TYPE_170_RAYON`), 220 (`TYPE_220_PIERREACORDE`), 221 (`TYPE_221_CFUMEE`), 224 (`TYPE_224_STOSKO_PINCE`), 234 (`TYPE_234_LAVE`), 248 (`TYPE_248_SKO_PINCE`)

```c
return 2;
```

### 123 (`TYPE_123_BLACKTOON1`)

```c
if ((num_world == 1) && (num_level == 14))
  return 3;

return 2;
```

### 28 (`TYPE_28_ONOFF_PLAT`), 44 (`TYPE_44_CLASH`), 46 (`TYPE_46_BB1`), 58 (`TYPE_58_CAGE`), 59 (`TYPE_59_CAGE2`), 66 (`TYPE_66_DARD`), 72 (`TYPE_72_PIRATE_NGAWE`), 73 (`TYPE_73_RING`), 74 (`TYPE_74_SAXO`), 77 (`TYPE_77_PIRATE_GUETTEUR`), 86 (`TYPE_86_MARACAS`), 97 (`TYPE_97_BBL`), 119 (`TYPE_119_TNT_BOMB`), 133 (`TYPE_133_SUPERHELICO`), 138 (`TYPE_138_ROULETTE`), 154 (`TYPE_154_ROULETTE2`), 155 (`TYPE_155_ROULETTE3`), 180 (`TYPE_180_SAXO2`), 183 (`TYPE_183_SAXO3`), 187 (`TYPE_187_MAMA_PIRATE`), 190 (`TYPE_190_COUTEAU`), 198 (`TYPE_198_BB12`), 200 (`TYPE_200_BB13`), 201 (`TYPE_201_BB14`), 203 (`TYPE_203_SMA_WEAPON`), 211 (`TYPE_211_BOUT_TOTEM`), 239 (`TYPE_239_PIRATE_GUETTEUR2`), 249 (`TYPE_249_RIDEAU`)

```c
return 3;
```

### 253 (`TYPE_253_PLANCHES`)

```c
return (bateau_obj_id != (-1)) ? (4) : (7);
```

### 7 (`TYPE_7_MORNINGSTAR`), 20 (`TYPE_20_GENEBADGUY`), 96 (`TYPE_96_TOTEM`), 109 (`TYPE_109_PI`), 111 (`TYPE_111_PI_MUS`), 112 (`TYPE_112_WASHING_MACHINE`), 246 (`TYPE_246_CORDE_DARK`), 251 (`TYPE_251_VAGUE_DEVANT`)

```c
return 5;
```

### 2 (`TYPE_2_POWERUP`), 31 (`TYPE_31_ONEUP_ALWAYS`), 55 (`TYPE_55_MUS_WAIT`), 82 (`TYPE_82_JAUGEUP`), 95 (`TYPE_95_POING_POWERUP`), 137 (`TYPE_137_REDUCTEUR`), 142 (`TYPE_142_ONEUP`), 148 (`TYPE_148_GRAP_BONUS`), 173 (`TYPE_173_BATEAU`)

```c
return 6;
```

### 4 (`TYPE_4_NEUTRAL`), 42 (`TYPE_42_SIGNPOST`), 88 (`TYPE_88_TAMBOUR2`), 252 (`TYPE_252_VAGUE_DERRIERE`)

```c
return 7;
```

### 192 (`TYPE_192_SMA_GRAND_LASER`), 193 (`TYPE_193_SMA_BOMB`), 194 (`TYPE_194_SMA_BOMB_CHIP`)

```c
return 4;
```

### 196 (`TYPE_196_DARD_PLAFOND`), 244 (`TYPE_244_SMA_PETIT_LASER`)

```c
break;
```

### default

```c
break;
```

## first_obj_init — 1:4e640

### 98 (`TYPE_98_SPACE_MAMA`), 213 (`TYPE_213_SPACE_MAMA2`)

```c
findMereDenisWeapon()
obj->link = 0
break;
```

### 153 (`TYPE_153_FEE`)

```c
obj->detect_zone = 200
obj->link = 0
break;
```

### 70 (`TYPE_70_TROMPETTE`), 217 (`TYPE_217_MITE2`)

```c
obj->detect_zone = 160
obj->link = 0
break;
```

### 50 (`TYPE_50_MOSKITO`), 227 (`TYPE_227_MOSKITO2`), 5 (`TYPE_5_WIZARD1`), 65 (`TYPE_65_SPIDER`)

```c
obj->detect_zone = 60
obj->link = 0
break;
```

### 12 (`TYPE_12_CHASSEUR1`), 14 (`TYPE_14_CHASSEUR2`), 20 (`TYPE_20_GENEBADGUY`), 183 (`TYPE_183_SAXO3`)

```c
obj->detect_zone = 180
obj->link = 0
break;
```

### 172 (`TYPE_172_STONEWOMAN`), 56 (`TYPE_56_STONEWOMAN2`), 60 (`TYPE_60_BIG_CLOWN`), 100 (`TYPE_100_MITE`)

```c
obj->detect_zone = 220
obj->link = 0
break;
```

### 35 (`TYPE_35_STONEMAN1`), 40 (`TYPE_40_STONEDOG`), 43 (`TYPE_43_STONEMAN2`), 55 (`TYPE_55_MUS_WAIT`), 72 (`TYPE_72_PIRATE_NGAWE`), 120 (`TYPE_120_BATTEUR_FOU`), 122 (`TYPE_122_STONEDOG2`)

```c
obj->detect_zone = 100
obj->link = 0
break;
```

### 174 (`TYPE_174_PIRATE_POELLE`), 225 (`TYPE_225_PIRATE_P_45`)

```c
obj->detect_zone = 150
obj->link = 0
break;
```

### 184 (`TYPE_184_PIRATE_POELLE_D`), 226 (`TYPE_226_PIRATE_P_D_45`)

```c
obj->detect_zone = 50
obj->link = 0
break;
```

### 116 (`TYPE_116_CLOWN_TNT`)

```c
obj->detect_zone = 200
obj->link = 0
break;
```

### 57 (`TYPE_57_STALAG`)

```c
obj->detect_zone = 250
obj->link = 0
break;
```

### 81 (`TYPE_81_CYMBALE`), 168 (`TYPE_168_CYMBAL1`), 169 (`TYPE_169_CYMBAL2`)

```c
obj->detect_zone = 0
obj->link = 0
break;
```

### 113 (`TYPE_113_BAG1`)

```c
obj->param = 0
obj->link = 0
break;
```

### 150 (`TYPE_150_SCORPION`)

```c
obj->detect_zone = 160
obj->link = 0
break;
```

### 61 (`TYPE_61_WAT_CLOWN`), 187 (`TYPE_187_MAMA_PIRATE`), 32 (`TYPE_32_DARK_PHASE2`), 212 (`TYPE_212_DARK`)

```c
obj->detect_zone = 150
obj->link = 0
break;
```

### default

```c
obj->detect_zone = 40
obj->link = 0
break;
```

## obj_init — 1:4e9bc

### 74 (`TYPE_74_SAXO`), 180 (`TYPE_180_SAXO2`), 183 (`TYPE_183_SAXO3`)

```c
INIT_SAXO(obj)
break;
```

### 228 (`TYPE_228_PRI`)

```c
if (finBosslevel.helped_joe_2)
{
  prise_branchee = 1;
  set_main_and_sub_etat(obj, 0, 2);
  obj->init_sub_etat = 2;
  skipToLabel(obj, 1, 1);
  break;
}

break;
```

### 251 (`TYPE_251_VAGUE_DEVANT`), 252 (`TYPE_252_VAGUE_DERRIERE`)

```c
obj->x = (obj->init_x = -15)
break;
```

### 223 (`TYPE_223_HYBRIDE_STOSKO`)

```c
break;
```

### 218 (`TYPE_218_HYBRIDE_MOSAMS`)

```c
bossXToReach = obj->x
bossYToReach = (firstFloorBelow(&ray) - obj->offset_by) + 16
bossSpeedFactor = 0x4000
alternateBossSpeedFactor = 0
break;
```

### 176 (`TYPE_176_CRAYON_BAS`), 240 (`TYPE_240_CRAYON_HAUT`)

```c
{
  s16 frame_count = obj->animations[obj->anim_index].frames_count;
  obj->anim_frame = myRand(frame_count - 1);
}

break;
```

### 191 (`TYPE_191_FALLING_CRAYON`), 235 (`TYPE_235_PUNAISE4`)

```c
obj->anim_frame = obj->hit_points
break;
```

### 175 (`TYPE_175_PUNAISE1`), 229 (`TYPE_229_PUNAISE2`), 230 (`TYPE_230_PUNAISE3`), 250 (`TYPE_250_PUNAISE5`)

```c
obj->flags.flip_x = obj->hit_points & 1
break;
```

### 51 (`TYPE_51_MST_FRUIT1`), 53 (`TYPE_53_MST_SHAKY_FRUIT`)

```c
obj->gravity_value_2 = 5
break;
```

### 52 (`TYPE_52_MST_FRUIT2`)

```c
obj->gravity_value_2 = 10
break;
```

### 98 (`TYPE_98_SPACE_MAMA`)

```c
obj->type = TYPE_213_SPACE_MAMA2
```

### 213 (`TYPE_213_SPACE_MAMA2`)

```c
swapMereDenisCollZones(obj, 1)
setBossScrollLimits(obj)
bossSpeedFactor = 0x4000
alternateBossSpeedFactor = 0
laserSourceSprNumInAnim = -1
IsBossThere = 0
scrollLocked = 0
obj->timer = 180
saveBossEncounter = 255
currentBossAction = 0
currentBossActionIsOver = 0
bossEncounter = 0
break;
```

### 227 (`TYPE_227_MOSKITO2`)

```c
bossEncounter = 2
goto init_moskito;
```

### 50 (`TYPE_50_MOSKITO`)

```c
bossEncounter = 0
init_moskito:
obj->scale = 255;

setBossScrollLimits(obj)
bossSpeedFactor = 0x4000
alternateBossSpeedFactor = 0
IsBossThere = 0
scrollLocked = 0
obj->timer = 100
saveBossEncounter = 255
currentBossAction = 0
currentBossActionIsOver = 0
break;
```

### 0 (`TYPE_0_BADGUY1`)

```c
obj->offset_hy = 30
obj->flags.follow_enabled = 0
break;
```

### 144 (`TYPE_144_LIDOLPINK2`)

```c
obj->iframes_timer = 0
break;
```

### 123 (`TYPE_123_BLACKTOON1`)

```c
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
```

### 145 (`TYPE_145_KILLING_EYES`)

```c
obj->param = 40
obj->iframes_timer = 40
break;
```

### 72 (`TYPE_72_PIRATE_NGAWE`)

```c
obj->config = 1
break;
```

### 77 (`TYPE_77_PIRATE_GUETTEUR`), 239 (`TYPE_239_PIRATE_GUETTEUR2`)

```c
swapGuetteurCollZones(obj, obj->init_sub_etat == 14)
obj->iframes_timer = 0
obj->detect_zone = 80
break;
```

### 28 (`TYPE_28_ONOFF_PLAT`)

```c
obj->iframes_timer = (obj->follow_id = 100)
obj->flags.alive = 1
break;
```

### 26 (`TYPE_26_CRUMBLE_PLAT`)

```c
if (num_world != 1)
{
  s16 unk_2 = (s16) (vblToEOA(obj, 1) - 2);
  obj->param = unk_2;
  obj->iframes_timer = unk_2;
}
else
{
  obj->param = 40;
  obj->iframes_timer = 20;
}

obj->link = 20
obj->flags.alive = 1
obj->flags.follow_enabled = 1
break;
```

### 25 (`TYPE_25_INST_PLAT`)

```c
obj->iframes_timer = (obj->follow_id = 15)
obj->flags.alive = 1
break;
```

### 29 (`TYPE_29_AUTOJUMP_PLAT`), 34 (`TYPE_34_MOVE_AUTOJUMP_PLAT`), 152 (`TYPE_152_CAISSE_CLAIRE`), 237 (`TYPE_237_GOMME`), 243 (`TYPE_243_MARK_AUTOJUMP_PLAT`)

```c
obj->iframes_timer = (obj->follow_id = 1)
obj->flags.alive = 1
break;
```

### 116 (`TYPE_116_CLOWN_TNT`), 117 (`TYPE_117_CLOWN_TNT2`), 118 (`TYPE_118_CLOWN_TNT3`)

```c
obj->config = 0
obj->link = 0
break;
```

### 168 (`TYPE_168_CYMBAL1`), 169 (`TYPE_169_CYMBAL2`), 178 (`TYPE_178_HERSE_BAS`), 241 (`TYPE_241_HERSE_HAUT`)

```c
obj->link = 0
break;
```

### 150 (`TYPE_150_SCORPION`)

```c
obj->detect_zone = 160
sko_phase = 0
break;
```

### 21 (`TYPE_21_PHOTOGRAPHE`), 7 (`TYPE_7_MORNINGSTAR`)

```c
obj->flags.alive = 1
break;
```

### 75, 170 (`TYPE_170_RAYON`), 197 (`TYPE_197_MEDAILLON_TOON`)

```c
obj->flags.alive = 0
obj->is_active = 0
break;
```

### 184 (`TYPE_184_PIRATE_POELLE_D`), 226 (`TYPE_226_PIRATE_P_D_45`)

```c
obj->flags.flip_x = 1
break;
```

### 212 (`TYPE_212_DARK`)

```c
obj->param = 0
break;
```

### 217 (`TYPE_217_MITE2`)

```c
obj->eta[1][0].flags &= ~0x10
obj->eta[1][2].flags &= ~0x10
obj->follow_id = 0
break;
```

### 202 (`TYPE_202_RAY_ETOILES`)

```c
if ((obj->main_etat == 0) && (obj->sub_etat == 57))
  star_ray_der = obj;
else
  star_ray_dev = obj;

obj->display_prio = 3
obj->flags.alive = 0
obj->is_active = 0
break;
```

## INIT_OBJECTS — 1:4f2d4

### 37 (`TYPE_37_TARZAN`)

```c
{
  obj->flags.alive = !RayEvts.magicseed;
}

break;
```

### 249 (`TYPE_249_RIDEAU`)

```c
{
  if (obj->init_sub_etat == 2)
  {
    rideau_obj_id = obj->id;
  }
}

break;
```

### 133 (`TYPE_133_SUPERHELICO`)

```c
{
  if (!RayEvts.super_helico)
  {
    obj->flags.alive = finBosslevel.helped_musician;
  }
  else
  {
    obj->flags.alive = 0;
  }
}

break;
```

### 257 (`TYPE_257_BREAKOUT_GAME`)

```c
{
  cb_ball_obj_id = obj->id;
}

break;
```

### 96 (`TYPE_96_TOTEM`)

```c
{
  obj->param = 0;
  obj->iframes_timer = 0;
}

break;
```

### 246 (`TYPE_246_CORDE_DARK`)

```c
{
  corde_dark_obj_id = obj->id;
}

break;
```

### 223 (`TYPE_223_HYBRIDE_STOSKO`)

```c
{
  obj->param = 0;
  stosko_obj_id = obj->id;
  obj->flags.alive = 0;
  obj->is_active = 0;
}

break;
```

### 218 (`TYPE_218_HYBRIDE_MOSAMS`)

```c
{
  obj->param = 0;
  moskitosaxo_obj_id = obj->id;
  obj->flags.alive = 0;
  obj->is_active = 0;
}

break;
```

### 231 (`TYPE_231_HYB_BBF2_D`)

```c
{
  obj->param = 0;
  moskitomama_droite_obj_id = obj->id;
  obj->flags.alive = 0;
  obj->is_active = 0;
  found_bbf2_droite = obj->id;
}

break;
```

### 232 (`TYPE_232_HYB_BBF2_G`)

```c
{
  moskitomama_gauche_obj_id = obj->id;
  obj->param = 0;
  found_bbf2_gauche = obj->id;
  obj->flags.flip_x = 1;
  obj->flags.alive = 0;
  obj->is_active = 0;
}

break;
```

### 220 (`TYPE_220_PIERREACORDE`)

```c
{
  pierreAcorde_obj_id = i;
  obj->iframes_timer = 0;
}

break;
```

### 219 (`TYPE_219_CORDE`)

```c
{
  obj->hit_points = obj->init_hit_points >> 2;
  if (pierreAcorde_obj_id != (-1))
  {
    if (obj->x < level.objects[pierreAcorde_obj_id].x)
    {
      obj->y = level.objects[pierreAcorde_obj_id].y + 112;
    }
    else
    {
      obj->y = level.objects[pierreAcorde_obj_id].y + 120;
    }
    obj->y = obj->init_y;
  }
}

break;
```

### 179 (`TYPE_179_HERSE_BAS_NEXT`), 242 (`TYPE_242_HERSE_HAUT_NEXT`)

```c
{
  if (obj->field_1C == 0)
  {
    obj->flags.alive = 0;
    obj->is_active = 0;
  }
}

break;
```

### 157 (`TYPE_157_EAU`)

```c
{
  if (obj->hit_points != 0)
  {
    obj->x = xmap - obj->offset_bx;
    if (num_world == world_1_jungle)
    {
      if (RayEvts.magicseed)
      {
        obj->hit_points = 1;
      }
      obj->y = ymapmax + SCREEN_HEIGHT;
    }
    else
    {
      obj->param = obj->x;
      obj->hit_points = 0;
      if ((num_world != world_3_mountain) || (num_level != 7))
      {
        obj->y = ymapmax + (SCREEN_HEIGHT - 80);
        obj->init_y = ymapmax + (SCREEN_HEIGHT - 80);
        if ((num_world == world_5_cave) && (num_level == 8))
        {
          obj->y = (ymapmax + Bloc_lim_H2) - 80;
          obj->init_y = (ymapmax + Bloc_lim_H2) - 80;
        }
        else
          if (num_world == world_4_image)
        {
          obj->y = ymapmax + (SCREEN_HEIGHT - 70);
          obj->init_y = ymapmax + (SCREEN_HEIGHT - 70);
        }
      }
      else
      {
        obj->sub_speed = 0;
      }
    }
    obj->is_active = 1;
    eau_obj_id = i;
    obj->flags.alive = 1;
    obj->iframes_timer = 0;
    calc_obj_pos(obj);
  }
  else
  {
    if ((eau_obj_id != (-1)) && (num_world != world_1_jungle))
    {
      obj->flags.flip_x = 0;
      obj->speed_y = 0;
      obj->speed_x = 0;
      obj->x = (level.objects[eau_obj_id].param += 101);
      obj->y = level.objects[eau_obj_id].y;
      if ((num_world != world_3_mountain) || (num_level != 7))
      {
        obj->init_y = ymapmax + (SCREEN_HEIGHT - 80);
        if (num_world == world_4_image)
        {
          obj->init_y = ymapmax + (SCREEN_HEIGHT - 70);
        }
        if ((num_world == world_5_cave) && (num_level == 8))
        {
          obj->y = (eau_obj_id + level.objects[eau_obj_id].y) - obj->id;
          obj->init_y = obj->y;
        }
      }
      else
      {
        obj->init_y = obj->y;
        obj->sub_speed = 0;
      }
      calc_obj_pos(obj);
      obj->is_active = 1;
      obj->flags.alive = 1;
      obj->sub_etat = ++level.objects[eau_obj_id].hit_points;
      if (obj->sub_etat > 3)
      {
        obj->sub_etat -= 4;
      }
      obj->iframes_timer = 0;
    }
    else
    {
      obj->flags.alive = 0;
      obj->is_active = 0;
    }
  }
}

break;
```

### 153 (`TYPE_153_FEE`)

```c
{
  fee_obj_id = i;
}

break;
```

### 147 (`TYPE_147_MST_SCROLL`)

```c
{
  if (obj->hit_points == 0)
  {
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
}

break;
```

### 136 (`TYPE_136_DEMI_RAYMAN`)

```c
{
  obj->flags.alive = 0;
  obj->is_active = 0;
  reduced_rayman_id = i;
}

break;
```

### 99 (`TYPE_99_RAY_POS`)

```c
{
  if (save1.is_just_saved && (!bonus_map))
  {
    obj->flags.alive = 0;
  }
  switch (obj->main_etat)
  {
    case 0:
      obj->timer = 0;
      obj->is_active = 0;
      break;

    case 5:
      obj->timer = 30;
      break;

  }

}

break;
```

### 64 (`TYPE_64_SCROLL`)

```c
{
  scroll_obj_id[NumScrollObj] = i;
  obj->y = obj->init_y;
  NumScrollObj++;
  obj->x = obj->init_x;
  if (NumScrollObj >= 50)
  {
    FatalError("Augmenter MAX_NB_SCROLL_PER_MAP");
  }
  obj->flags.alive = 0;
}

break;
```

### 19 (`TYPE_19_SPLASH`)

```c
obj->flags.alive = 0
break;
```

### 9 (`TYPE_9_BADGUY2`), 165 (`TYPE_165_BADGUY3`)

```c
{
  obj->eta[2][2].sound_index = 0;
}

break;
```

### 81 (`TYPE_81_CYMBALE`)

```c
{
  cymbal_obj_id[nb_cymbal_in_map++] = i;
}

break;
```

### 23 (`TYPE_23_RAYMAN`)

```c
{
  obj->flags.alive = 0;
  rayman_obj_id = i;
}

break;
```

### 205 (`TYPE_205_BLACK_FIST`)

```c
{
  obj->flags.alive = 0;
  black_fist_obj_id = obj->id;
}

break;
```

### 94 (`TYPE_94_POING`)

```c
{
  obj->init_sub_etat = poing.sub_etat;
  poing_obj_id = i;
  obj->sub_etat = poing.sub_etat;
  poing_obj = &level.objects[poing_obj_id];
  obj->init_flag = 7;
  obj->flags.alive = 0;
}

break;
```

### 11 (`TYPE_11_BOUM`)

```c
{
  sbar_obj_id = i;
  obj->init_flag = 7;
  obj->flags.alive = 0;
}

break;
```

### 145 (`TYPE_145_KILLING_EYES`)

```c
{
  obj->iframes_timer = 40;
}

break;
```

### 39 (`TYPE_39_NEN_GRAINE`)

```c
{
  obj->iframes_timer = 0;
}

```

### 203 (`TYPE_203_SMA_WEAPON`), 224 (`TYPE_224_STOSKO_PINCE`), 238 (`TYPE_238_POING_FEE`), 38 (`TYPE_38_GRAINE`)

```c
{
  obj->flags.alive = 0;
}

break;
```

### 65 (`TYPE_65_SPIDER`)

```c
{
  obj->link = 0;
}

break;
```

### 13 (`TYPE_13_BALLE1`), 15 (`TYPE_15_BALLE2`), 33 (`TYPE_33_DARK2_SORT`), 36 (`TYPE_36_STONEBOMB`), 44 (`TYPE_44_CLASH`), 51 (`TYPE_51_MST_FRUIT1`), 52 (`TYPE_52_MST_FRUIT2`), 59 (`TYPE_59_CAGE2`), 62 (`TYPE_62_DROP`), 66 (`TYPE_66_DARD`), 71 (`TYPE_71_NOTE`), 73 (`TYPE_73_RING`), 78 (`TYPE_78_PIRATE_BOMB`), 79 (`TYPE_79_STONECHIP`), 83 (`TYPE_83_EXPLOSION`), 90 (`TYPE_90_NOTE0`), 91 (`TYPE_91_NOTE1`), 92 (`TYPE_92_NOTE2`), 93 (`TYPE_93_BONNE_NOTE`), 97 (`TYPE_97_BBL`), 102 (`TYPE_102_BNOTE`), 108 (`TYPE_108_DARK2_PINK_FLY`), 110 (`TYPE_110_PI_BOUM`), 112 (`TYPE_112_WASHING_MACHINE`), 115 (`TYPE_115_BB1_PLAT`), 119 (`TYPE_119_TNT_BOMB`), 121 (`TYPE_121_ECLAIR`), 122 (`TYPE_122_STONEDOG2`), 130 (`TYPE_130_BLACKTOON_EYES`), 135 (`TYPE_135_ETINC`), 139 (`TYPE_139_MARACAS_BAS`), 143 (`TYPE_143_NOVA2`), 146 (`TYPE_146_FLASH`), 186 (`TYPE_186_POELLE_ALWAYS`), 192 (`TYPE_192_SMA_GRAND_LASER`), 193 (`TYPE_193_SMA_BOMB`), 194 (`TYPE_194_SMA_BOMB_CHIP`), 196 (`TYPE_196_DARD_PLAFOND`), 197 (`TYPE_197_MEDAILLON_TOON`), 206 (`TYPE_206_PIEDS_RAYMAN`), 209 (`TYPE_209_FIRE_LEFT`), 210 (`TYPE_210_FIRE_RIGHT`), 215 (`TYPE_215_DARK_SORT`), 233 (`TYPE_233_HYB_BBF2_LAS`), 244 (`TYPE_244_SMA_PETIT_LASER`)

```c
{
  obj->is_active = 0;
  obj->flags.alive = 0;
  obj->init_flag = 7;
}

break;
```

### 3 (`TYPE_3_LIDOLPINK`)

```c
{
  obj->init_flag = 7;
  obj->flags.alive = 0;
}

break;
```

### 20 (`TYPE_20_GENEBADGUY`)

```c
{
  obj->init_flag = 7;
  obj->timer = 0;
}

break;
```

### 204 (`TYPE_204_BLACK_RAY`)

```c
{
  obj->is_active = 0;
  obj->flags.alive = 0;
  obj->init_flag = 7;
  obj->animations = ray.animations;
  black_ray_obj_id = obj->id;
}

break;
```

### 190 (`TYPE_190_COUTEAU`)

```c
{
  obj->follow_id = 1;
  obj->is_active = 0;
  obj->init_flag = 7;
  obj->flags.alive = 0;
  obj->iframes_timer = obj->follow_id;
}

break;
```

### 10 (`TYPE_10_FISH`)

```c
{
  if (i >= 1)
  {
    obj_t *prev_obj = &level.objects[i - 1];
    obj->flags.alive = 1;
    if ((prev_obj->type == TYPE_10_FISH) && (prev_obj->x == obj->x))
    {
      obj->flags.alive = 0;
      obj->is_active = 0;
    }
  }
}

break;
```

### 46 (`TYPE_46_BB1`), 198 (`TYPE_198_BB12`), 200 (`TYPE_200_BB13`)

```c
{
  INIT_BBMONT(obj);
}

break;
```

### 111 (`TYPE_111_PI_MUS`)

```c
{
  if (finBosslevel.helped_musician)
  {
    set_sub_etat(obj, 10);
    obj->init_sub_etat = 10;
  }
}

```

### 82 (`TYPE_82_JAUGEUP`), 109 (`TYPE_109_PI`), 142 (`TYPE_142_ONEUP`), 148 (`TYPE_148_GRAP_BONUS`)

```c
{
  obj->init_flag = 7;
}

break;
```

### 55 (`TYPE_55_MUS_WAIT`)

```c
{
  obj->init_flag = 7;
  Mus_obj_id = obj->id;
  if (finBosslevel.helped_musician)
  {
    obj->init_sub_etat = 4;
    set_main_and_sub_etat(obj, 0, 4);
  }
}

break;
```

### 31 (`TYPE_31_ONEUP_ALWAYS`)

```c
{
  obj->flags.alive = 0;
  obj->init_flag = 7;
}

break;
```

### 173 (`TYPE_173_BATEAU`)

```c
{
  obj->param = -1;
  obj->link = 0;
  bateau_obj_id = i;
  obj->timer = 0;
}

break;
```

### 21 (`TYPE_21_PHOTOGRAPHE`)

```c
{
  obj->timer = 0;
}

break;
```

### 187 (`TYPE_187_MAMA_PIRATE`)

```c
{
  mama_pirate_obj_id = i;
}

break;
```

### 32 (`TYPE_32_DARK_PHASE2`)

```c
{
  obj->flags.alive = 0;
  obj->x = (obj->init_x = 0);
  obj->y = (obj->init_y = 0);
  obj->is_active = 0;
}

break;
```
