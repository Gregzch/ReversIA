#include "Moves.h"
int color = 10;

void fillcase(char tab[], const int sizex, const int sizey)
{
    int i,j,cmpt = 0,k=0;
    int binaire[8];
    int **cases = malloc(sizeof(int*) * sizey);
    for(i = 0; i < sizey; i++)
        cases[i] = malloc(sizeof(int) * sizex);
    int array[sizey][sizex];
    int taille = (sizex*sizey) / 4;
    int nb;
    /* for(i=0;i<taille;i++)
         {
         nb = tab[7+i];

         }
     */for(i=0; i<sizey; i++)
    {
        for(j=0; j<sizex; j++)
        {
            cmpt++;
            if(cmpt == 4) /// Chaque nombre reprèsente 4 cases
            {
                k++;
                cmpt = 0;
            }
            nb = tab[7+k];
            if (nb >= 0)
                positiveConvertDecimalToBinary(nb,binaire);

            else
                negativeConvertDecimalToBinary(nb, binaire);
            if(binaire[cmpt*2] == 0 && binaire[cmpt*2+1] ==1) cases[i][j] = 1;
            else if(binaire[cmpt*2] == 1 && binaire[cmpt*2+1] ==0) cases[i][j] = 10;
            else if(binaire[cmpt*2] == 0 && binaire[cmpt*2+1] ==0) cases[i][j] = 0;
            }
    }

    for(i=0; i<sizex; i++)
    {
        printf("\n");
        for(j=0; j<sizey; j++)
        {
            if(cases[i][j]==10)
            printf("%d ",cases[i][j]);
                else
            printf(" %d ",cases[i][j]);
        }
    }
    checkmove(cases,sizex,sizey);

}

void checkmove(int **array, const int sizex, const int sizey)
{
    int verify[sizey][sizex];
    int i,j,advcolor,a,b;
    ///On définit la couleur des pions adverse
    if(color == 10)
    {
        advcolor = 1;
    }
    else
        advcolor = 10;
    ///Initialisation du tableau à 0
    for(i=0; i < sizey; i++)
    {
        for(j=0; j < sizex; j++)
        {
            verify[i][j] = 0;
        }
    }
    ///Insertion des coups possibles dans le tableau
    for(i=0; i < sizey; i++)
    {
        for(j=0; j < sizex; j++)
            if(array[i][j] == color)//Si la case est occupée par un de nos pions
            {
                ///Vérification des possibilités sur la ligne
                if(j>0 && array[i][j-1] == advcolor) //Si il existe une case à gauche
                {
                    a=j-1;
                    while(array[i][a]==advcolor && a >= 0)
                    {

                        a--;
                    }
                    if(array[i][a] == 0)
                        verify[i][a] = 1;
                }
                if(j<sizex-1 && array[i][j+1] ==advcolor)
                {
                    a = j+1;
                    while(array[i][a] == advcolor && a < sizex)//Tant que les pieces voisines sont celle adverses
                    {
                        a++;
                    }
                    if(array[i][a] == 0)//Si la derniere piece du plateau est libre
                        verify[i][a] = 1;
                }
                ///Vérification des possibilités sur la colonne
                if(i>0 && array[i-1][j] == advcolor)//Vers le haut
                {
                    a = i-1;
                    while(array[a][j] == advcolor && a>=0)
                    {
                        a--;
                    }
                    if(array[a][j] == 0)//Si la derniere piece du plateau est libre
                        verify[a][j] = 1;
                }
                if(i<sizey-1 && array[i+1][j] == advcolor)//Vers le bas
                {
                    a = i+1;
                    while(array[a][j] == advcolor && a<sizey)
                    {
                        a++;
                    }
                    if(array[a][j] == 0)//Si la derniere piece du plateau est libre
                        verify[a][j] = 1;
                }

                ///On vérifie les possibilités sur les diagonales
                if(i<sizey-1 && j < sizex-1 && array[i+1][j+1] == advcolor)//Diagonale bas droite
                {
                    a = i+1;
                    b = j+1;
                    while(array[a][b] == advcolor && a <sizey && b < sizex)
                    {
                        a++;
                        b++;
                    }
                    if(array[a][b] == 0)//Si la derniere piece du plateau est libre
                        verify[a][b] = 1;
                }
                if(i<sizey-1 && j > 0 && array[i+1][j-1] == advcolor)//Diagonale bas gauche
                {
                    a = i+1;
                    b = j-1;
                    while(array[a][b] == advcolor && a <sizey && b >= 0)
                    {
                        a++;
                        b--;
                    }
                    if(array[a][b] == 0)//Si la derniere piece du plateau est libre
                        verify[a][b] = 1;
                }
                if(i> 0 && j > 0 && array[i-1][j-1] == advcolor)//Diagonale haut gauche
                {
                    a = i-1;
                    b = j-1;
                    while(array[a][b] == advcolor && a >=0 && b >= 0)
                    {
                        a--;
                        b--;
                    }
                    if(array[a][b] == 0)//Si la derniere piece du plateau est libre
                        verify[a][b] = 1;
                }
                if(i> 0 && j <sizex-1 && array[i-1][j+1] == advcolor)//Diagonale haut droite
                {
                    a = i-1;
                    b = j+1;
                    while(array[a][b] == advcolor && a >=0 && b < sizex)
                    {
                        a--;
                        b++;
                    }
                    if(array[a][b] == 0)//Si la derniere piece du plateau est libre
                        verify[a][b] = 1;
                }

            }


    }
    printf("\n");
    for(i=0; i<sizey; i++)
    {
        printf("\n");
        for(j=0; j<sizex; j++)
            printf("% d ",verify[i][j]);
    }
}

void positiveConvertDecimalToBinary(int n, int tab[])
{
    printf("\nNombre a convertir : %d \n",n);
    long long binaryNumber = 0;
    int remainder, i = 1, step = 1,cmpt =7,j;
    if(n==0)
    {
        for(j=0; j<8; j++)
            tab[j] = 0;
    }
    while (n != 0)
    {
        remainder = n % 2;
        n /= 2;
        tab[cmpt] = abs(remainder);
        binaryNumber += remainder * i;
        i *= 10;
        cmpt--;
    }

    for(cmpt=0; cmpt<8; cmpt++)
        printf("%d",tab[cmpt]);
    printf("\n");
}


void negativeConvertDecimalToBinary(int n, int tab[])
{
    printf("Negative Convert : %d\n", n);
    long long binaryNumber = 0;
    int remainder, i = 1, step = 1, cmpt = 7;

    if (n == -128)
    {
        tab[0] = 1;

        for (cmpt; cmpt > 1; cmpt--)
            tab[cmpt] = 0;
    }

    else
    {
        n = abs(n);

        while (cmpt != -1)
        {
            remainder = n % 2;
            //printf("Step %d: %d/2, Remainder = %d, Quotient = %d\n", step++, n, remainder, n / 2);
            n /= 2;


            if (remainder == 0)
                tab[cmpt] = 1;
            else
                tab[cmpt] = 0;

            binaryNumber += remainder * i;
            i *= 10;

            cmpt--;
        }

        tab[0] = 1;
    }
    for(i=0; i<8; i++)
        printf("%d",tab[i]);
    printf("\n");

}
