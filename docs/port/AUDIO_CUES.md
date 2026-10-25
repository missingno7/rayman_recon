# Sound cues and CD routing

Numeric identities are retained. A trigger is not a claimed recording title. Effects are U8 mono, speech S16LE mono;10000Hz reference, Q16 source step, byte-start plus byte-length loops follow PLATFORM_OPEN item3.

Game cue policy bits:01 initial distance attenuation;02 dynamic attenuation together with 01;04 same-cue retrigger;08 stop previous owner cue on replacement;10 working/dynamic/cleanup voice. These are independent bits, not numeric priorities. HMI sample IDs1(effects)/2(speech) are not priority ranks. KeyOn preserves backend failure and handle predicates.

Pan: narrow signed16 screen_x+offset_bx, map[-96,416] by(x+96)>>2 and cap0..127. Distance volume:127-((abs(animation_center_x-Rayman_anchor_x)+abs(animation_center_y-(Rayman_anchor_y-40)))>>2), with the DOS signed16 narrowings and lower clamp0. Live gain is(master*distance*cue_gain)>>14; unowned immediate path is(master*cue_gain)>>7, pan64. KeyVol scales to 32767/127 and 65535/127. Exact pan table is retained.

| Cue | Bank0=fixed/1=world | Program | Note | Tone (ignored by KeyOn) | Gain | Policy hex | Delay | Followup | Followup delay |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | 0 | 255 | 0 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 1 | 0 | 15 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 2 | 0 | 0 | 53 | 0 | 90 | 0x1b | 0 | 0 | 0 |
| 3 | 0 | 255 | 53 | 0 | 90 | 0x7 | 2 | 0 | 0 |
| 4 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 5 | 0 | 5 | 53 | 0 | 115 | 0x3 | 0 | 0 | 0 |
| 6 | 0 | 3 | 55 | 0 | 50 | 0x1b | 0 | 0 | 0 |
| 7 | 0 | 255 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 8 | 0 | 8 | 53 | 0 | 55 | 0x3 | 0 | 0 | 0 |
| 9 | 0 | 12 | 53 | 0 | 50 | 0x7 | 0 | 0 | 0 |
| 10 | 0 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 11 | 0 | 6 | 72 | 0 | 90 | 0x7 | 0 | 0 | 0 |
| 12 | 0 | 8 | 53 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 13 | 0 | 1 | 65 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 14 | 0 | 7 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 15 | 0 | 13 | 58 | 0 | 80 | 0x7 | 20 | 0 | 0 |
| 16 | 0 | 4 | 54 | 0 | 80 | 0x7 | 28 | 0 | 0 |
| 17 | 0 | 31 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 18 | 0 | 30 | 55 | 0 | 80 | 0x3 | 1 | 0 | 0 |
| 19 | 0 | 16 | 62 | 0 | 80 | 0x7 | 2 | 0 | 0 |
| 20 | 0 | 21 | 55 | 0 | 65 | 0x3 | 0 | 0 | 0 |
| 21 | 0 | 22 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 22 | 0 | 23 | 49 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 23 | 0 | 25 | 60 | 0 | 60 | 0xf | 34 | 0 | 0 |
| 24 | 0 | 24 | 55 | 0 | 75 | 0x7 | 0 | 0 | 0 |
| 25 | 1 | 76 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 26 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 27 | 1 | 255 | 56 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 28 | 1 | 71 | 53 | 0 | 60 | 0x3 | 0 | 0 | 0 |
| 29 | 1 | 72 | 53 | 0 | 65 | 0x3 | 0 | 0 | 0 |
| 30 | 1 | 73 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 31 | 1 | 74 | 53 | 0 | 80 | 0xf | 0 | 0 | 0 |
| 32 | 1 | 75 | 60 | 0 | 80 | 0xb | 0 | 0 | 0 |
| 33 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 34 | 1 | 104 | 53 | 0 | 45 | 0x3 | 0 | 0 | 0 |
| 35 | 1 | 79 | 55 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 36 | 1 | 80 | 54 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 37 | 1 | 81 | 57 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 38 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 39 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 40 | 0 | 26 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 41 | 1 | 85 | 53 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 42 | 1 | 93 | 53 | 0 | 40 | 0x7 | 1 | 42 | 65 |
| 43 | 1 | 87 | 53 | 0 | 80 | 0x3 | 7 | 0 | 0 |
| 44 | 1 | 88 | 53 | 0 | 80 | 0x7 | 2 | 0 | 0 |
| 45 | 1 | 89 | 53 | 0 | 100 | 0x3 | 0 | 0 | 0 |
| 46 | 1 | 90 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 47 | 1 | 91 | 53 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 48 | 1 | 92 | 53 | 0 | 80 | 0x1b | 0 | 0 | 0 |
| 49 | 1 | 70 | 53 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 50 | 0 | 27 | 56 | 0 | 80 | 0x7 | 30 | 0 | 0 |
| 51 | 0 | 5 | 51 | 0 | 80 | 0x3 | 15 | 0 | 0 |
| 52 | 1 | 96 | 57 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 53 | 0 | 1 | 53 | 0 | 90 | 0x7 | 2 | 0 | 0 |
| 54 | 1 | 86 | 56 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 55 | 0 | 39 | 60 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 56 | 0 | 39 | 65 | 0 | 80 | 0x7 | 1 | 56 | 40 |
| 57 | 0 | 39 | 70 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 58 | 0 | 21 | 53 | 0 | 50 | 0x7 | 0 | 0 | 0 |
| 59 | 0 | 22 | 53 | 0 | 60 | 0x3 | 0 | 0 | 0 |
| 60 | 0 | 32 | 53 | 0 | 30 | 0x3 | 0 | 0 | 0 |
| 61 | 0 | 33 | 53 | 0 | 60 | 0x3 | 0 | 0 | 0 |
| 62 | 1 | 106 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 63 | 1 | 99 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 64 | 1 | 107 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 65 | 1 | 107 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 66 | 1 | 73 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 67 | 1 | 84 | 53 | 0 | 70 | 0x3 | 0 | 0 | 0 |
| 68 | 0 | 12 | 53 | 0 | 40 | 0x7 | 0 | 0 | 0 |
| 69 | 0 | 11 | 53 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 70 | 1 | 94 | 58 | 0 | 80 | 0xb | 40 | 0 | 0 |
| 71 | 1 | 95 | 51 | 0 | 127 | 0xb | 0 | 0 | 0 |
| 72 | 1 | 96 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 73 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 74 | 1 | 255 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 75 | 1 | 105 | 53 | 0 | 45 | 0x3 | 0 | 0 | 0 |
| 76 | 1 | 97 | 53 | 0 | 80 | 0xb | 0 | 0 | 0 |
| 77 | 0 | 95 | 53 | 0 | 95 | 0x7 | 0 | 0 | 0 |
| 78 | 0 | 21 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 79 | 0 | 22 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 80 | 1 | 82 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 81 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 82 | 1 | 84 | 53 | 0 | 80 | 0x7 | 10 | 0 | 0 |
| 83 | 1 | 51 | 53 | 0 | 95 | 0x7 | 0 | 0 | 0 |
| 84 | 1 | 52 | 54 | 0 | 70 | 0x0 | 0 | 0 | 0 |
| 85 | 1 | 70 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 86 | 1 | 71 | 53 | 0 | 40 | 0x7 | 0 | 0 | 0 |
| 87 | 1 | 72 | 53 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 88 | 1 | 73 | 56 | 0 | 127 | 0xf | 30 | 0 | 0 |
| 89 | 1 | 74 | 53 | 0 | 80 | 0x7 | 54 | 0 | 0 |
| 90 | 1 | 75 | 53 | 0 | 80 | 0x1f | 0 | 0 | 0 |
| 91 | 1 | 61 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 92 | 1 | 62 | 53 | 0 | 60 | 0x7 | 0 | 0 | 0 |
| 93 | 1 | 54 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 94 | 1 | 55 | 53 | 0 | 50 | 0x3 | 0 | 0 | 0 |
| 95 | 1 | 56 | 48 | 0 | 127 | 0x3 | 0 | 0 | 0 |
| 96 | 1 | 77 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 97 | 1 | 78 | 53 | 0 | 115 | 0x3 | 0 | 0 | 0 |
| 98 | 1 | 79 | 53 | 0 | 100 | 0x3 | 0 | 0 | 0 |
| 99 | 1 | 50 | 53 | 0 | 45 | 0x1b | 0 | 0 | 0 |
| 100 | 1 | 80 | 53 | 0 | 115 | 0x3 | 40 | 0 | 0 |
| 101 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 102 | 1 | 255 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 103 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 104 | 0 | 97 | 53 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 105 | 0 | 98 | 53 | 0 | 80 | 0x7 | 30 | 0 | 0 |
| 106 | 0 | 99 | 53 | 0 | 80 | 0x7 | 60 | 0 | 0 |
| 107 | 0 | 100 | 55 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 108 | 0 | 101 | 53 | 0 | 80 | 0x7 | 60 | 0 | 0 |
| 109 | 0 | 102 | 53 | 0 | 80 | 0x7 | 40 | 0 | 0 |
| 110 | 1 | 71 | 53 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 111 | 1 | 70 | 55 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 112 | 1 | 71 | 49 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 113 | 1 | 103 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 114 | 1 | 75 | 53 | 0 | 115 | 0x1b | 0 | 0 | 0 |
| 115 | 1 | 94 | 50 | 0 | 127 | 0x3 | 0 | 0 | 0 |
| 116 | 1 | 72 | 51 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 117 | 1 | 73 | 53 | 0 | 80 | 0x3 | 20 | 0 | 0 |
| 118 | 1 | 255 | 57 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 119 | 1 | 75 | 53 | 0 | 120 | 0xf | 60 | 0 | 0 |
| 120 | 1 | 76 | 55 | 0 | 127 | 0x7 | 1 | 120 | 35 |
| 121 | 1 | 77 | 53 | 0 | 110 | 0x7 | 0 | 0 | 0 |
| 122 | 1 | 78 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 123 | 1 | 80 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 124 | 1 | 79 | 56 | 0 | 55 | 0x7 | 0 | 0 | 0 |
| 125 | 1 | 81 | 55 | 0 | 80 | 0x1b | 9 | 0 | 0 |
| 126 | 1 | 82 | 63 | 0 | 55 | 0x3 | 0 | 0 | 0 |
| 127 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 128 | 1 | 84 | 62 | 0 | 45 | 0x1b | 0 | 0 | 0 |
| 129 | 1 | 85 | 57 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 130 | 1 | 86 | 60 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 131 | 1 | 87 | 53 | 0 | 90 | 0x7 | 0 | 0 | 0 |
| 132 | 1 | 88 | 53 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 133 | 0 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 134 | 0 | 19 | 53 | 0 | 37 | 0x7 | 0 | 0 | 0 |
| 135 | 0 | 15 | 53 | 0 | 65 | 0xb | 10 | 0 | 0 |
| 136 | 0 | 36 | 53 | 0 | 90 | 0x3 | 0 | 0 | 0 |
| 137 | 0 | 34 | 53 | 0 | 80 | 0xb | 11 | 137 | 21 |
| 138 | 0 | 35 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 139 | 1 | 98 | 53 | 0 | 45 | 0x7 | 0 | 0 | 0 |
| 140 | 1 | 98 | 55 | 0 | 45 | 0x7 | 0 | 0 | 0 |
| 141 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 142 | 1 | 73 | 53 | 0 | 80 | 0x7 | 31 | 0 | 0 |
| 143 | 1 | 74 | 50 | 0 | 80 | 0x7 | 25 | 0 | 0 |
| 144 | 1 | 75 | 65 | 0 | 80 | 0x7 | 30 | 0 | 0 |
| 145 | 0 | 103 | 53 | 0 | 100 | 0x7 | 0 | 0 | 0 |
| 146 | 1 | 76 | 55 | 0 | 30 | 0x7 | 0 | 0 | 0 |
| 147 | 1 | 77 | 59 | 0 | 40 | 0x7 | 0 | 0 | 0 |
| 148 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 149 | 1 | 79 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 150 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 151 | 1 | 81 | 53 | 0 | 80 | 0x7 | 40 | 0 | 0 |
| 152 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 153 | 1 | 83 | 61 | 0 | 120 | 0x3 | 75 | 0 | 0 |
| 154 | 1 | 84 | 53 | 0 | 45 | 0x3 | 0 | 0 | 0 |
| 155 | 1 | 85 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 156 | 1 | 86 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 157 | 1 | 51 | 53 | 0 | 95 | 0x3 | 50 | 0 | 0 |
| 158 | 1 | 87 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 159 | 1 | 88 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 160 | 1 | 89 | 53 | 0 | 85 | 0x3 | 0 | 0 | 0 |
| 161 | 1 | 90 | 53 | 0 | 55 | 0x3 | 0 | 0 | 0 |
| 162 | 1 | 72 | 60 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 163 | 1 | 72 | 60 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 164 | 1 | 73 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 165 | 1 | 51 | 60 | 0 | 70 | 0x7 | 0 | 0 | 0 |
| 166 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 167 | 1 | 63 | 53 | 0 | 110 | 0xf | 80 | 0 | 0 |
| 168 | 1 | 76 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 169 | 1 | 77 | 53 | 0 | 55 | 0x1b | 0 | 0 | 0 |
| 170 | 1 | 78 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 171 | 1 | 79 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 172 | 1 | 80 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 173 | 1 | 255 | 50 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 174 | 1 | 82 | 53 | 0 | 80 | 0x3 | 60 | 0 | 0 |
| 175 | 1 | 83 | 55 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 176 | 1 | 84 | 48 | 0 | 90 | 0x7 | 0 | 0 | 0 |
| 177 | 1 | 85 | 46 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 178 | 1 | 255 | 50 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 179 | 1 | 86 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 180 | 1 | 70 | 53 | 0 | 40 | 0x3 | 0 | 0 | 0 |
| 181 | 1 | 71 | 53 | 0 | 40 | 0x3 | 0 | 0 | 0 |
| 182 | 1 | 72 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 183 | 1 | 73 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 184 | 1 | 74 | 53 | 0 | 55 | 0x3 | 0 | 0 | 0 |
| 185 | 1 | 75 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 186 | 1 | 76 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 187 | 1 | 77 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 188 | 1 | 78 | 53 | 0 | 100 | 0x3 | 0 | 0 | 0 |
| 189 | 1 | 79 | 53 | 0 | 127 | 0x3 | 0 | 0 | 0 |
| 190 | 1 | 80 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 191 | 1 | 81 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 192 | 0 | 37 | 50 | 0 | 65 | 0x4 | 0 | 0 | 0 |
| 193 | 0 | 38 | 53 | 0 | 60 | 0x4 | 0 | 0 | 0 |
| 194 | 0 | 11 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 195 | 0 | 104 | 37 | 0 | 30 | 0x3 | 0 | 0 | 0 |
| 196 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 197 | 0 | 8 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 198 | 1 | 100 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 199 | 0 | 1 | 56 | 0 | 80 | 0x7 | 1 | 199 | 40 |
| 200 | 1 | 53 | 53 | 0 | 50 | 0x7 | 0 | 0 | 0 |
| 201 | 1 | 60 | 53 | 0 | 45 | 0x1b | 0 | 0 | 0 |
| 202 | 1 | 76 | 53 | 0 | 80 | 0x7 | 12 | 0 | 0 |
| 203 | 1 | 98 | 53 | 0 | 100 | 0x3 | 0 | 0 | 0 |
| 204 | 0 | 41 | 53 | 0 | 40 | 0x0 | 0 | 0 | 0 |
| 205 | 1 | 57 | 53 | 0 | 80 | 0x1f | 0 | 0 | 0 |
| 206 | 0 | 40 | 53 | 0 | 100 | 0x7 | 0 | 0 | 0 |
| 207 | 0 | 39 | 60 | 0 | 105 | 0x7 | 0 | 0 | 0 |
| 208 | 1 | 98 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 209 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 210 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 211 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 212 | 1 | 76 | 53 | 0 | 127 | 0x3 | 0 | 0 | 0 |
| 213 | 0 | 96 | 60 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 214 | 0 | 42 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 215 | 1 | 255 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 216 | 0 | 43 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 217 | 1 | 255 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 218 | 1 | 90 | 46 | 0 | 90 | 0x7 | 30 | 0 | 0 |
| 219 | 1 | 92 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 220 | 1 | 95 | 53 | 0 | 40 | 0x7 | 0 | 0 | 0 |
| 221 | 1 | 94 | 53 | 0 | 50 | 0x1b | 0 | 0 | 0 |
| 222 | 1 | 94 | 53 | 0 | 80 | 0x1f | 0 | 0 | 0 |
| 223 | 1 | 82 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 224 | 1 | 83 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 225 | 1 | 84 | 50 | 0 | 70 | 0x3 | 0 | 0 | 0 |
| 226 | 1 | 77 | 52 | 0 | 80 | 0x3 | 45 | 0 | 0 |
| 227 | 1 | 74 | 56 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 228 | 1 | 74 | 50 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 229 | 1 | 90 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 230 | 1 | 89 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 231 | 1 | 85 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 232 | 1 | 86 | 48 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 233 | 1 | 87 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 234 | 1 | 88 | 57 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 235 | 1 | 76 | 50 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 236 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 237 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 238 | 1 | 51 | 53 | 0 | 127 | 0x7 | 15 | 238 | 29 |
| 239 | 0 | 25 | 60 | 0 | 60 | 0xf | 70 | 23 | 15 |
| 240 | 0 | 21 | 65 | 0 | 50 | 0x3 | 0 | 0 | 0 |
| 241 | 0 | 22 | 65 | 0 | 60 | 0x3 | 0 | 0 | 0 |
| 242 | 0 | 93 | 53 | 0 | 55 | 0x7 | 0 | 0 | 0 |
| 243 | 1 | 94 | 53 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 244 | 1 | 94 | 53 | 0 | 127 | 0x4 | 0 | 0 | 0 |
| 245 | 0 | 92 | 53 | 0 | 40 | 0x1b | 0 | 0 | 0 |
| 246 | 1 | 95 | 55 | 0 | 127 | 0x4 | 0 | 0 | 0 |
| 247 | 1 | 55 | 53 | 0 | 80 | 0x7 | 0 | 0 | 0 |
| 248 | 1 | 97 | 70 | 0 | 55 | 0x7 | 0 | 0 | 0 |
| 249 | 1 | 97 | 60 | 0 | 50 | 0x7 | 0 | 0 | 0 |
| 250 | 1 | 103 | 60 | 0 | 80 | 0x3 | 7 | 0 | 0 |
| 251 | 1 | 101 | 58 | 0 | 80 | 0x3 | 20 | 70 | 20 |
| 252 | 1 | 102 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 253 | 0 | 19 | 53 | 0 | 80 | 0x3 | 0 | 0 | 0 |
| 254 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |
| 255 | 1 | 255 | 60 | 0 | 127 | 0x7 | 0 | 0 | 0 |

