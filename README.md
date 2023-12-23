# Rush Hour Solver

It is a solver of a sliding block puzzle [Rush Hour](https://en.wikipedia.org/wiki/Rush_Hour_(puzzle)) implemented in C++ with fltk. This project is the second project of SJTU 2023 Fall Course CS2309.

## To do:

use array of row/columns bit vector to promote the performance of the program and cut the length of the code

Separate class Solver into several classes such as Board, Grid, BitVector

Consider whether it's good to use a bit to divide 3+2 and 2+3

rowCnt & colCnt is unnecessary to store in every bitBoard but just in the whole Solver as they never change
