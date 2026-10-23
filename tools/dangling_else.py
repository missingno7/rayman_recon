"""Find the pycparser dangling-else signature: an unbraced `if` printed deeper than the `else` C binds to it.

    python tools/dangling_else.py FILE.c [...]
An emitter that prints the intended tree indents a nested unbraced if deeper than an outer else; C binds that
else to the nested if (permuter defect fixed in 4bad18c). Preprocessor lines are ignored. Exit 1 on any hit.
"""
import re
import sys

TOK = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*|\belse\b|\bif\b|[{};()]', re.S)


def scan(path):
    text = open(path, encoding="utf-8", errors="replace").read()
    text = re.sub(r"(?m)^[ 	]*#.*$", lambda m: " " * len(m[0]), text)  # drop preprocessor lines
    line_of = lambda pos: text.count("\n", 0, pos) + 1
    indent_of = lambda pos: len(text[text.rfind("\n", 0, pos) + 1:pos]) - len(text[text.rfind("\n", 0, pos) + 1:pos].lstrip())
    toks = [m for m in TOK.finditer(text) if not m[0].startswith(("/*", "//", '"', "'"))]
    hits = []

    stack = [[]]
    paren = 0
    for i, m in enumerate(toks):
        t = m[0]
        if t == "(":
            paren += 1
        elif t == ")":
            paren -= 1
        elif paren:
            continue
        elif t == "{":
            stack.append([])
        elif t == "}":
            stack.pop() if len(stack) > 1 else None
        elif t == "if":
            prev = toks[i - 1][0] if i else ""
            # find the token after the condition's closing paren: braced body or not
            j, depth = i + 1, 0
            while j < len(toks):
                depth += (toks[j][0] == "(") - (toks[j][0] == ")")
                j += 1
                if depth == 0:
                    break
            braced = j < len(toks) and toks[j][0] == "{"
            stack[-1].append((indent_of(m.start()), m.start(), braced))
        elif t == "else":
            ifs = stack[-1]
            if not ifs:
                continue
            ind, pos, braced = ifs.pop()
            if not braced and ind > indent_of(m.start()):
                hits.append((line_of(m.start()), line_of(pos)))
            # an else-if chain keeps the enclosing ifs; plain else closes this if only
    return hits


def main(files):
    bad = 0
    for f in files:
        for else_line, if_line in scan(f):
            bad += 1
            print(f"{f}:{else_line}: else binds to if at line {if_line} printed deeper (possible dangling else)")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