Delay units are map_time increments, not milliseconds. Followups arm once; preserve setvol’s mixed queued-index/pt_pile_snd stores. Program255 is disabled; PlaySnd also forces-1 for cues55/57/103. Fixed sample bank 2; world 1..6 banks4,6,5,3,1,0. Speech IDs1,6,2,7,4,3,8,5/9 select chunks8,7,5,1,0,2,6,3.

## CD world/level tables

| World | Tracks, level 1..22 | Duration seconds, level 1..22 |
|---|---|---|
| 1 | 14,14,9,14,14,5,14,9,14,14,14,14,14,14,14,5,9,15,15,15,15,15 | 293,293,81,293,293,151,293,81,293,293,293,293,293,293,293,151,81,70,70,70,70,70 |
| 2 | 17,17,17,17,17,17,17,17,17,17,9,17,17,17,7,7,15,15,0,0,0,0 | 315,315,315,315,315,315,315,315,315,315,81,315,315,315,149,149,70,70,0,0,0,0 |
| 3 | 16,16,16,16,16,16,16,16,16,6,9,15,15,0,0,0,0,0,0,0,0,0 | 206,206,206,206,206,206,206,206,206,148,81,70,70,0,0,0,0,0,0,0,0,0 |
| 4 | 13,13,13,4,13,13,13,13,13,13,4,15,15,0,0,0,0,0,0,0,0,0 | 258,258,258,151,258,258,258,258,258,258,151,70,70,0,0,0,0,0,0,0,0,0 |
| 5 | 8,8,8,8,8,8,8,8,8,3,3,15,0,0,0,0,0,0,0,0,0,0 | 282,282,282,282,282,282,282,282,282,146,146,70,0,0,0,0,0,0,0,0,0,0 |
| 6 | 11,11,11,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 | 216,216,216,148,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 |

