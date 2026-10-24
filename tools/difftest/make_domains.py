"""Writes the per-function domain files used by difftest.py (evidence: difftriage/TRIAGE.md and STRICT setters)."""
import json
from pathlib import Path

OUT = Path(__file__).resolve().parent / 'domains'
OUT.mkdir(exist_ok=True)


def w(unit, fn, evidence, constraints):
    (OUT / f'{unit}_{fn}.json').write_text(json.dumps({'unit': unit, 'function': fn, 'evidence': evidence,
                                                       'constraints': constraints}, indent=2) + '\n', encoding='utf-8')


LANG = {'loc': {'global': 'language', 'size': 1}, 'values': [0, 1, 2],
        'evidence': 'STRICT INIT_OPTIONS_CHOICE / InitParamGraphicsScreen switch (language) handle cases 0, 1, 2 only '
                    '(src/menu.c:959..963, 2005..2009)'}


def rows(first, nbre, exit_, pos_lo, pos_hi):
    return [{'loc': {'global': 'first_option', 'size': 1}, 'values': [first]},
            {'loc': {'global': 'nbre_options', 'size': 2}, 'values': [nbre]},
            {'loc': {'global': 'option_exit', 'size': 2}, 'values': [exit_]},
            {'loc': {'global': 'position', 'size': 2}, 'range': [pos_lo, pos_hi]}]


w('jumelle', 'RecaleRayPosInJumelle',
  'Binocular geometry is only produced by DefaultJumelleVariable (RayonJumelle=25), ChangeDeltaSizeJumelle (clamp 25..64) and '
  'ChangeJumelleVariable (LargeurJumelle=7*r/2, HauteurJumelle=2*r), all STRICT (src/jumelle.c:329..356, 516..517). '
  'difftriage case 2.',
  [{'loc': {'global': 'RayonJumelle', 'size': 4}, 'range': [25, 64],
    'evidence': 'STRICT ChangeDeltaSizeJumelle clamps to 25..64; DefaultJumelleVariable sets 25'},
   {'loc': {'global': 'LargeurJumelle', 'size': 4}, 'range': [87, 224], 'evidence': 'STRICT ChangeJumelleVariable: 7*r/2'},
   {'loc': {'global': 'HauteurJumelle', 'size': 4}, 'range': [50, 128], 'evidence': 'STRICT ChangeJumelleVariable: 2*r'}])

w('ray', 'ray_inertia_speed',
  'num_world is a loaded level world 1..6 (STRICT INIT_WORLD_INFO / DEPART_LEVEL, src/worldmap.c:726, src/cheat.c:589). Nonzero '
  'slope acceleration a2 comes only from RAY_SWIP slope types 18..23 or type-254 platforms, and the 12 shipped CAVE (world 5) '
  'levels contain neither (difftriage evidence/cave_domain.json, STRONG). difftriage case 3.',
  [{'any_of': [[{'loc': {'global': 'num_world', 'size': 2}, 'values': [1, 2, 3, 4, 6]}],
               [{'loc': {'global': 'num_world', 'size': 2}, 'values': [5]}, {'loc': {'arg': 1}, 'values': [0]}]],
    'evidence': 'worlds 1..6; world 5 only with a2 == 0 (no slopes / type-254 platforms in shipped CAVE levels)'}])

w('ray', 'RAY_SWIP',
  'RAY_SWIP is called from the ground/air/state-3 branches of the player state machine (src/ray.c:1419,1434,1489,1555,1595,1601,'
  '1689,1756,2920); main_etat 255 is not a player state. Domain restricted to the states the triage verified (0..3). '
  'difftriage case 4.',
  [{'loc': {'global': 'ray', 'offset': '0x6d', 'size': 1}, 'values': [0, 1, 2, 3],
    'evidence': 'ray.main_etat (obj_t +0x6d) in the calling branches'}])

w('moteur', 'DO_MOTEUR2',
  'Player scale is initialised/reset to 0 or 256 (src/objinit.c:440, src/worldmap.c:743,750) and only changed here by writing 256 '
  'or subtracting 5 with a zero clamp: 0..256 (STRONG). difftriage case 5.',
  [{'loc': {'global': 'ray', 'offset': '0x5a', 'size': 2}, 'range': [0, 256], 'evidence': 'ray.scale (obj_t +0x5a)'}])

w('menu', 'INIT_AFFICHE_ECRAN_OPTIONS',
  'Row range set by STRICT INIT_OPTIONS_CHOICE: first_option=0, (nbre_options, option_exit, position)=(5,5,5) or (6,6,6) '
  '(src/menu.c:943..978); DO_COMMANDE moves position inside the rows. difftriage case 6.',
  [{'any_of': [rows(0, 5, 5, 0, 5), rows(0, 6, 6, 0, 6)], 'evidence': 'INIT_OPTIONS_CHOICE OptionGame / !OptionGame branches'},
   LANG])

