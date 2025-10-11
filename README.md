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