Scene tracks: credits18, Ubi12, boss-death/rap19, game-over 10, intro20, conclusion21. Empty continue/gros-Rayman entries add no cue. TestCdLoop uses unsigned10*tick_delta/182+2, stop/wait/restart callback phases; pause corrects the start snapshot. Credits/rap polling uses callback count>180, independent of CD sector/MSF clocks.

## Event call sites

DOS literal sites independently recover the low EAX/AX cue argument inside the final basic block. The complete event_calls.json also lists containing-routine candidate sites and source guards; branch merging means a source row need not have its own unique call instruction. Dynamic eta cues are emitted by active-object animation transitions, not by reading every eta record.

| Caller | Call expression | DOS literal-confirmed sites | Source |
|---|---|---|---|
| start_cd_credits | `AudioTrackCommand(18)` | 1:0cf4c | src/audiocd.c:45 |
| start_cd_ubi_soft | `AudioTrackCommand(12)` | 1:0cf74 | src/audiocd.c:53 |
| start_cd_perdu | `PlayTchatchVignette(8)` | 1:0cf8e | src/audiocd.c:63 |
| start_cd_bbdead | `AudioTrackCommand(19)` | 1:0cff4 | src/audiocd.c:84 |
| start_cd_gameover | `AudioTrackCommand(10)` | 1:0d01c | src/audiocd.c:92 |
| DO_PI_EXPLOSION2 | `PlaySnd_old(0x54)` | 1:1062b | src/bbmont.c:324 |
| DO_BBL_REBOND | `PlaySnd(200, obj->id)` | 1:10684 | src/bbmont.c:337 |
| DO_BBL_COMMAND | `PlaySnd(200, obj->id)` | 1:10e15, 1:10f21 | src/bbmont.c:621 |
| DO_BBL_COMMAND | `PlaySnd(200, obj->id)` | 1:10e15, 1:10f21 | src/bbmont.c:658 |
| DO_BBMONT_COMMAND | `PlaySnd(114, obj->id)` | 1:11b5a, 1:11bbe | src/bbmont.c:1035 |
| DO_BBMONT_COMMAND | `PlaySnd(114, obj->id)` | 1:11b5a, 1:11bbe | src/bbmont.c:1046 |
| IS_ON_RESSORT_BLOC | `PlaySnd(249, -1)` | 1:1575a | src/blocs.c:457 |
| DO_PERFECT_BONUS_MAP | `PlayTchatchVignette(6)` | 1:13eb7 | src/bonus.c:231 |
| collision_briques | `PlaySnd_old(no_sound)` |  | src/breakout.c:665 |
| collision_rayman | `PlaySnd_old(44)` | 1:17981 | src/breakout.c:710 |
| casse_brique_start | `PlaySnd_old(0xc2)` | 1:180de | src/breakout.c:914 |
| casse_brique_play | `PlaySnd_old(0x6b)` | 1:18236 | src/breakout.c:962 |
| casse_brique_play | `PlaySnd_old(0x69)` | 1:1825a | src/breakout.c:966 |
| casse_brique_play | `PlaySnd_old(45)` | 1:182d7, 1:1831a, 1:1837a | src/breakout.c:977 |
| casse_brique_play | `PlaySnd_old(45)` | 1:182d7, 1:1831a, 1:1837a | src/breakout.c:984 |
| casse_brique_play | `PlaySnd_old(45)` | 1:182d7, 1:1831a, 1:1837a | src/breakout.c:991 |
| casse_brique_end | `PlaySnd_old(0x16)` |  | src/breakout.c:1027 |
| casse_brique_end | `PlaySnd_old(0xf)` |  | src/breakout.c:1030 |
| DO_CB_BALL_COMMAND | `PlaySnd_old(0x16)` | 1:18815 | src/breakout.c:1112 |
| DoCagePoingCollision | `PlaySnd(17, obj->id)` | 1:18c01 | src/cage.c:91 |
| DO_MEDAILLON_TOON | `PlaySnd(192, obj->id)` | 1:18c62 | src/cage.c:106 |
| DO_MEDAILLON_TOON | `PlaySnd(193, obj->id)` | 1:18c7a | src/cage.c:108 |
| DO_MEDAILLON_TOON_GELE | `PlaySnd(192, obj->id)` |  | src/cage.c:136 |
| DO_MEDAILLON_TOON_GELE | `PlaySnd(193, obj->id)` |  | src/cage.c:138 |
| DO_MEDAILLON_TOON_GELE | `PlaySnd(61, -1)` |  | src/cage.c:145 |
| DO_POING_COLLISION | `PlaySnd(214, (flags[obj->type].raw & 0x80) ? poing_obj_id : obj->id)` | 1:1d816 | src/collision1.c:1198 |
| goToRay | `PlaySnd(204, obj->id)` | 1:1e748 | src/collision1.c:1611 |
| DO_MOVE_MARACAS_COMMAND | `PlaySnd(0x73, obj->id)` | 1:2012b | src/collision2.c:603 |
| DoFallingObjPoingCollision | `PlaySnd(44, obj->id)` |  | src/collision2.c:1426 |
| DoFallingObjPoingCollision | `PlaySnd(139, obj->id)` |  | src/collision2.c:1430 |
| WORLD_CHOICE | `PlaySnd_old(77)` | 1:250fb | src/display.c:681 |
| DO_WORLD_MAP | `PlaySnd_old(77)` | 1:252cd | src/display.c:740 |
| DO_WORLD_MAP | `PlaySnd_old(69)` | 1:25351 | src/display.c:753 |
| DISPLAY_GAME_VIGNET | `PlayTchatchVignette(0)` | 1:25543 | src/display.c:818 |
| DO_VICTOIRE | `playVideo(&demo_video_mem, "conclu.dat", 21)` |  | src/display.c:965 |
| DoPaletteSpecialPC | `PlaySnd_old(195)` | 1:2d83e | src/frame.c:1282 |
| DO_PAR_BOMB_COMMAND | `PlaySnd_old(147)` | 1:2e56c | src/frame.c:1703 |
| DO_COMMANDE_GENERAL | `PlaySnd_old(68)` | 1:3a218, 1:3a294 | src/menu.c:871 |
| DO_COMMANDE_GENERAL | `PlaySnd_old(68)` | 1:3a218, 1:3a294 | src/menu.c:880 |
| DO_COMMANDE_GENERAL | `PlaySnd_old(69)` | 1:3a2df | src/menu.c:889 |
| DO_COMMANDE_OPTIONS | `PlaySnd_old(68)` | 1:3afa1, 1:3b031, 1:3b15b, 1:3b1cc | src/menu.c:1195 |
| DO_COMMANDE_OPTIONS | `PlaySnd_old(68)` | 1:3afa1, 1:3b031, 1:3b15b, 1:3b1cc | src/menu.c:1205 |
| DO_COMMANDE_OPTIONS | `PlaySnd_old(68)` | 1:3afa1, 1:3b031, 1:3b15b, 1:3b1cc | src/menu.c:1231 |
| DO_COMMANDE_OPTIONS | `PlaySnd_old(68)` | 1:3afa1, 1:3b031, 1:3b15b, 1:3b1cc | src/menu.c:1240 |
| DO_COMMANDE_KEY | `PlaySnd_old(0x44)` | 1:3bb62, 1:3bb98 | src/menu.c:1472 |
| DO_COMMANDE_KEY | `PlaySnd_old(0x44)` | 1:3bb62, 1:3bb98 | src/menu.c:1479 |
| DO_BUTTON_COMMANDE | `PlaySnd_old(0x44)` | 1:3c1c9, 1:3c20a, 1:3c269, 1:3c2b8, 1:3c307, 1:3c356 | src/menu.c:1649 |
| DO_BUTTON_COMMANDE | `PlaySnd_old(0x44)` | 1:3c1c9, 1:3c20a, 1:3c269, 1:3c2b8, 1:3c307, 1:3c356 | src/menu.c:1656 |
| DO_BUTTON_COMMANDE | `PlaySnd_old(0x44)` | 1:3c1c9, 1:3c20a, 1:3c269, 1:3c2b8, 1:3c307, 1:3c356 | src/menu.c:1668 |
| DO_BUTTON_COMMANDE | `PlaySnd_old(0x44)` | 1:3c1c9, 1:3c20a, 1:3c269, 1:3c2b8, 1:3c307, 1:3c356 | src/menu.c:1680 |
| DO_BUTTON_COMMANDE | `PlaySnd_old(0x44)` | 1:3c1c9, 1:3c20a, 1:3c269, 1:3c2b8, 1:3c307, 1:3c356 | src/menu.c:1692 |
| DO_BUTTON_COMMANDE | `PlaySnd_old(0x44)` | 1:3c1c9, 1:3c20a, 1:3c269, 1:3c2b8, 1:3c307, 1:3c356 | src/menu.c:1704 |
| DO_COMMANDE_PAD | `PlaySnd_old(0x44)` | 1:3ca40, 1:3ca80, 1:3cc8c | src/menu.c:1861 |
| DO_COMMANDE_PAD | `PlaySnd_old(0x44)` | 1:3ca40, 1:3ca80, 1:3cc8c | src/menu.c:1868 |
| DO_COMMANDE_PAD | `PlaySnd_old(0x44)` | 1:3ca40, 1:3ca80, 1:3cc8c | src/menu.c:1931 |
| DO_COMMANDE_GRAPHIC | `PlaySnd_old(0x44)` | 1:3db02, 1:3db61 | src/menu.c:2293 |
| DO_COMMANDE_GRAPHIC | `PlaySnd_old(0x44)` | 1:3db02, 1:3db61 | src/menu.c:2302 |
| DO_COMMANDE_VIDEOMODE | `PlaySnd_old(0x44)` | 1:3e5da, 1:3e61b | src/menu.c:2550 |
| DO_COMMANDE_VIDEOMODE | `PlaySnd_old(0x44)` | 1:3e5da, 1:3e61b | src/menu.c:2557 |
| menu_prg | `PlaySnd_old(69)` | 1:3f2be | src/menu.c:2847 |
| menu_prg | `PlaySnd_old(77)` | 1:3f2d1 | src/menu.c:2850 |
| DO_YESNOBIS | `PlaySnd_old(68)` | 1:3fb3d | src/menu.c:3107 |
| allocateMereDenisBombChips | `PlaySnd(220, obj->id)` | 1:40fec | src/meredenis.c:172 |
| DO_ANIM | `PlaySnd(get_eta(obj)->sound_index, obj->id)` |  | src/moteur.c:1519 |
| DO_ANIM | `PlaySnd(get_eta(obj)->sound_index, obj->id)` |  | src/moteur.c:1527 |
| DO_ANIM | `PlaySnd(get_eta(obj)->sound_index, obj->id)` |  | src/moteur.c:1550 |
| OBJ_IN_THE_AIR | `PlaySnd(187, obj->id)` | 1:47c8e | src/moteur.c:2180 |
| OBJ_IN_THE_AIR | `PlaySnd(200, obj->id)` | 1:47cbd, 1:47ce0 | src/moteur.c:2191 |
| OBJ_IN_THE_AIR | `PlaySnd(200, obj->id)` | 1:47cbd, 1:47ce0 | src/moteur.c:2196 |
| OBJ_IN_THE_AIR | `PlaySnd(45, obj->id)` | 1:47d34, 1:47d6c, 1:47da4 | src/moteur.c:2209 |
| OBJ_IN_THE_AIR | `PlaySnd(45, obj->id)` | 1:47d34, 1:47d6c, 1:47da4 | src/moteur.c:2218 |
| OBJ_IN_THE_AIR | `PlaySnd(45, obj->id)` | 1:47d34, 1:47d6c, 1:47da4 | src/moteur.c:2227 |
| OBJ_IN_THE_AIR | `PlaySnd(45, obj->id)` | 1:47d34, 1:47d6c, 1:47da4 | src/moteur.c:2245 |
| OBJ_IN_THE_AIR | `PlaySnd(140, obj->id)` |  | src/moteur.c:2249 |
| OBJ_IN_THE_AIR | `PlaySnd(0, obj->id)` |  | src/moteur.c:2253 |
| RAY_DEMIRAY | `PlaySnd(206, -1)` |  | src/moteur.c:4016 |
| RAY_DEMIRAY | `PlaySnd(207, -1)` |  | src/moteur.c:4021 |
| DoOneUpPoingCollision | `PlaySnd(194, obj->id)` | 1:51c6d | src/objupdate2.c:510 |
| DoOneUpRaymanCollision | `PlaySnd(194, obj->id)` | 1:51c99 | src/objupdate2.c:517 |
| DoPoingPowerupRaymanCollision | `PlaySnd(11, obj->id)` | 1:51e95 | src/objupdate2.c:584 |
| DoPowerupRaymanCollision | `PlaySnd(8, obj->id)` | 1:51edc | src/objupdate2.c:594 |
| DoSuperHelicoRaymanCollision | `PlaySnd(213, obj->id)` | 1:51ef3 | src/objupdate2.c:599 |
| DoGraineRaymanCollision | `PlaySnd(10, obj->id)` | 1:51f49 | src/objupdate2.c:609 |
| DoWizRaymanCollision | `PlaySnd(14, obj->id)` | 1:51f5f | src/objupdate2.c:613 |
| DoJaugeUpRaymanCollision | `PlaySnd(12, obj->id)` | 1:52099 | src/objupdate2.c:656 |
| DoBadGuy23PoingCollision | `PlaySnd(28, obj->id)` | 1:52ac9 | src/objupdate2.c:995 |
| DoBadGuy1PoingCollision | `PlaySnd(28, obj->id)` | 1:52bef | src/objupdate2.c:1022 |
| DO_PHOTOGRAPHE_CMD | `PlaySnd(40, obj->id)` | 1:530eb | src/objupdate2.c:1202 |
| PcMain | `playVideo(demo_video_mem, "intro.dat", 20)` |  | src/pcmain.c:1063 |
| DO_PMA_COMMAND | `PlaySnd(153, obj->id)` | 1:55ef4 | src/pmama.c:932 |
| DO_PMA_ATTER | `PlaySnd(62, obj->id)` | 1:5642d | src/pmama.c:1129 |
| IS_RAY_ON_LIANE | `PlaySnd(9, -1)` | 1:5c1fe | src/ray.c:391 |
| rayMayLandOnAnObject | `PlaySnd(110, cur_obj->id)` | 1:5c6a7 | src/ray.c:455 |
| rayMayLandOnAnObject | `PlaySnd((num_world == 1 \|\| num_world == 4) ? 19 : 242, -1)` |  | src/ray.c:460 |
| rayMayLandOnAnObject | `PlaySnd((num_world == 4 \|\| num_world == 5) ? 19 : 242, -1)` |  | src/ray.c:464 |
| rayMayLandOnAnObject | `PlaySnd((num_world == 4) ? 19 : 242, -1)` |  | src/ray.c:471 |
| rayMayLandOnAnObject | `PlaySnd(248, -1)` |  | src/ray.c:482 |
| rayMayLandOnAnObject | `PlaySnd(52, -1)` |  | src/ray.c:485 |
| rayMayLandOnAnObject | `PlaySnd(19, -1)` |  | src/ray.c:488 |
| rayMayLandOnAnObject | `PlaySnd(110, cur_obj->id)` | 1:5c6a7 | src/ray.c:528 |
| rayMayLandOnAnObject | `PlaySnd(112, cur_obj->id)` | 1:5c6de | src/ray.c:533 |
| rayMayLandOnAnObject | `PlaySnd(111, cur_obj->id)` | 1:5c708 | src/ray.c:540 |
| SET_RAY_BALANCE | `PlaySnd(0, -1)` | 1:5f2df | src/ray.c:1943 |
| RAY_BALANCE | `PlaySnd(19, -1)` | 1:5f797 | src/ray.c:2121 |
| RAY_IN_THE_AIR | `PlaySnd(242, -1)` |  | src/ray.c:2333 |
| RAY_IN_THE_AIR | `PlaySnd(19, -1)` |  | src/ray.c:2335 |
| DO_EXPLOSE_NOTE2 | `PlaySnd(246, obj->id)` | 1:65aef | src/saxo.c:204 |
| DO_NOTE_CMD | `PlaySnd(243, obj->id)` | 1:65cfc | src/saxo.c:275 |
| DO_EXPLOSE_NOTE1 | `PlaySnd(244, obj->id)` | 1:65ec7 | src/saxo.c:350 |
| DO_SKO | `PlaySnd(172, obj->id)` | 1:69f97 | src/skops.c:1158 |
| DO_SKO | `PlaySnd(173, obj->id)` | 1:69fad | src/skops.c:1161 |
| DO_SKO | `PlaySnd(177, obj->id)` | 1:69fc3 | src/skops.c:1164 |
| TestCdLoop | `AudioTrackCommand(((u8 *)mapCd - 1)[22 * (num_world - 1) + num_level])` |  | src/sound.c:867 |
| DO_TIR | `PlaySnd(24, cur_obj->id)` | 1:6af83 | src/special.c:703 |
| DO_REDEYE_FIRE | `PlaySnd(205, i)` | 1:6bc39 | src/special.c:1239 |
| DO_STONE_EXPLOSION | `PlaySnd_old(84)` | 1:6d45a | src/stoneman.c:488 |
| allocateStonemanStone | `PlaySnd(90, cur_obj->id)` | 1:6d704 | src/stoneman.c:581 |
| DO_PI_EXPLOSION | `PlaySnd_old(84)` | 1:6de79 | src/stoneman.c:846 |
| DoPiMusPoingCollision | `PlaySnd(83, obj->id)` | 1:6e026, 1:6e0e4 | src/stoneman.c:921 |
| DoPiMusPoingCollision | `PlaySnd(83, obj->id)` | 1:6e026, 1:6e0e4 | src/stoneman.c:941 |
| DO_PNG_COLL_STONEWOMAN | `PlaySnd(86, obj->id)` | 1:6e27d | src/stoneman.c:1009 |
| DO_LEV_POING_COLLISION | `PlaySnd(235, lev_obj->id)` | 1:6ebe0 | src/ufo.c:301 |
| DO_COMMANDE_SAVE | `PlaySnd_old(68)` | 1:599e0, 1:59a7d, 1:59b13, 1:59bac | src/worldmap.c:1151 |
| DO_COMMANDE_SAVE | `PlaySnd_old(68)` | 1:599e0, 1:59a7d, 1:59b13, 1:59bac | src/worldmap.c:1157 |
| DO_COMMANDE_SAVE | `PlaySnd_old(68)` | 1:599e0, 1:59a7d, 1:59b13, 1:59bac | src/worldmap.c:1164 |
| DO_COMMANDE_SAVE | `PlaySnd_old(68)` | 1:599e0, 1:59a7d, 1:59b13, 1:59bac | src/worldmap.c:1170 |
| DO_COMMANDE_SAVE | `PlaySnd_old(77)` | 1:59c0f | src/worldmap.c:1180 |
| SELECTION_SAVE_OPTION | `PlaySnd_old(69)` | 1:59c7e, 1:59cda, 1:59d39 | src/worldmap.c:1189 |
| SELECTION_SAVE_OPTION | `PlaySnd_old(69)` | 1:59c7e, 1:59cda, 1:59d39 | src/worldmap.c:1195 |
| SELECTION_SAVE_OPTION | `PlaySnd_old(69)` | 1:59c7e, 1:59cda, 1:59d39 | src/worldmap.c:1201 |
| SAISIE_NOM | `PlaySnd_old(68)` | 1:59e80, 1:59ea2, 1:59eb3, 1:59fb1, 1:5a083 | src/worldmap.c:1238 |
| SAISIE_NOM | `PlaySnd_old(68)` | 1:59e80, 1:59ea2, 1:59eb3, 1:59fb1, 1:5a083 | src/worldmap.c:1242 |
| SAISIE_NOM | `PlaySnd_old(68)` | 1:59e80, 1:59ea2, 1:59eb3, 1:59fb1, 1:5a083 | src/worldmap.c:1244 |
| SAISIE_NOM | `PlaySnd_old(68)` | 1:59e80, 1:59ea2, 1:59eb3, 1:59fb1, 1:5a083 | src/worldmap.c:1262 |
| SAISIE_NOM | `PlaySnd_old(68)` | 1:59e80, 1:59ea2, 1:59eb3, 1:59fb1, 1:5a083 | src/worldmap.c:1274 |
| SAISIE_NOM | `PlaySnd_old(77)` | 1:5a0e2 | src/worldmap.c:1283 |
| MAIN_CONTINUE_PRG | `PlaySnd_old(216)` | 1:5a7f4 | src/worldmap.c:1450 |
