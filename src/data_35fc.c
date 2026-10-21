/* ObjectsFonctions: DOS GOG v1.21 2:035fc..2:04a74 (5240 bytes).
 * Block split/name INVENTED; function and field names RAYVERSE.
 * DOS: 262 records, stride 20; every 4-byte slot has a code relocation.
 * Indexing: command 1:48325 (+0), poing 1:1d7a0 (+4),
 * rayman collision 1:1ec54/1:1ed1a (+8), detection zone 1:48292 (+12).
 * +16 is a function pointer; its u_turn role/signature is RAYVERSE semantic
 * evidence (no direct DOS access found). Existing callers use the same type.
 * DOS correction: type 182 command = DO_NOTE_CMD (1:65b0c), not DO_ONE_CMD.
 * 2:035fa..2:035fc are preceding alignment bytes, not table members.
 * See NOTES.md for evidence, boundaries, and verification commands.
 */
#include "obj.h"

typedef struct obj_procs_t {
    void (*command)(obj_t *obj);
    void (*poing_collision)(obj_t *obj, s16 sprite);
    void (*rayman_collision)(obj_t *obj);
    void (*rayman_in_zone)(obj_t *obj);
    void (*u_turn)(obj_t *obj);
} obj_procs_t;

typedef char obj_procs_size_check[(sizeof(obj_procs_t) == 20) ? 1 : -1];

