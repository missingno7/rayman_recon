# Boss/enemy pass6: remaining uncertainty

Names are retained. Observed operations/layouts are separate from intended meanings. Complete referenced shared types include unused members; these are not additional unexplained enemy instructions.
147 selected current UNKNOWN/HYPOTHESIS records; 98 are newly documented entities in this delivery. New HYPOTHESIS count is zero.

## field:bande_t._pad_0a

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for bande_t._pad_0a. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $[2]`. Units: see observed computation and contract.

Packed offset +0xa; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:bande_t._pad_16

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for bande_t._pad_16. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $[2]`. Units: see observed computation and contract.

Packed offset +0x16; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:bande_t._pad_5

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for bande_t._pad_5. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x5; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:bb_attack_entry_t.reserved

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for bb_attack_entry_t.reserved. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x3; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:bb_data_t._pad_15

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for bb_data_t._pad_15. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0xf; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:bb_data_t._pad_7

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for bb_data_t._pad_7. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x7; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:block_flags_t.bits.unused0

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved.

Declaration: `u8 $ : 1`. Units: tile-class bit.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:block_flags_t.bits.unused2

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved.

Declaration: `u8 $ : 3`. Units: packed tile-class bits.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:casse_brique_entry_t.reserved

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for casse_brique_entry_t.reserved. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x7; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:cb_collision_t.reserved

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for cb_collision_t.reserved. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x7; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:couteaux_info_t.dos_tail

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for couteaux_info_t.dos_tail. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x13; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:credit_t.cmd

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Credit control byte consumed by credits logic outside the display loop.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x9; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:credit_t.unknown_b

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Credit record trailing byte not read by the display loop.

Declaration: `u8 $`. Units: unknown.

Packed offset +0xb; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:def_bande_t._pad_3

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for def_bande_t._pad_3. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x3; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:def_bande_t._pad_5

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for def_bande_t._pad_5. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x5; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:def_bande_t._pad_7

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for def_bande_t._pad_7. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x7; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:def_sprite_t._pad_6

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for def_sprite_t._pad_6. Observed layout and typed accesses are retained without a rename.

Declaration: `s16 $`. Units: see observed computation and contract.

Packed offset +0x6; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:finbosslevel_t.crazy_drummer

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

Packed offset +0x1; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:flocon_t.field_4

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

Packed offset +0x4; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:flocon_t.field_6

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x6; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:flocon_t.field_7

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x7; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:map_tile_t.field_3

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Raw serialized map byte+3 with no intended meaning assigned.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x3; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:map_tile_t.field_5

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Raw serialized map byte+5 with no intended meaning assigned.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x5; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.charge

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.charge. Observed layout and typed accesses are retained without a rename.

Declaration: `u16 $`. Units: see observed computation and contract.

Packed offset +0x8; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.damage

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.damage. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0xf; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.field_12

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.field_12. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x12; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.field_13

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.field_13. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x13; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.field_4

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.field_4. Observed layout and typed accesses are retained without a rename.

Declaration: `u16 $`. Units: see observed computation and contract.

Packed offset +0x4; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.field_A

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.field_A. Observed layout and typed accesses are retained without a rename.

Declaration: `u16 $`. Units: see observed computation and contract.

Packed offset +0xa; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.is_active

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.is_active. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0xe; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.is_boum

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.is_boum. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x11; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.is_charging

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.is_charging. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x10; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.is_returning

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.is_returning. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0xd; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.speed_x

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.speed_x. Observed layout and typed accesses are retained without a rename.

Declaration: `u16 $`. Units: see observed computation and contract.

Packed offset +0x6; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.sub_etat

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.sub_etat. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0xc; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:mite_poing_local_t.y_16

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for mite_poing_local_t.y_16. Observed layout and typed accesses are retained without a rename.

Declaration: `s32 $`. Units: see observed computation and contract.

