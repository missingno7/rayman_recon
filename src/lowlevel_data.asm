 ; GOG v1.21 obj2 low-level initialized data, 2:00010..2:0215c.
; Replaces lowlevel_data + data_195a + data_2140 (see NOTES.md).
; Values below are declarations; no executable code or relocation fields occur in these spans.
.386p

_DATA segment para public use32 'DATA'
; Partition from the rasterizer's accesses (build/workers/s_00010/NOTES.md); names INVENTED.
public _raster_first_y, _raster_rows, _raster_descriptor, _raster_texture, _raster_du, _raster_neg_du
public _raster_dv, _raster_neg_dv, _raster_destination, _raster_scanline, _raster_row_offset, _raster_rows_left
_raster_first_y:            ; 2:00010
    dd 0
_raster_rows:               ; 2:00014, 200 rows of 32 bytes
    dd 200 dup (0ffffh, 0, 0, 0, 0, 0, 0, 0)
_raster_descriptor:         ; 2:01914
    dd 0
    db 10 dup (0)           ; 2:01918..01922, no access found (OPEN)
_raster_texture:            ; 2:01922
    dd 0
    db 8 dup (0)            ; 2:01926..0192e, no access found (OPEN)
_raster_du:                 ; 2:0192e
    dd 0
_raster_neg_du:             ; 2:01932
    dd 0
_raster_dv:                 ; 2:01936
    dd 0
_raster_neg_dv:             ; 2:0193a
    dd 0
_raster_destination:        ; 2:0193e
    dd 0
_raster_scanline:           ; 2:01942
    dd 0
_raster_row_offset:         ; 2:01946
    dd 0
_raster_rows_left:          ; 2:0194a
    dd 0
    db 12 dup (0)           ; 2:0194e..0195a, no access found (OPEN)
 ; Assembly-owned table at 2:0195a (2 mod 4); no C contribution boundary.
; DOS 1:04a91 (fixup 1:04a94) indexes four dwords; Android TableImageX size=16.
public _TableImageX
_TableImageX:
    dd 0, 16000, 32000, 48000
public saved_vga_registers_0196a
public row_offsets_320_01b00
public row_offsets_80_01e20
saved_vga_registers_0196a:
    db 6 dup (0)
; 2:01970..2:01b00 is retained as unlabelled zero storage; its ownership is open.
    db 400 dup (0)
row_offsets_320_01b00:
    dd 0, 320, 640, 960
    dd 1280, 1600, 1920, 2240
    dd 2560, 2880, 3200, 3520
    dd 3840, 4160, 4480, 4800
    dd 5120, 5440, 5760, 6080
    dd 6400, 6720, 7040, 7360
    dd 7680, 8000, 8320, 8640
    dd 8960, 9280, 9600, 9920
    dd 10240, 10560, 10880, 11200
    dd 11520, 11840, 12160, 12480
    dd 12800, 13120, 13440, 13760
    dd 14080, 14400, 14720, 15040
    dd 15360, 15680, 16000, 16320
    dd 16640, 16960, 17280, 17600
    dd 17920, 18240, 18560, 18880
    dd 19200, 19520, 19840, 20160
    dd 20480, 20800, 21120, 21440
    dd 21760, 22080, 22400, 22720
    dd 23040, 23360, 23680, 24000
    dd 24320, 24640, 24960, 25280
    dd 25600, 25920, 26240, 26560
    dd 26880, 27200, 27520, 27840
    dd 28160, 28480, 28800, 29120
    dd 29440, 29760, 30080, 30400
    dd 30720, 31040, 31360, 31680
    dd 32000, 32320, 32640, 32960
    dd 33280, 33600, 33920, 34240
    dd 34560, 34880, 35200, 35520
    dd 35840, 36160, 36480, 36800
    dd 37120, 37440, 37760, 38080
    dd 38400, 38720, 39040, 39360
    dd 39680, 40000, 40320, 40640
    dd 40960, 41280, 41600, 41920
    dd 42240, 42560, 42880, 43200
    dd 43520, 43840, 44160, 44480
    dd 44800, 45120, 45440, 45760
    dd 46080, 46400, 46720, 47040
    dd 47360, 47680, 48000, 48320
    dd 48640, 48960, 49280, 49600
    dd 49920, 50240, 50560, 50880
    dd 51200, 51520, 51840, 52160
    dd 52480, 52800, 53120, 53440
    dd 53760, 54080, 54400, 54720
    dd 55040, 55360, 55680, 56000
    dd 56320, 56640, 56960, 57280
    dd 57600, 57920, 58240, 58560
    dd 58880, 59200, 59520, 59840
    dd 60160, 60480, 60800, 61120
    dd 61440, 61760, 62080, 62400
    dd 62720, 63040, 63360, 63680
