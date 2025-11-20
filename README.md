# MiniVM

A minimal virtual machine created for didactic purposes.

The specification is in the [VM source](minivm.c).

There is also an [assembler](mvmc) written in AWK,
supporting labels and some additional directives,
see [an example](src/hello.src).

The virtual machine is simple, but quite expressive - it can
[interpret Brainfuck](src/bf.src), for example.
Even [recursion](src/hanoi.src) can be done
with a little magic.

![Some examples](img/demos.png)

The 240-byte file size limit is just enough for a "Code 39" (ISO/IEC 16388:2023)
barcode generator (at most 15 characters):

![Barcode example](img/code39-demo.png)

It can display the [bifurcation diagram of the logistic map](src/logmap.src),
despite the lack of floating point numbers (or multiplication):

![LogMap example](img/logmap-demo.png)

A little [PostScript program](punchcard.eps) generates punch cards for
any program you write (see [an example](punchcard.pdf)).

There is a [disassembler](disasm.c) written by an AI - but it can be
done even in the machine itself: a [quine example](src/quine.src)
prints its own (disasembled) source code.

## The Next Step?

For a not-so-minimal version, consider (for example) the following:


Consider using a larger cell size (16 or 32 bit). To keep it concise,
an instruction and the associated parameter can be encoded in one cell
(the opcode in the high bits, for example). Using 4 bits for the
instructions, you can index 2^(16-4) = 4096 cells, and there is also
space to create 6 new instructions! (Similarly for the 32 bit version.)

|              | 16-bit version             | 32-bit version             |
|--------------|----------------------------|----------------------------|
| Layout       | 4 bit opcode + 12 bit data | 6 bit opcode + 26 bit data |
| Memory       | 4K (4096) cells = 8 KB     | 64M cells = 256 MB         |
| Instructions | 16                         | 64                         |

For new instructions, some recommendations are:

- Direct memory read (`SET`'s pair): `LOAD`

- Jumps, e.g. `JMP`, `JZ`, also indirect jump `JTV` (jump-to-value)

- Arithmetics, e.g. `MUL`, `DIV`, `MOD`

- Bit manipulations, e.g. `NAND`, `AND`, `OR`, `XOR`, `LSH`, `RSH`

In the 32-bit version direct/indirect versions of all instructions can be added.