extern void BAT_ray_in_zone(obj_t *obj);
extern void BombExplosion(obj_t *obj);
extern void DO_2_PARTS_CYMBAL(obj_t *obj);
extern void DO_BAG1_COMMAND(obj_t *obj);
extern void DO_BAT_COMMAND(obj_t *obj);
extern void DO_BAT_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_BB1_PLAT_CMD(obj_t *obj);
extern void DO_BBL_COMMAND(obj_t *obj);
extern void DO_BBMONT2_COMMAND(obj_t *obj);
extern void DO_BBMONT2_MARCHE(obj_t *obj);
extern void DO_BBMONT3_COMMAND(obj_t *obj);
extern void DO_BBMONT4_COMMAND(obj_t *obj);
extern void DO_BBMONT_COMMAND(obj_t *obj);
extern void DO_BBMONT_TOUCHE(obj_t *obj, s16 sprite);
extern void DO_BLKTOON_COMMAND(obj_t *obj);
extern void DO_BLKTOON_EYES_CMD(obj_t *obj);
extern void DO_BOUEE_JOE_COMMAND(obj_t *obj);
extern void DO_CAGE(obj_t *obj);
extern void DO_CAGE2(obj_t *obj);
extern void DO_CB_BALL_COMMAND(obj_t *obj);
extern void DO_CB_BRIK_COMMAND(obj_t *obj);
extern void DO_CCL_COMMAND(obj_t *obj);
extern void DO_CFUMEE_COMMAND(obj_t *obj);
extern void DO_CLOWN_TNT2_COMMAND(obj_t *obj);
extern void DO_CLOWN_TNT3_COMMAND(obj_t *obj);
extern void DO_CLOWN_TNT_COMMAND(obj_t *obj);
extern void DO_COLL_RAY_CYMBALE(obj_t *obj);
extern void DO_CORDE_COMMAND(obj_t *obj);
extern void DO_COU_COMMAND(obj_t *obj);
extern void DO_CYMBAL_COMMAND(obj_t *obj);
extern void DO_DARD_PLAFOND_ALWAYS(obj_t *obj);
extern void DO_DARK2_SORT_COMMAND(obj_t *obj);
extern void DO_DARK2_TOONS_COMMAND(obj_t *obj);
extern void DO_DARK_COMMAND(obj_t *obj);
extern void DO_DARK_PHASE2_COMMAND(obj_t *obj);
extern void DO_DARK_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_DARK_REACT(obj_t *obj);
extern void DO_DARK_SORT_COLLISION(obj_t *obj);
extern void DO_DARK_SORT_COMMAND(obj_t *obj);
extern void DO_DROP_COMMAND(obj_t *obj);
extern void DO_EAU_QUI_MONTE(obj_t *obj);
extern void DO_ECLAIR_COMMAND(obj_t *obj);
extern void DO_ENSEIGNE_COMMAND(obj_t *obj);
extern void DO_FALLING_OBJ_CMD(obj_t *obj);
extern void DO_FEE(obj_t *obj);
extern void DO_FLASH_COMMAND(obj_t *obj);
extern void DO_HYB_BBF2_LAS(obj_t *obj);
extern void DO_HYB_BBF2_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_IDC_COMMAND(obj_t *obj);
extern void DO_JOE_COMMAND(obj_t *obj);
extern void DO_LEV_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_MEDAILLON_TOON(obj_t *obj);
extern void DO_MITE2_COMMAND(obj_t *obj);
extern void DO_MIT_COMMAND(obj_t *obj);
extern void DO_MOVE_MARACAS_COMMAND(obj_t *obj);
extern void DO_MOVING_PLATFORM_COMMAND(obj_t *obj);
extern void DO_MOVING_WITH_INDICATOR_COMMAND(obj_t *obj);
extern void DO_MST_CHANGE_COMMAND(obj_t *obj);
extern void DO_MST_SCROLL_COMMAND(obj_t *obj);
extern void DO_MUSICIEN(obj_t *obj);
extern void DO_NGW_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_NOMOVE_STONEWOMAN_COMMAND(obj_t *obj);
extern void DO_NOTE_CMD(obj_t *obj);
extern void DO_NOTE_TOUCHEE(obj_t *obj, s16 sprite);
extern void DO_NOVA2_COMMAND(obj_t *obj);
extern void DO_ONE_CMD(obj_t *obj);
extern void DO_ONE_NGW_COMMAND(obj_t *obj);
extern void DO_ONE_NGW_RING_COMMAND(obj_t *obj);
extern void DO_ONE_PAR_COMMAND(obj_t *obj);
extern void DO_ONE_PINK_CMD(obj_t *obj);
extern void DO_ONE_STONECHIP_COMMAND(obj_t *obj);
extern void DO_PAC_COMMAND(obj_t *obj);
extern void DO_PAR_BOMB_COMMAND(obj_t *obj);
extern void DO_PAR_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_PETIT_COUTEAU_COMMAND(obj_t *obj);
extern void DO_PHOTOGRAPHE_CMD(obj_t *obj);
extern void DO_PIEDS_RAYMAN(obj_t *obj);
extern void DO_PIRATE_POELLE(obj_t *obj);
extern void DO_PIRATE_POELLE_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_PMA_COMMAND(obj_t *obj);
extern void DO_PMA_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_PNG_COLL_STONEMAN(obj_t *obj, s16 sprite);
extern void DO_PNG_COLL_STONEWOMAN(obj_t *obj, s16 sprite);
extern void DO_POELLE_COMMAND(obj_t *obj);
extern void DO_POING(obj_t *obj);
extern void DO_POISSON_BLEU_CMD(obj_t *obj);
extern void DO_POISSON_VERT_CMD(obj_t *obj);
extern void DO_PTI_ESQUIVE(obj_t *obj);
extern void DO_PYRANHA(obj_t *obj);
extern void DO_RAY_POS_CMD(obj_t *obj);
extern void DO_REDUCTEUR(obj_t *obj);
extern void DO_ROLL_EYES(obj_t *obj);
extern void DO_SAXO2_COMMAND(obj_t *obj);
extern void DO_SAXO3_COMMAND(obj_t *obj);
extern void DO_SAXO3_DEBUT(obj_t *obj);
extern void DO_SAXO_COMMAND(obj_t *obj);
extern void DO_SAXO_MARCHE(obj_t *obj);
extern void DO_SKO(obj_t *obj);
extern void DO_SKO_HIT(obj_t *obj, s16 sprite);
extern void DO_SLOPEY_PLAT_CMD(obj_t *obj);
extern void DO_SPIDER_PLAFOND(obj_t *obj);
extern void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_SPIDER_TIR(obj_t *obj);
extern void DO_STONEBOMB_COMMAND(obj_t *obj);
extern void DO_STONEDOG_COMMAND(obj_t *obj);
extern void DO_STONEMAN1_TIR(obj_t *obj);
extern void DO_STONEMAN2_TIR(obj_t *obj);
extern void DO_STONEWOMAN_COMMAND(obj_t *obj);
extern void DO_STONE_EXPLOSION(obj_t *obj, s16 sprite);
extern void DO_TARZAN(obj_t *obj);
extern void DO_TEN_COMMAND(obj_t *obj);
extern void DO_TIRE_BOUCHON_COMMAND(obj_t *obj);
extern void DO_TOTEM_COMMAND(obj_t *obj);
extern void DO_TOTEM_TOUCHE(obj_t *obj, s16 sprite);
extern void DO_TRP_COMMAND(obj_t *obj);
extern void DO_VITRAIL_COMMAND(obj_t *obj);
extern void DO_WLKNOT_COMMAND(obj_t *obj);
extern void DoAnnuleDarkSortRaymanCollision(obj_t *obj);
extern void DoAudioStartRaymanCollision(obj_t *obj);
extern void DoBadGuy1PoingCollision(obj_t *obj, s16 sprite);
extern void DoBadGuy1RaymanZDD(obj_t *obj);
extern void DoBadGuy23PoingCollision(obj_t *obj, s16 sprite);
extern void DoBadGuy23RaymanZDD(obj_t *obj);
extern void DoBlackToonPoingCollision(obj_t *obj, s16 sprite);
extern void DoBlackToonRaymanZDD(obj_t *obj);
extern void DoCagePoingCollision(obj_t *obj, s16 sprite);
extern void DoCaisseClairePoingCollision(obj_t *obj, s16 sprite);
extern void DoCaisseClaireRaymanZDD(obj_t *obj);
extern void DoChasseurPoingCollision(obj_t *obj, s16 sprite);
extern void DoChasseurRaymanZDD(obj_t *obj);
extern void DoFallingObjPoingCollision(obj_t *obj, s16 sprite);
extern void DoFeeRaymanZDD(obj_t *obj);
extern void DoFishPoingCollision(obj_t *obj, s16 sprite);
extern void DoFlammeCommand(obj_t *obj);
extern void DoFlammeRaymanCollision(obj_t *obj);
extern void DoGeneBadGuyPoingCollision(obj_t *obj, s16 sprite);
extern void DoGeneBadGuyRaymanZDD(obj_t *obj);
extern void DoGeneratingDoorRaymanCollision(obj_t *obj);
extern void DoGraineRaymanCollision(obj_t *obj);
extern void DoGrapBonusRaymanCollision(obj_t *obj);
extern void DoHybBBF2LasRaymanCollision(obj_t *obj);
extern void DoJaugeUpRaymanCollision(obj_t *obj);
extern void DoJoeRaymanZDD(obj_t *obj);
extern void DoKillingEyesPoingCollision(obj_t *obj, s16 sprite);
extern void DoLidolPinkRaymanZDD(obj_t *obj);
extern void DoMOSAMSPoingCollision(obj_t *obj, s16 sprite);
extern void DoMite2PoingCollision(obj_t *obj, s16 sprite);
extern void DoMitePoingCollision(obj_t *obj, s16 sprite);
extern void DoMorningStarPoingCollision(obj_t *obj, s16 sprite);
extern void DoMstShakyFruitRaymanZDD(obj_t *obj);
extern void DoMusicienRaymanInZDD(obj_t *obj);
extern void DoNeigeRaymanCollision(obj_t *obj);
extern void DoOneUpPoingCollision(obj_t *obj, s16 sprite);
extern void DoOneUpRaymanCollision(obj_t *obj);
extern void DoPTGRAPPINPoingCollision(obj_t *obj, s16 sprite);
extern void DoPancarteRaymanCollision(obj_t *obj);
extern void DoPetitCouteauPoingCollision(obj_t *obj, s16 sprite);
extern void DoPiMusPoingCollision(obj_t *obj, s16 sprite);
extern void DoPiPoingCollision(obj_t *obj, s16 sprite);
extern void DoPlatformPoingCollision(obj_t *obj, s16 sprite);
extern void DoPoingCollisionDefault(obj_t *obj, s16 sprite);
extern void DoPoingPowerupRaymanCollision(obj_t *obj);
extern void DoPoissonBleuPoingCollision(obj_t *obj, s16 sprite);
extern void DoPoissonBleuRaymanZDD(obj_t *obj);
extern void DoPowerupRaymanCollision(obj_t *obj);
extern void DoPrisePoingCollision(obj_t *obj, s16 sprite);
extern void DoRaymanBag1Collision(obj_t *obj);
extern void DoRaymanCollisionDefault(obj_t *obj);
extern void DoRaymanInZDDDefault(obj_t *obj);
extern void DoReducteurRaymanCollision(obj_t *obj);
extern void DoSTOSKOPoingCollision(obj_t *obj, s16 sprite);
extern void DoSignPostRaymanCollision(obj_t *obj);
extern void DoSpiderPoingCollision(obj_t *obj, s16 sprite);
extern void DoSpiderRaymanZDD(obj_t *obj);
extern void DoStalagRaymanZDD(obj_t *obj);
extern void DoStoneDogPoingCollision(obj_t *obj, s16 sprite);
extern void DoStoneDogRaymanZDD(obj_t *obj);
extern void DoStoneMan1et2RaymanZDD(obj_t *obj);
extern void DoSuperHelicoRaymanCollision(obj_t *obj);
extern void DoTarzanPoingCollision(obj_t *obj, s16 sprite);
extern void DoTntClownPoingCollision(obj_t *obj, s16 sprite);
extern void DoTntClownRaymanZDD(obj_t *obj);
extern void DoTrompetteRaymanZDD(obj_t *obj);
extern void DoTrumpetPoingCollision(obj_t *obj, s16 sprite);
extern void DoWatAndBigClownPoingCollision(obj_t *obj, s16 sprite);
extern void DoWatAndBigClownRaymanZDD(obj_t *obj);
extern void DoWizRaymanCollision(obj_t *obj);
extern void NGW_REACT_TO_RAY_IN_ZONE(obj_t *obj);
extern void ObjectUTurnDefault(obj_t *obj);
extern void PAR_REACT_TO_RAY_IN_ZONE(obj_t *obj);
extern void PIRATE_POELLE_REACT(obj_t *obj);
extern void SKO_ray_in_zone(obj_t *obj);
extern void SPIDER_PLAFOND_REACT(obj_t *obj);
extern void TEST_WIZARD(obj_t *obj);
extern void ToonDonnePoing(obj_t *obj);
extern void blkUTurn(obj_t *obj);
extern void doBBF2command(obj_t *obj);
extern void doBlKRayRaymanCollision(obj_t *obj);
extern void doBlackRaymanCommand(obj_t *obj);
extern void doHerseCommand(obj_t *obj);
extern void doMOSAMScommand(obj_t *obj);
extern void doMereDenisCommand(obj_t *obj);
extern void doMereDenisHit(obj_t *obj, s16 sprite);
extern void doMoskitoCommand(obj_t *obj);
extern void doMoskitoHit(obj_t *obj, s16 sprite);
extern void doSTOSKOcommand(obj_t *obj);
extern void doShipCommand(obj_t *obj);
extern void mereDenisBigLaserCommand(obj_t *obj);
extern void mereDenisBombCommand(obj_t *obj);