Packed offset +0x0; 4 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:obj_t.field_83

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x83; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:obj_t.init_flag

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Serialized byte with initializer writes of7; no intended common meaning or typed reader is established.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x78; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:obj_t.link

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Multipurpose signed16 scratch; initialization includes paired-object links and unrelated timers/parameters.

Declaration: `s16 $`. Units: type dependent.

Packed offset +0x52; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:obj_t.sub_speed

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Type-dependent signed16 scratch used for a water-motion residual and another type's horizontal distance.

Declaration: `s16 $`. Units: type-dependent pixels / sixteenths.

Packed offset +0x3a; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:obj_t.timer

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Multipurpose byte countdown/state scratch with common reset0 and actor-specific defaults.

Declaration: `u8 $`. Units: type dependent.

Packed offset +0x7f; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:obj_t.unused_padding

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x67; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:options_jeu_t.is_stereo

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

Packed offset +0x1c; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:options_jeu_t.music_enabled

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

Packed offset +0x18; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:options_jeu_t.sound_volume

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

Packed offset +0x1a; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:options_jeu_t.unknown_1e

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

Packed offset +0x1e; 2 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:pma_block_flags_t.bits

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for pma_block_flags_t.bits. Observed layout and typed accesses are retained without a rename.

Declaration: `struct 
{
  u8 unused0 : 1;
  u8 solid : 1;
  u8 unused1 : 6;
} $`. Units: see observed computation and contract.

Packed offset +0x0; 1 byte(s); not applicable (aggregate).

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:pma_block_flags_t.bits.solid

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for pma_block_flags_t.bits.solid. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $ : 1`. Units: see observed computation and contract.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:pma_block_flags_t.bits.unused0

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for pma_block_flags_t.bits.unused0. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $ : 1`. Units: see observed computation and contract.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:pma_block_flags_t.bits.unused1

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for pma_block_flags_t.bits.unused1. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $ : 6`. Units: see observed computation and contract.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:pma_block_flags_t.raw

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for pma_block_flags_t.raw. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:poing_t.field_12

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x12; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:poing_t.field_13

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x13; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:poing_t.field_4

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

Declaration: `s16 $`. Units: unknown.

Packed offset +0x4; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:rayevts_t.handstand

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:rayevts_t.handstand_dash

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

Packed offset +0x0; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:record_t.unknown_17

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x17; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:save_state_t.field_25b

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Trailing checkpoint byte with no typed reader/writer found.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x25b; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:sax_note_entry_t.dos_alignment

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for sax_note_entry_t.dos_alignment. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x1; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:sax_note_entry_t.field4_0x8

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for sax_note_entry_t.field4_0x8. Observed layout and typed accesses are retained without a rename.

Declaration: `s16 $`. Units: see observed computation and contract.

Packed offset +0x8; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:sprite_t.color_bits.unused

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 4`. Units: unknown.

Packed offset +0xa; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:sprite_t.flags_bits.unused0

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 2`. Units: unknown.

Packed offset +0xb; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:sprite_t.flags_bits.unused3

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 5`. Units: unknown.

Packed offset +0xb; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:vitraux_info_t.field_6

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

No intended meaning established for vitraux_info_t.field_6. Observed layout and typed accesses are retained without a rename.

Declaration: `u8 $`. Units: see observed computation and contract.

Packed offset +0x6; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.color

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.color storage; no intended meaning is assigned from its spelling alone.

Declaration: `s8 $`. Units: unknown.

Packed offset +0xc; 1 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.field_D

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.field_D storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0xd; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.field_E

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.field_E storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0xe; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.field_F

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.field_F storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0xf; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.index_down

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_down storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x5; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.index_left

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_left storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x6; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.index_right

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_right storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x7; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.index_up

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_up storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x4; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.is_unlocked

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.is_unlocked storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 1`. Units: unknown.

Packed offset +0x8; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.is_unlocking

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.is_unlocking storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 1`. Units: unknown.

Packed offset +0x8; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.level

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.level storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0xb; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.state

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.state storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0x8; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.state_bit1

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.state_bit1 storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 1`. Units: unknown.

Packed offset +0x8; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.state_rest

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.state_rest storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 5`. Units: unknown.