row_offsets_80_01e20:
    dd 0, 80, 160, 240
    dd 320, 400, 480, 560
    dd 640, 720, 800, 880
    dd 960, 1040, 1120, 1200
    dd 1280, 1360, 1440, 1520
    dd 1600, 1680, 1760, 1840
    dd 1920, 2000, 2080, 2160
    dd 2240, 2320, 2400, 2480
    dd 2560, 2640, 2720, 2800
    dd 2880, 2960, 3040, 3120
    dd 3200, 3280, 3360, 3440
    dd 3520, 3600, 3680, 3760
    dd 3840, 3920, 4000, 4080
    dd 4160, 4240, 4320, 4400
    dd 4480, 4560, 4640, 4720
    dd 4800, 4880, 4960, 5040
    dd 5120, 5200, 5280, 5360
    dd 5440, 5520, 5600, 5680
    dd 5760, 5840, 5920, 6000
    dd 6080, 6160, 6240, 6320
    dd 6400, 6480, 6560, 6640
    dd 6720, 6800, 6880, 6960
    dd 7040, 7120, 7200, 7280
    dd 7360, 7440, 7520, 7600
    dd 7680, 7760, 7840, 7920
    dd 8000, 8080, 8160, 8240
    dd 8320, 8400, 8480, 8560
    dd 8640, 8720, 8800, 8880
    dd 8960, 9040, 9120, 9200
    dd 9280, 9360, 9440, 9520
    dd 9600, 9680, 9760, 9840
    dd 9920, 10000, 10080, 10160
    dd 10240, 10320, 10400, 10480
    dd 10560, 10640, 10720, 10800
    dd 10880, 10960, 11040, 11120
    dd 11200, 11280, 11360, 11440
    dd 11520, 11600, 11680, 11760
    dd 11840, 11920, 12000, 12080
    dd 12160, 12240, 12320, 12400
    dd 12480, 12560, 12640, 12720
    dd 12800, 12880, 12960, 13040
    dd 13120, 13200, 13280, 13360
    dd 13440, 13520, 13600, 13680
    dd 13760, 13840, 13920, 14000
    dd 14080, 14160, 14240, 14320
    dd 14400, 14480, 14560, 14640
    dd 14720, 14800, 14880, 14960
    dd 15040, 15120, 15200, 15280
    dd 15360, 15440, 15520, 15600
    dd 15680, 15760, 15840, 15920
 ; DOS sprite code consumes these two true-start s32 state variables.
public _saved_sprite_color, _saved_sprite_width
_saved_sprite_color: ; 2:02140
    dd 0
_saved_sprite_width: ; 2:02144
    dd 0
public lowlevel_timer_02148
public lowlevel_timer_02150
public lowlevel_timer_02154
public lowlevel_timer_02158
lowlevel_timer_02148:
    dd 0
; 2:0214c..2:02150 has no found access; keep the zero dword anonymous.
    db 4 dup (0)
lowlevel_timer_02150:
    dd 0
lowlevel_timer_02154:
    dd 0
lowlevel_timer_02158:
    dd 0
_DATA ends
DGROUP group _DATA

end
