# Rayman state meanings

State labels apply to Rayman only. Main/sub values are not shared behaviour enums for all objects. ALLFIX default bank0 is proven by descriptor tail binding; mounted/tiny template swaps can change eta tables. UNKNOWN rows retain null names; table inclusion is not reachability proof.

| main | Observed family | Level |
|---|---|---|
| 0 | ground stationary/action family; motion can persist through inertia | PROVEN |
| 1 | ground locomotion | PROVEN |
| 2 | airborne | PROVEN |
| 3 | mixed scripted/action/death family; no single universal action name | PROVEN |
| 4 | vine attachment/climbing | PROVEN |
| 5 | ledge hanging | PROVEN |
| 6 | mounted motion | PROVEN |
| 7 | grapple swinging | PROVEN |

| main:sub | Meaning | Level | DOS consumers/writers |
|---|---|---|---|
| 0:0 | stationary idle | STRONG | RAY_STOP 1:5d724, RAY_RESPOND_TO_NOTHING 1:5eb40 |
| 0:8 | landing | PROVEN | RAY_IN_THE_AIR 1:5fa44, rayMayLandOnAnObject 1:5c224 |
| 0:11 | fist preparation | STRONG | RAY_RESPOND_TO_FIRE0 1:5f1c8, RAY_PREPARE_FIST 1:56af0 |
| 0:12 | fist charging | PROVEN | DO_RAYMAN 1:6133c, RAY_GROW_FIST 1:56bd0 |
| 0:13 | fist throw animation | STRONG | RAY_THROW_FIST 1:5676c |
| 0:15 | stationary crouch | STRONG | RAY_RESPOND_TO_DOWN 1:5e030, RAY_RESPOND_TO_NOTHING 1:5eb40, GET_RAY_ZDC 1:0dcfc |
| 0:40 | ground slide | STRONG | RAY_SURF 1:60964, DO_SURF_CHANGE_HAIRS 1:60a8c |
| 0:41 | enter ground slide | STRONG | RAY_SURF 1:60964 |
| 0:42 | leave ground slide | STRONG | RAY_SURF 1:60964 |
| 0:43 | fast-air landing transition | STRONG | RAY_IN_THE_AIR 1:5fa44 |
| 0:47 | stop forward crawl | STRONG | RAY_STOP 1:5d724, RAY_RESPOND_TO_DIR 1:5e548 |
| 0:48 | stop backward crawl | STRONG | RAY_STOP 1:5d724, RAY_RESPOND_TO_DIR 1:5e548 |
| 0:50 | crouched direction change | STRONG | RAY_RESPOND_TO_DIR 1:5e548 |
| 0:51 | crouched facing turn | STRONG | RAY_RESPOND_TO_DIR 1:5e548 |
| 0:61 | hurt while crouched under blocking ceiling | PROVEN | RAY_HIT 1:1dbdc |
| 1:0 | ordinary walking | STRONG | RAY_RESPOND_TO_DIR 1:5e548 |
| 1:3 | running | PROVEN | RAY_RESPOND_TO_BUTTON3 1:5f0b0, DO_RAYMAN 1:6133c |
| 1:4 | locomotion turn | STRONG | RAY_RESPOND_TO_DIR 1:5e548 |
| 1:7 | running after fast-air landing | STRONG | RAY_IN_THE_AIR 1:5fa44 |
| 1:8 | enter forward crawl | STRONG | RAY_RESPOND_TO_DIR 1:5e548 |
| 1:9 | enter backward crawl | STRONG | RAY_RESPOND_TO_DIR 1:5e548, RAY_SWIP 1:5d2e8 |
| 1:10 | forward crawl | STRONG | RAY_RESPOND_TO_DIR 1:5e548, RAY_SWIP 1:5d2e8 |
| 1:11 | backward crawl | STRONG | RAY_RESPOND_TO_DIR 1:5e548, RAY_SWIP 1:5d2e8 |
| 2:0 | initial rising jump | PROVEN | ray_jump 1:5cd40, RAY_IN_THE_AIR 1:5fa44 |
| 2:1 | ordinary descending air state | STRONG | RAY_IN_THE_AIR 1:5fa44, RAY_TOMBE 1:5df28 |
| 2:2 | airborne continuation after departure or grapple release | STRONG | RAY_IN_THE_AIR 1:5fa44, RAY_FIN_BALANCE 1:5f7d0 |
| 2:3 | ordinary helicopter | PROVEN | RAY_HELICO 1:5d7d4 |
| 2:5 | ordinary helicopter stop transition | PROVEN | RAY_HELICO 1:5d7d4 |
| 2:8 | hit knockback | PROVEN | RAY_HIT 1:1dbdc, RAY_HURT 1:60408 |
| 2:9 | below-map death fall | PROVEN | RAY_DEAD 1:601a8 |
| 2:14 | super-helicopter startup | PROVEN | RAY_HELICO 1:5d7d4 |
| 2:15 | active super-helicopter | PROVEN | RAY_HELICO 1:5d7d4, DO_CORDE_COMMAND 1:20ea8 |
| 2:17 | fast-air/rolling jump family | STRONG | ray_jump 1:5cd40, Reset_air_speed 1:5caf8 |
| 2:18 | fast-air/rolling continuation | STRONG | Reset_air_speed 1:5caf8, RAY_IN_THE_AIR 1:5fa44 |
| 2:19 | fast-air descending continuation | PROVEN | RAY_IN_THE_AIR 1:5fa44 |
| 2:24 | ordinary support-departure transition | STRONG | RAY_TOMBE 1:5df28, RAY_IN_THE_AIR 1:5fa44 |
| 2:31 | cliff hazard fall | PROVEN | COLL_RAY_BLK_MORTEL 1:1da98 |
| 2:32 | fast support-departure transition | STRONG | RAY_TOMBE 1:5df28, RAY_IN_THE_AIR 1:5fa44 |
| 3:0 | knockout | PROVEN | RAY_KO 1:1dbac |
| 3:6 | crouch entry transition | STRONG | RAY_RESPOND_TO_DOWN 1:5e030 |
| 3:16 | plant magic seed | STRONG | RAY_RESPOND_TO_BUTTON3 1:5f0b0, DO_RAYMAN 1:6133c |
| 3:20 | scripted horizontal target wait | PROVEN | remoteControlRay 1:5f95c, DO_RAYMAN 1:6133c |
| 3:21 | scripted horizontal target approach | PROVEN | remoteControlRay 1:5f95c, DO_RAYMAN 1:6133c |
| 3:22 | drowning | PROVEN | rayfallsinwater 1:6010c, RAY_DEAD 1:601a8 |
| 3:32 | lethal spike contact | PROVEN | COLL_RAY_BLK_MORTEL 1:1da98, RAY_DEAD 1:601a8 |
| 4:0 | stationary on vine | PROVEN | RAY_RESPOND_TO_NOTHING 1:5eb40 |
| 4:1 | vine attachment transition | STRONG | IS_RAY_ON_LIANE 1:5c138 |
| 4:2 | climb vine upward | PROVEN | RAY_RESPOND_TO_UP 1:5e370 |
| 4:3 | climb vine downward | PROVEN | RAY_RESPOND_TO_DOWN 1:5e030 |
| 5:0 | ledge hanging | PROVEN | Make_Ray_Hang 1:5db1c |
| 6:8 | mounted hit response | PROVEN | RAY_HIT 1:1dbdc |
| 7:0 | grapple attachment approach | PROVEN | SET_RAY_BALANCE 1:5f29c, RAY_GOING_BALANCE 1:5f328 |
| 7:1 | grapple swinging | PROVEN | RAY_BALANCE 1:5f500 |
