# Per-unit native contracts

Keep integer widths, signed divisions/remainders, arithmetic right shifts, narrowing/wrapping points, RNG order and update/draw order from the registries’ computation records and complete C copies. Confidence is distinct from byte matching. Reuse the existing PLATFORM_OPEN, ASM_SPEC, FILE_FORMATS and RENDERING specifications.

- **cdaudio** — Replace MSCDEX/DPMI buffers, selectors, drive discovery and busy polling with a native CD-track backend. Keep track IDs, pause/resume semantics, TOC sector/MSF distinction and scene-level completion policy. Orphan byte APIs retain UNKNOWN meanings; existing PLATFORM_OPEN omit decision applies.
- **sound** — Keep numeric cue routing, per-object ownership/repeat/interruption policy, delayed/followup map_time queue, table gains/pitches, sentinels and call ordering. Backend voice handles can change representation. There is no game-level numeric priority comparator to recreate from the policy-byte magnitude.
- **hifi** — Replace HMI driver/device/IRQ/DMA/timer representation. Retain bank decoding/checksums, fixed/world mapping, PCM reference10000Hz, Q16 pitch, exact volume/pan scale, byte loop interval and speech resource lifetimes; reuse PLATFORM_OPEN item3/4.
- **audiocd** — Preserve scene CD tracks18/12/19/10, permission/protection gates, speech8 loss path and >180 callback poll cadence. Backend drive-index/status representations are replaceable.
- **movie** — Replace BIOS/PIT/VGA/file-handle representation. Preserve chunk semantics, compact header handling, centering320 stride, frame index/input order, selected-frame pacing and CD sync table. Reuse ASM_SPEC palette/sync contracts; alternate presentation decodes every frame.
- **effect1** — Keep pause input/callback cadence, palette fades, chosen algorithm parameters and BIOS-tick correction of CD time. Replace VGA plane I/O/screen storage while preserving four-plane copy semantics.
- **effect2** — Keep fragment grid, random draw sequence, byte angle wrapping, Q9 rotation/depth projection and draw order. Reuse ASM_SPEC quad rasterization and packed256-pitch textures.
- **effecthelp** — Keep sphere-grid generation, integer projection/interpolation and asymmetric bounce/drop latches. Floating setup uses DOS constant/truncation semantics. Reuse ASM_SPEC rasterizer.
- **effect50350** — Keep15 mode hinge/axis cases, six-point Q9 Z/Y/X rotations, pivot/depth calculation and two draw records; surface/backend allocation representation can change.
- **effect34580** — Keep12 row-span modes, signed16 truncation, clipped-copy behavior and mode 9 extra immediate passes. Source/destination clipping does not compensate the opposite coordinate.
- **effect6ec80** — Keep75-byte signed wave history/ramp, unsigned geometry widths, integer radius-index/divisor projection, border vertices and quad order.
- **effect633e0** — Keep acos setup table, three remap phases/clear phase, asymmetric zero-pixel fallback, signed phase direction and exact source-column arithmetic. Visual name beyond observed column remap is not promoted.
- **effect6c3b0** — Keep radial displacement formula, truncation to signed bytes, random velocity signs and boundary reflection; replace raw surfaces but preserve sampling positions.
- **effect6e380** — Keep200-row width-limited copies and 320/256 pitch conversion; native textures may replace physical packed buffers.
- **dosmem** — Replace conventional/DPMI allocation and interrupt scratch with native platform services. Preserve error/result contracts at callers; selectors/real-mode segment math are backend representation.
- **dosprobe** — Replace BIOS/DOS/Windows detection with explicit native platform policy. Preserve the existing downstream conditional behavior where relevant; no native OS-version number should masquerade as DOS return bytes.
- **fnd** — Keep one-based world/level background-mode exceptions, camera parallax equations and signed16 output. VGA buffer/PLAN0 conversion remains the existing FILE_FORMATS/RENDERING contract.
- **blood** — Keep RNG call order, ten falling seeds,160 two-pixel columns, byte-height propagation and frame-origin comparison. This routine consumes game RNG and is observable presentation state.

Keep game cue ownership, delays, phase counters, palette/effect geometry and resource lifetimes in game/presentation state. Replace real-mode pointers/selectors, IRQ/DMA/device handles, BIOS/PIT/VGA accesses, file descriptors and HMI mixer handles at the native boundary. `sound_config` is a 36-byte consuming prefix of 76-byte storage; caps copy is106 bytes; neither view justifies changing canonical ownership in this pass.
