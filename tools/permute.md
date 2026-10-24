Watcom source permuter
=====================

Requirements: Python 3.10+, pycparser >= 2.21 (tested with 3.00), capstone,
and the repository's existing pinned toolchain and extracted oracle. No GCC,
Clang, external preprocessor, or upstream permuter runtime is used.

Run from the repository root:

```
python tools/permute.py src/collision1.c inter_box --conservative --seed 20261023 --max-compiles 100 --seconds 300 --out build/workers/arm_b/inter_box --mechanism colour
python tools/permute.py src/ray.c RAY_BALANCE_ANIM --aggressive --seed 20261023 --max-compiles 100 --seconds 300 --out build/workers/arm_c/balance --mechanism Score --jobs 4
```

Before integration, replace `tools/permute.py` with
`build/workers/t_perm/patch/tools/permute.py`. It discovers the real repository
by walking parent directories; `--repo-root` is available for other layouts.
Use an unused output directory under build/ or scratch/.

`--mechanism` can be repeated. `--explanation path/to/explanation.json` reads an
existing explain.py report and weights relevant mutation families. It does not
run the debugger/compiler itself. Guidance changes sampling weights, not safety
guards or acceptance. Inspect explain.py first when investigating a new NEAR.

Continue with the same command plus `--resume` and increased total budgets:

```
python tools/permute.py src/collision1.c inter_box --conservative --seed 20261023 --max-compiles 200 --seconds 600 --out build/workers/arm_b/inter_box --mechanism colour --resume
```

The manifest checks input/header/tool/config hashes, Python/parser versions,
mode, seed, profile, oracle, and worker count. Changes require a new run.
Frozen snapshots and committed candidate hashes are checked on resume.
The deterministic candidate RNG is derived from (seed, candidate ID). Parents
are selected from the baseline and four best admissible candidates. Mutations
can accumulate across generations. Workers are consumed in submission order,
so OS completion order does not affect selection. Parallel search uses fixed
worker-sized batches; changing jobs or stopping part-way through a batch can
change the subsequent search stream. Timing cutoffs inherently affect the
number of admitted candidates. Logs preserve the actual stream.

Every compiler call counts, including preprocessing, baseline, compile errors,
and the additional fresh hit check. Budgets are cumulative across resumes.
Interrupted reservations remain charged even if their result was lost. Their
sources/logs remain available; uncommitted attempts are abandoned. A candidate
that hits on the final allowed compile awaits a fresh check on resume.
No new call starts after the seconds budget. In-flight calls finish; wcc's
native compiler timeout is 120 s, and a child worker has a 180 s bound.

Outputs:

* `input/`: original full unit and recursively copied quoted headers; same C filename.
* `context.i`: pinned wcc preprocessing with wcc's normal INCLUDE ordering.
* `state.json`: atomic, fsynced checkpoint and manifest.
* `candidates.jsonl`: candidate hash, source path, parent, complete mutation chain,
  proof guards, score, elapsed time, compiler count, losses, errors and duplicates.
* `c/ID/<unit>.c`: full unit candidates, compiler log/object, full verifier results.
* `summary.json`: best target ranking/byte differences, counts, time and status.
* `accepted/<unit>.c`: only after a separate fresh full-unit STRICT check with
  zero baseline STRICT losses and unchanged data/public/alignment invariants.

The score uses ONLY target extent bytes from match.verify_object, masked
relocation fields, fixup-site differences, reference problems, length delta
to the next function binding, and a Capstone instruction diff. Instruction
decoding retains undecodable tails. Byte/instruction/length/reference penalties
are ranking hints. A score of zero is not an acceptance condition. The matcher
checks the candidate's full public extent, fixup sites, and all reference targets.
Every candidate is checked as a complete unit. No objects/executables are patched.

The input function signature and all source outside its body are retained.
The pinned preprocessed body is parsed with its real typedef/struct context.
Context-only pragmas/calling conventions are removed for parsing; far/near
qualifiers round-trip through parser placeholders. Existing inline helpers
remain in their original headers. The body is never emitted with new pragmas.
The replacement preserves its newline count so downstream __LINE__/assert
expansions do not move. String/character contents are never whitespace-edited.
Residual macro names from the original source, copied headers or pinned system
headers cause a refusal to prevent accidental second expansion. This includes
inactive definitions: a deliberate conservative limitation.

Conservative families: pure integral operand commutation, terminated case order,
uninitialized leading scalar declaration order, independent local assignments,
condition truth spelling, branch reversal, compound assignment spelling,
unused prefix/postfix results, bounded integer thresholds, and new scopes.
Aggressive adds exact-type temporaries, same-width operand snapshots, address-taken
fresh locals, forward-goto if lowering, and duplicated trailing returns.
Arbitrary width changes are deliberately excluded: same-width snapshots retain
the original promotion at use. No arithmetic reassociation, float commutation,
pointer arithmetic commutation, speculative reads, dead uninitialized reads,
volatile accesses, public prototype changes, or invented globals are offered.

These transformations are sound under their syntactic/type guards for executions
where the original C has defined behavior. This is a guarded rule set, not a
general C equivalence prover. Side-effecting operands cannot be commuted, memory
assignments cannot be reordered, and declaration initializers cannot be reordered.
Fixed literal-sized automatic arrays are preserved; nonliteral extents, local
statics/typedefs/prototypes and target-local directives are refused. Unsupported
dialects fail closed. The supplied 26-function pilot passes parsing.

Data checks compare non-code segment bytes, coverage, size/alignment, publics,
relocations, BSS metadata, and code-segment alignment/unowned data prefixes.
Full result files preserve code sizes, function placements and neighboring
reference/jump-table problems. Rejected candidates may have a better target score;
they cannot become search parents or accepted sources. For promotion, rerun
tools/match.py on the accepted full unit, then the integrator's normal gate.

`permute_bench.py --out build/workers/bench --max-compiles 20` runs the three
pilot functions in both modes and an identical-input serial/parallel control.
Its throughput includes compile+full-unit verification and process startup.
Process workers use ordinary files/pipes, not multiprocessing's named Windows
semaphores. All workers are bounded and reaped; there are no daemon services.
Default jobs=1 keeps the normal serial policy. Measured concurrency is optional. With --jobs N the permuter keeps N persistent evaluation processes (oracle decoded once each); measured
15 compiles/s at --jobs 8 vs 5.3/s serial (docs/COMPILER.md "Source permutation").

Quality: conservative successful rewrites are COMPILER_STEERED; chains using
synthetic families are SYNTHETIC. An unchanged winning baseline is NATURAL.
Quality is independent of STRICT. No historical-spelling filter is applied.