w('menu', 'AFFICHE_ECRAN_OPTIONS',
  'STRICT INIT_OPTIONS_CHOICE writes max_sound=20, ecart_barre=4 and the row range (src/menu.c:943..978); the sound volume is '
  '0..max_sound. difftriage case 7.',
  [{'any_of': [rows(0, 5, 5, 0, 5), rows(0, 6, 6, 0, 6)], 'evidence': 'INIT_OPTIONS_CHOICE row ranges'},
   {'loc': {'global': 'max_sound', 'size': 2}, 'values': [20], 'evidence': 'INIT_OPTIONS_CHOICE max_sound = 20'},
   {'loc': {'global': 'ecart_barre', 'size': 2}, 'values': [4], 'evidence': 'INIT_OPTIONS_CHOICE ecart_barre = 4'},
   {'loc': {'global': 'options', 'offset': 2, 'size': 2}, 'range': [0, 20],
    'evidence': 'options.sound_volume (options_t +2, src/menu_local.h:8) in 0..max_sound'},
   LANG])

w('menu', 'INIT_AFFICHE_KEY_SCREEN',
  'STRICT INIT_KEY_SCREEN sets position=nbre_options=option_exit=8 (src/menu.c:1297..1311); first_option stays 0. '
  'difftriage case 8.',
  [{'any_of': [rows(0, 8, 8, 0, 8)], 'evidence': 'INIT_KEY_SCREEN'}, LANG])

w('menu', 'INIT_AFFICHE_PAD_SCREEN',
  'STRICT INIT_PAD_SCREEN sets first_option=0, position=nbre_options=option_exit=5 (src/menu.c:1723..1739). difftriage case 9.',
  [{'any_of': [rows(0, 5, 5, 0, 5)], 'evidence': 'INIT_PAD_SCREEN'}, LANG])

w('menu', 'INIT_AFFICHE_GRAPHIC_SCREEN',
  'STRICT InitParamGraphicsScreen (src/menu.c:1960..2040) selects one of these row spans (after its --nbre_options where present); '
  'position starts inside and DO_COMMANDE keeps it inside [first_option, option_exit]. difftriage case 10.',
  [{'any_of': [rows(2, 3, 5, 2, 5), rows(3, 2, 5, 3, 5), rows(1, 6, 7, 1, 7), rows(3, 4, 6, 3, 6), rows(1, 5, 6, 1, 6)],
    'evidence': 'InitParamGraphicsScreen branches'}, LANG])

w('menu', 'INIT_AFFICHE_VIDEOMODE_SCREEN',
  'STRICT INIT_VIDEOMODE_SCREEN sets first_option=0, position=nbre_options=option_exit=3 (src/menu.c:2414..2433). '
  'difftriage case 11.',
  [{'any_of': [rows(0, 3, 3, 0, 3)], 'evidence': 'INIT_VIDEOMODE_SCREEN'}, LANG])

w('effect50350', 'effect_activate',
  'effect_helper_50738 allocates the 0x38-byte state, a 0x58-byte vertices block and a 0x78-byte transformed block, checks each, '
  'sets screen_a=EffetBufferNormal, screen_b=EffetBufferNormal+0xc800 and only then calls effect_activate '
  '(src/effect50350.c:140..170): live, disjoint allocations. difftriage case 13.',
  [{'loc': {'global': 'EffetBufferNormal', 'size': 4}, 'fresh': {'size': '0x19000'},
    'evidence': 'effect buffer holding two 0xc800 screens'},
   {'loc': {'global': 'dword_E4DA8', 'size': 4},
    'fresh': {'size': '0x38', 'fields': {'0x20': {'fresh': {'size': '0xfa00'}}, '0x24': {'fresh': {'size': '0xfa00'}},
                                          '0x28': {'copy_global': 'EffetBufferNormal'},
                                          '0x2c': {'copy_global': 'EffetBufferNormal', 'add': '0xc800'},
                                          '0x30': {'fresh': {'size': '0x58'}}, '0x34': {'fresh': {'size': '0x78'}}}},
    'evidence': 'effect3d_t: source +0x20, destination +0x24, screen_a +0x28, screen_b +0x2c, vertices +0x30, '
                'transformed +0x34 (src/effect_types.h:46..70)'}])

if __name__ == '__main__':
    print(sorted(p.name for p in OUT.glob('*.json')))