Packed offset +0x8; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.text

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.text storage; no intended meaning is assigned from its spelling alone.

Declaration: `char *$`. Units: unknown.

Packed offset +0x10; 4 byte(s); not applicable (pointer).

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.world

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.world storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

Packed offset +0xa; 1 byte(s); unsigned.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.xpos

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.xpos storage; no intended meaning is assigned from its spelling alone.

Declaration: `s16 $`. Units: unknown.

Packed offset +0x0; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## field:world_info_t.ypos

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.ypos storage; no intended meaning is assigned from its spelling alone.

Declaration: `s16 $`. Units: unknown.

Packed offset +0x2; 2 byte(s); signed.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:RMrepere

Owner: `docs/semantics/globals.json`. Level: **UNKNOWN**.

Only initialization to zero was found; no consuming reader in the full source/direct-fixup review.

Declaration: `s16 $`. Units: see observed computation and contract.

DOS binding: `2:3fac0`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:SaxMarche

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Only initialization to zero was found; no gameplay role established.

Declaration: `s16 $`. Units: see observed computation and contract.

DOS binding: `2:5577e`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:XOFFSETRAY

Owner: `docs/semantics/breakout.json`. Level: **UNKNOWN**.

Breakout initialization stores 56 or -8; no consuming reader found in the scoped code.

Declaration: `s16 $`. Units: see observed computation and contract.

DOS binding: `2:4e81c`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:byte_DF74C

Owner: `docs/semantics/dark.json`. Level: **UNKNOWN**.

Dark spell-selection branches store two. No value-consuming direct reader was found; purpose unresolved.

Declaration: `u8 $`. Units: see observed computation and contract.

DOS binding: `2:4f74c`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:final_pass_a

Owner: `docs/semantics/hybrid.json`. Level: **UNKNOWN**.

Written as zero then as final_pass_x; no consuming reader found in the source/direct-fixup review.

Declaration: `s16 $`. Units: see observed computation and contract.

DOS binding: `2:3f8c2`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:let_shadow

Owner: `docs/semantics/globals.json`. Level: **UNKNOWN**.

Byte written by save/world-map rendering without a located value-consuming reader.

Declaration: `u8 $`. Units: unknown.

DOS binding: `2:51d0`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## global:niveau

Owner: `docs/semantics/globals.json`. Level: **UNKNOWN**.

Meaning remains unresolved. Exact DOS readers/writers and source computations for niveau are recorded; its existing name is retained.

Declaration: `s16 $`. Units: see observed computation and contract.

DOS binding: `2:5ec4`.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BAT_POING_COLLISION:bat_obj:1

Owner: `docs/semantics/bat.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `obj_t *$`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BAT_POING_COLLISION:sprite:1

Owner: `docs/semantics/bat.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBL_COMMAND:bb1_x:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 bb1_x;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBL_COMMAND:speed_x:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 speed_x;`
- `if (obj->speed_x < 0)`
- `spd_x = obj->speed_x;`
- `if (obj->speed_x > 0)`
- `obj->speed_x = new_spd_x;`
- `if (Abs((s32) obj->speed_x) == 13)`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:temp_v0:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 temp_v0;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:temp_v0_2:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 temp_v0_2;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:temp_v0_3:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 temp_v0_3;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:temp_v0_4:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u16 temp_v0_4;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:temp_v1_2:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 temp_v1_2;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:temp_v1_3:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 temp_v1_3;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:test_1:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `bb_data_t *$`. Units: unknown.

Observed uses (full list is in the registry):
- `bb_data_t *test_1;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:test_3:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 test_3;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:test_4:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 test_4;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:test_5:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 test_5;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:test_6:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u32 test_6;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v0:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u16 var_v0;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v0_2:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 var_v0_2;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v0_3:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 var_v0_3;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v0_4:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 var_v0_4;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v0_5:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 var_v0_5;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v0_6:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 var_v0_6;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v1:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 var_v1;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT2_COMMAND:var_v1_2:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 var_v1_2;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT_COMMAND:pos_pierre:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 pos_pierre;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT_COMMAND:wait:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 wait;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT_COMMAND:x_1:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 x_1;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_BBMONT_COMMAND:x_2:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 x_2;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_DARK_POING_COLLISION:sprite:1

