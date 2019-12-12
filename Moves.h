#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include "main.h"
void fillcase(char tab[], const int sizex, const int sizey,SOCKET sock);
void checkmove(int **array, const int sizex, const int sizey,SOCKET sock);
void negativeConvertDecimalToBinary(int n, int tab[]);
void positiveConvertDecimalToBinary(int n, int tab[]);
void choosemove(int **array, const int sizex, const int sizey,SOCKET sock);
#endif // MOVES_H_INCLUDED