obj_procs_t ObjectsFonctions[262] = {
    {DO_ONE_CMD, DoBadGuy1PoingCollision, DoRaymanCollisionDefault, DoBadGuy1RaymanZDD, ObjectUTurnDefault}, // 0: livingstone
    {DO_ONE_CMD, DoPlatformPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 1: platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoPowerupRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 2: energy point
    {DO_ONE_PINK_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoLidolPinkRaymanZDD, ObjectUTurnDefault}, // 3: electoon
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 4: scenery
    {TEST_WIZARD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 5: magician
    {DO_ONE_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 6: yin with pin
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 7: swinging spiky fruit
    {DO_FALLING_OBJ_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 8: plum 1
    {DO_ONE_CMD, DoBadGuy23PoingCollision, DoRaymanCollisionDefault, DoBadGuy23RaymanZDD, ObjectUTurnDefault}, // 9: small livingstone
    {DO_PYRANHA, DoFishPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 10: piranha
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 11: hit effect
    {DO_ONE_CMD, DoChasseurPoingCollision, DoRaymanCollisionDefault, DoChasseurRaymanZDD, ObjectUTurnDefault}, // 12: hunter 1
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 13: hunter bullet
    {DO_ONE_CMD, DoChasseurPoingCollision, DoRaymanCollisionDefault, DoChasseurRaymanZDD, ObjectUTurnDefault}, // 14: hunter 2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 15:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 16: falling platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 17: rising platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 18:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 19: splash
    {DO_TEN_COMMAND, DoGeneBadGuyPoingCollision, DoRaymanCollisionDefault, DoGeneBadGuyRaymanZDD, ObjectUTurnDefault}, // 20: tentacle
    {DO_PHOTOGRAPHE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 21: photographer
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 22: moving platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 23: rayman
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 24:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 25: ejecting platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 26: disappearing cloud
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 27: trampoline cloud
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 28: blinking cloud
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 29:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoAudioStartRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 30: audio start
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 31: 1-up collected
    {DO_DARK_PHASE2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 32: Mr Dark 2nd phase
    {DO_DARK2_SORT_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 33: Mr Dark fireballs
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 34: horizontal nenuphar 2
    {DO_STONEMAN1_TIR, DO_PNG_COLL_STONEMAN, DoRaymanCollisionDefault, DoStoneMan1et2RaymanZDD, ObjectUTurnDefault}, // 35: stoneman (throwing)
    {DO_STONEBOMB_COMMAND, DO_STONE_EXPLOSION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 36: stoneman lava ball
    {DO_TARZAN, DoTarzanPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 37: tarayzan
    {DO_ONE_CMD, DoPoingCollisionDefault, DoGraineRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 38: magic seed
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 39: planted nenuphar
    {DO_STONEDOG_COMMAND, DoStoneDogPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 40: stone dog
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 41: prickly
    {DO_ONE_CMD, DoPoingCollisionDefault, DoSignPostRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 42: exit sign
    {DO_STONEMAN2_TIR, DO_PNG_COLL_STONEMAN, DoRaymanCollisionDefault, DoStoneMan1et2RaymanZDD, ObjectUTurnDefault}, // 43: stoneman (shooting)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 44: stoneman flame
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 45: moving prickly
    {DO_BBMONT_COMMAND, DO_BBMONT_TOUCHE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 46: Mr Stone
    {DO_STONEBOMB_COMMAND, DO_STONE_EXPLOSION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 47: bouncing lava ball
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 48: torch
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 49: moving flat stone
    {doMoskitoCommand, doMoskitoHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 50: moskito (Bzzit)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 51: moskito fruit small
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 52: moskito fruit medium
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoMstShakyFruitRaymanZDD, ObjectUTurnDefault}, // 53: shaky fruit
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 54: medaillon
    {DO_MUSICIEN, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoMusicienRaymanInZDD, ObjectUTurnDefault}, // 55: musician
    {DO_NOMOVE_STONEWOMAN_COMMAND, DO_PNG_COLL_STONEWOMAN, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 56: nonmoving stoneman
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoStalagRaymanZDD, ObjectUTurnDefault}, // 57: stake
    {DO_CAGE, DoCagePoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 58: cage
    {DO_CAGE2, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 59: opened cage
    {DO_ONE_CMD, DoWatAndBigClownPoingCollision, DoRaymanCollisionDefault, DoWatAndBigClownRaymanZDD, ObjectUTurnDefault}, // 60: big clown
    {DO_ONE_CMD, DoWatAndBigClownPoingCollision, DoRaymanCollisionDefault, DoWatAndBigClownRaymanZDD, ObjectUTurnDefault}, // 61: big water clown
    {DO_DROP_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 62: water balloon
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 63: platform starts moving
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 64: autoscroll
    {DO_SPIDER_TIR, DoSpiderPoingCollision, DoRaymanCollisionDefault, DoSpiderRaymanZDD, ObjectUTurnDefault}, // 65: spider
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 66: spider dart
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 67: swinging flower
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 68: big boing plat
    {DO_STONEBOMB_COMMAND, DO_STONE_EXPLOSION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 69: big bouncing boulder
    {DO_TRP_COMMAND, DoTrumpetPoingCollision, DoRaymanCollisionDefault, DoTrompetteRaymanZDD, ObjectUTurnDefault}, // 70: trumpet
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 71: note
    {DO_ONE_NGW_COMMAND, DO_NGW_POING_COLLISION, DoRaymanCollisionDefault, NGW_REACT_TO_RAY_IN_ZONE, ObjectUTurnDefault}, // 72: pirate with earring
    {DO_ONE_NGW_RING_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 73: earring
    {DO_SAXO_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 74: Mr Sax
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 75: bounce glitter
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 76: kildoor
    {DO_ONE_PAR_COMMAND, DO_PAR_POING_COLLISION, DoRaymanCollisionDefault, PAR_REACT_TO_RAY_IN_ZONE, ObjectUTurnDefault}, // 77: bomber pirate
    {DO_PAR_BOMB_COMMAND, DoPoingCollisionDefault, BombExplosion, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 78: pirate bomb
    {DO_ONE_STONECHIP_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 79: lava ball fragment
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 80: bigstone
    {DO_CYMBAL_COMMAND, DoPoingCollisionDefault, DO_COLL_RAY_CYMBALE, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 81: cymbal
    {DO_ONE_CMD, DoPoingCollisionDefault, DoJaugeUpRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 82: big powerup
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 83: explosion / dust effect
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 84: tibetan juggler
    {DO_ROLL_EYES, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 85: eyes
    {DO_MOVE_MARACAS_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 86: maracas
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 87: tambourin platform1
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 88: tambourin platform2
    {DO_JOE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoJoeRaymanZDD, ObjectUTurnDefault}, // 89: Joe
    {DO_ONE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 90: Mr Sax wrong note
    {DO_ONE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 91: note bomb 1
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 92: note bomb 2
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 93: punched wrong note
    {DO_POING, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 94: fist
    {DO_ONE_CMD, DoPoingCollisionDefault, DoPoingPowerupRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 95: fist pickup
    {DO_TOTEM_COMMAND, DO_TOTEM_TOUCHE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 96: Mr Stone totem
    {DO_BBL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 97: Mr Stone boulder
    {doMereDenisCommand, doMereDenisHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 98: Space Mama
    {DO_RAY_POS_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 99: Rayman start pos
    {DO_MIT_COMMAND, DoMitePoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 100: moth
    {DO_ONE_CMD, DoMorningStarPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 101: swinging prickly
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 102: tiny wrong note
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 103: large eyed fish
    {DO_POISSON_VERT_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 104: large nosed fish
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 105: bouncing crystal rock 1
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 106: bouncing crystal rock 2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 107: big spiky ball
    {DO_DARK2_TOONS_COMMAND, DoPoingCollisionDefault, ToonDonnePoing, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 108: Mr Dark electoons
    {DO_ONE_CMD, DoPiPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 109: boulder
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 110: destroyed boulder
    {DO_ONE_CMD, DoPiMusPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 111: crushed guitar
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 112: washing machine
    {DO_BAG1_COMMAND, DoPoingCollisionDefault, DoRaymanBag1Collision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 113: mallets
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 114:
    {DO_BB1_PLAT_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 115: Mr Stone cloud
    {DO_CLOWN_TNT_COMMAND, DoTntClownPoingCollision, DoRaymanCollisionDefault, DoTntClownRaymanZDD, ObjectUTurnDefault}, // 116: TNT clown
    {DO_CLOWN_TNT2_COMMAND, DoTntClownPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 117: TNT clown burst
    {DO_CLOWN_TNT3_COMMAND, DoTntClownPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 118: TNT clown kamikaze
    {DO_ONE_CMD, DoPoingCollisionDefault, BombExplosion, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 119: clown TNT
    {DO_BAT_COMMAND, DO_BAT_POING_COLLISION, DoRaymanCollisionDefault, BAT_ray_in_zone, ObjectUTurnDefault}, // 120: red drummer
    {DO_ECLAIR_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 121: Mr Stone flame
    {DO_STONEDOG_COMMAND, DoStoneDogPoingCollision, DoRaymanCollisionDefault, DoStoneDogRaymanZDD, ObjectUTurnDefault}, // 122: Mr Stone stone dog
    {DO_BLKTOON_COMMAND, DoBlackToonPoingCollision, DoRaymanCollisionDefault, DoBlackToonRaymanZDD, blkUTurn}, // 123: antitoon
    {DO_ONE_CMD, DoPoingCollisionDefault, DoPancarteRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 124: return sign
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 125: candy platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 126: fork
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 127: swiss knife
    {DO_TIRE_BOUCHON_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 128: corkscrew
    {DO_PETIT_COUTEAU_COMMAND, DoPetitCouteauPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 129: small knife
    {DO_BLKTOON_EYES_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 130: antitoon eyes
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 131: mallets spring
    {DO_POISSON_BLEU_CMD, DoPoissonBleuPoingCollision, DoRaymanCollisionDefault, DoPoissonBleuRaymanZDD, ObjectUTurnDefault}, // 132: large mouthed fish
    {DO_ONE_CMD, DoPoingCollisionDefault, DoSuperHelicoRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 133: supercopter
    {DO_FALLING_OBJ_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 134: plum2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 135: flame Mr Stone
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 136: tiny Rayman
    {DO_REDUCTEUR, DoPoingCollisionDefault, DoReducteurRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 137: fairy
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 138: sprinkler (4 heads)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 139: maracas flying
    {DO_MOVING_PLATFORM_COMMAND, DoPTGRAPPINPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 140: ring
    {DO_ONE_CMD, DoPoingCollisionDefault, DoNeigeRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 141: snow
    {DO_ONE_CMD, DoOneUpPoingCollision, DoOneUpRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 142: extralife
    {DO_NOVA2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 143: item blink
    {DO_ONE_PINK_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 144: lidolpink2
    {DO_ROLL_EYES, DoKillingEyesPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 145: bad eyes
    {DO_FLASH_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 146: eyes bolt
    {DO_MST_SCROLL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_MST_CHANGE_COMMAND, ObjectUTurnDefault}, // 147: moskito huge spiky fruit
    {DO_ONE_CMD, DoPoingCollisionDefault, DoGrapBonusRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 148: grap bonus
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 149: treble clef
    {DO_SKO, DO_SKO_HIT, DoRaymanCollisionDefault, SKO_ray_in_zone, ObjectUTurnDefault}, // 150: Mr Skops
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 151: bullet
    {DO_CCL_COMMAND, DoCaisseClairePoingCollision, DoRaymanCollisionDefault, DoCaisseClaireRaymanZDD, ObjectUTurnDefault}, // 152: snare drum
    {DO_FEE, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoFeeRaymanZDD, ObjectUTurnDefault}, // 153: Betilla
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 154: sprinkler (4 heads, moving)
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 155: sprinkler (3 heads)
    {DO_WLKNOT_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 156: walk note 1
    {DO_EAU_QUI_MONTE, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 157: water
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 158: palette change
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 159: tibetan 6 platforms
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 160: tibetan 2 platforms
    {DO_ONE_CMD, DoPoingCollisionDefault, DoWizRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 161: ting
    {DO_MOVING_WITH_INDICATOR_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 162: flying saucer platform
    {DO_IDC_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 163: circuit box
    {DO_ONE_CMD, DoPoingCollisionDefault, DoGeneratingDoorRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 164: gendoor
    {DO_PTI_ESQUIVE, DoBadGuy23PoingCollision, DoRaymanCollisionDefault, DoBadGuy23RaymanZDD, ObjectUTurnDefault}, // 165: evil livingstone
    {DO_ONE_CMD, DO_LEV_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 166: lever
    {DO_FALLING_OBJ_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 167: plum 3
    {DO_2_PARTS_CYMBAL, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 168: cymbal1 single
    {DO_2_PARTS_CYMBAL, DoPoingCollisionDefault, DO_COLL_RAY_CYMBALE, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 169: cymbal2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 170: Mr Skops beam
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 171: Rayman Bzzit cutscene
    {DO_STONEWOMAN_COMMAND, DO_PNG_COLL_STONEWOMAN, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 172: stoneman (aggressive)
    {doShipCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 173: viking ship
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 174: space pirate (left)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 175: punaise platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 176: pencil pointing down
    {DO_ONE_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 177: yin1
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 178: pencil pointing down (moving)
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 179: pencil pointing down (wave)
    {DO_SAXO2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 180: Mr Sax (chasing)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_SAXO_MARCHE, ObjectUTurnDefault}, // 181: Mr Sax walk speed
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 182: wrong note stationary punchable
    {DO_SAXO3_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_SAXO3_DEBUT, ObjectUTurnDefault}, // 183: Mr Sax (first encounter)
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 184: space pirate (right)
    {DO_WLKNOT_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 185: walk note 2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 186: pirate pan flying away
    {DO_PMA_COMMAND, DO_PMA_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 187: Viking Mama
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 188: red crystal platform
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 189: red crystal platform (moving)
    {DO_COU_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 190: Viking Mama sword
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 191: pencil (falling)
    {mereDenisBigLaserCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 192: Space Mama beam
    {mereDenisBombCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 193: Space Mama pot
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 194: Space Mama pot part
    {DO_SPIDER_PLAFOND, DO_SPIDER_PLAFOND_POING_COLLISION, DoRaymanCollisionDefault, SPIDER_PLAFOND_REACT, ObjectUTurnDefault}, // 195: spider (ceiling)
    {DO_DARD_PLAFOND_ALWAYS, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 196: spider homing dart
    {DO_MEDAILLON_TOON, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 197: medaillon
    {DO_BBMONT2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 198: Mr Stone (chase)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_BBMONT2_MARCHE, ObjectUTurnDefault}, // 199: Mr Stone (chase, walking)
    {DO_BBMONT3_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 200: BB13
    {DO_BBMONT4_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 201: BB14
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 202: stars on Rayman
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 203: washing machine 2
    {doBlackRaymanCommand, DoPoingCollisionDefault, doBlKRayRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 204: dark Rayman
    {DO_ONE_CMD, DoPoingCollisionDefault, doBlKRayRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 205: dark Rayman fist
    {DO_PIEDS_RAYMAN, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 206: Rayman's feet
    {DO_POELLE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 207: frying pan
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 208: dust circle
    {DoFlammeCommand, DoPoingCollisionDefault, DoFlammeRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 209: Mr Dark flame (left)
    {DoFlammeCommand, DoPoingCollisionDefault, DoFlammeRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 210: Mr Dark flame (right)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 211: Mr Stone swinging totem
    {DO_DARK_COMMAND, DO_DARK_POING_COLLISION, DoRaymanCollisionDefault, DO_DARK_REACT, ObjectUTurnDefault}, // 212: Mr Dark
    {doMereDenisCommand, doMereDenisHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 213: Space Mama (with washing machine)
    {DO_BOUEE_JOE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 214: buoy
    {DO_DARK_SORT_COMMAND, DoPoingCollisionDefault, DO_DARK_SORT_COLLISION, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 215: Mr Dark spell fireball
    {DO_ENSEIGNE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 216: Eat at Joe's sign
    {DO_MITE2_COMMAND, DoMite2PoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 217: moth 2
    {doMOSAMScommand, DoMOSAMSPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 218: Spaxkito
    {DO_CORDE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 219: flat stone rope
    {DO_PAC_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 220: flat stone (with ropes)
    {DO_CFUMEE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 221: rope dust
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 222: flat stone (detached rope)
    {doSTOSKOcommand, DoSTOSKOPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 223: Stoneskops
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 224: Stoneskops claw
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 225: pan pirate (45 degrees)
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 226: pan pirate 2 (45 degrees)
    {doMoskitoCommand, doMoskitoHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 227: Moskito
    {DO_ONE_CMD, DoPrisePoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 228: electric plug
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 229: punaise (facing down)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 230: punaise (horizontal)
    {doBBF2command, DO_HYB_BBF2_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 231: Moskitomama (right)
    {doBBF2command, DO_HYB_BBF2_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 232: Moskitomama (left)
    {DO_HYB_BBF2_LAS, DoPoingCollisionDefault, DoHybBBF2LasRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 233: Moskitomama beam
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 234: lava
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 235: punaise
    {DO_ONE_CMD, DoPoingCollisionDefault, DoAnnuleDarkSortRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 236: cancel Mr Dark spell
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 237: eraser
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 238: Betilla text fist
    {DO_ONE_PAR_COMMAND, DO_PAR_POING_COLLISION, DoRaymanCollisionDefault, PAR_REACT_TO_RAY_IN_ZONE, ObjectUTurnDefault}, // 239: viking ship nest
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 240: pencil (pointing up)
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 241: pencil (pointing up, moving)
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 242: pencil (pointing up, wave)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 243: stylo
    {mereDenisBigLaserCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 244: Space Mama second beam
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 245: Joe's beach
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 246: Mr Dark separate rope
    {DO_VITRAIL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 247: vitrail
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 248: Mr Skops claw
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 249: theatre curtain
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 250: punaise (up down)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 251: theatre water (front)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 252: theatre water (back)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 253: planches
    {DO_SLOPEY_PLAT_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 254: slopey plat
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 255: invalid
    {DO_CB_BRIK_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 256: breakout brick 20
    {DO_CB_BALL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 257: breakout game
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 258: nougat
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 259: EDU letter
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 260: EDU digit
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 261: EDU road sign
};
