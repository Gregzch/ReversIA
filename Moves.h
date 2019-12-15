#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include "main.h"
void fillcase(char tab[], const int sizex, const int sizey,SOCKET sock);
void checkmove(int **array, const int sizex, const int sizey,SOCKET sock);
void DivisionEuclidienne(int nombre, int tab[8]);
void choosemove(int **array, const int sizex, const int sizey,SOCKET sock, int **board);
#endif // MOVES_H_INCLUDED