Owner: `docs/semantics/dark.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_HYB_BBF2_POING_COLLISION:sprite:1

Owner: `docs/semantics/hybrid.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_LEV_POING_COLLISION:sprite:1

Owner: `docs/semantics/ufo.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_NOTE_TOUCHEE:sprite:1

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_PNG_COLL_STONEMAN:sprite:1

Owner: `docs/semantics/stoneman.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_PNG_COLL_STONEWOMAN:sprite:1

Owner: `docs/semantics/stoneman.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SAXO3_COMMAND:sprite_h:1

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 sprite_h;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SAXO3_COMMAND:sprite_w:1

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 sprite_w;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SAXO3_COMMAND:temp_flags:1

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 temp_flags;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SAXO3_COMMAND:temp_x:1

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u16 temp_x;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SAXO_COUP:hp:1

Owner: `docs/semantics/saxo.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 hp;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SKO_HIT:sprite:1

Owner: `docs/semantics/skops.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_SPIDER_PLAFOND_POING_COLLISION:sprite:1

Owner: `docs/semantics/araignee.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DO_TOTEM_COMMAND:new_x:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s16 new_x;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoBlackToonPoingCollision:sprite:1

Owner: `docs/semantics/blacktoon.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoFishPoingCollision:sprite:1

Owner: `docs/semantics/poisson.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoPiMusPoingCollision:sprite:1

Owner: `docs/semantics/stoneman.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoPiPoingCollision:sprite:1

Owner: `docs/semantics/stoneman.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoPoissonBleuPoingCollision:sprite:1

Owner: `docs/semantics/poisson.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoSpiderPoingCollision:sprite:1

Owner: `docs/semantics/araignee.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoStoneDogPoingCollision:sprite:1

Owner: `docs/semantics/stoneman.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoTntClownPoingCollision:sprite:1

Owner: `docs/semantics/clown.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoTrumpetPoingCollision:sprite:1

Owner: `docs/semantics/trumpet.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:DoWatAndBigClownPoingCollision:sprite:1

Owner: `docs/semantics/clown.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:allocateTir:new_x_2:1

Owner: `docs/semantics/bbmont.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 new_x_2;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:changeMoskitoPhase:hp:1

Owner: `docs/semantics/moskito.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 hp;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:doMoskitoHit:sprite:1

Owner: `docs/semantics/moskito.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:fix_mit_Xspeed:eta:1

Owner: `docs/semantics/mite.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `eta_t *$`. Units: unknown.

Observed uses (full list is in the registry):
- `eta_t *eta;`
- `obj->eta[obj->main_etat][obj->sub_etat].flags_bits.anim_reverse = (((!obj->flags.flip_x) && (obj->speed_x > 0)) || (obj->flags.flip_x && (obj->speed_x < 0))) ? (true) : (false);`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:fix_mit_Xspeed:unk_1:1

Owner: `docs/semantics/mite.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Observed uses (full list is in the registry):
- `s32 unk_1;`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:pma_get_eject_sens:obj:1

Owner: `docs/semantics/pmama.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `obj_t *$`. Units: unknown.

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

## local:snapLaserToWeapon:sub_etat:1

Owner: `docs/semantics/meredenis.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $`. Units: unknown.

Observed uses (full list is in the registry):
- `u8 sub_etat;`
- `eta = &unk_obj->eta[unk_obj->main_etat][unk_obj->sub_etat];`
- `switch (unk_obj->sub_etat)`

Evidence: owning registry; `evidence/negative_results.json`, `symbols.json`, per-unit `*.dos.txt` / `*_typed_accesses.json`, and the complete-image displacement candidates. A token/displacement hit alone is not typed identity.

