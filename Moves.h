#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
void fillcase(char tab[], const int sizex, const int sizey);
void checkmove(int **array, const int sizex, const int sizey);
void negativeConvertDecimalToBinary(int n, int tab[]);
void positiveConvertDecimalToBinary(int n, int tab[]);
#endif // MOVES_H_INCLUDED
