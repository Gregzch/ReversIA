#include "Moves.h"
#include "main.h"

void fillcase(char tab[], const int sizex, const int sizey,SOCKET sock)
{
    int i,j,cmpt = 0,k=0;
    int binaire[8];
    int **cases = malloc(sizeof(int*) * sizey);
    for(i = 0; i < sizey; i++)
        cases[i] = malloc(sizeof(int) * sizex);
    int nb;
     for(i=0; i<sizey; i++)
    {
        for(j=0; j<sizex; j++)
        {
            if(cmpt == 4) /// Chaque nombre reprèsente 4 cases
            {
                k++;
                cmpt = 0;
            }
            if(cmpt == 0)///Nouveau chiffre à convertir toutes les 4 cases
                {
                 nb = (int)tab[7+k];
                 DivisionEuclidienne(nb,binaire);
                }
        if(binaire[cmpt*2] == 0 && binaire[cmpt*2+1] ==1)
            cases[i][j] = 1;
        else if(binaire[cmpt*2] == 1 && binaire[cmpt*2+1] ==0)
            cases[i][j] = 10;
        else
            cases[i][j] = 0;
            cmpt++;
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
    checkmove(cases,sizex,sizey,sock);
}


void checkmove(int **array, const int sizex, const int sizey,SOCKET sock)
{

    int i,j,advcolor,a,b;
            int **verify = malloc(sizeof(int*) * sizey);
    for(i = 0; i < sizey; i++)
        verify[i] = malloc(sizeof(int) * sizex);
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
                    while(array[i][a]==advcolor && a > 0)
                    {

                        a--;
                    }
                    if(array[i][a] == 0)
                        verify[i][a] = 1;
                }
                if(j<sizex-1 && array[i][j+1] ==advcolor)
                {
                    a = j+1;
                    while(array[i][a] == advcolor && a < sizex-1)//Tant que les pieces voisines sont celle adverses
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
                    while(array[a][j] == advcolor && a>0)
                    {
                        a--;
                    }
                    if(array[a][j] == 0)//Si la derniere piece du plateau est libre
                        verify[a][j] = 1;
                }
                if(i<sizey-1 && array[i+1][j] == advcolor)//Vers le bas
                {
                    a = i+1;
                    while(array[a][j] == advcolor && a<sizey-1)
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
                    while(array[a][b] == advcolor && a <sizey-1 && b < sizex-1)
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
                    while(array[a][b] == advcolor && a <sizey-1 && b > 0)
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
                    while(array[a][b] == advcolor && a >0 && b > 0)
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
                    while(array[a][b] == advcolor && a >0 && b < sizex - 1)
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
            printf(" %d ",verify[i][j]);
    }
    choosemove(verify,sizex,sizey,sock);
}
void choosemove(int **array, const int sizex, const int sizey,SOCKET sock)
{
    int i,j;
    char movex = -1,movey = -1;
    for(i=sizey-1;i>=0;i--)
        {
            for(j=sizex-1;j>=0;j--)
                {
                if(array[i][j] == 1)
                    {
                        movex = j;
                        movey = i;
                    }
                }
        }
movemessage(sock, movex, movey);

}

void DivisionEuclidienne(int nombre, int tab[8])// Des pointeurs pour modifier les deux valeurs
{
    int quotient = 1, reste = 1;
    int i = 0;
    int signe = 0;

    for(i = 0; i < 8; i++)
        tab[i] = 0;

    if(nombre < 0){
        nombre = abs(nombre) + 127;
        signe = 1; //si nombre n gatif
    }
    i=0;
    while(quotient != 0)
    {
        reste = nombre%2;
        quotient = nombre/2 ;
        tab[i] = reste;
        i++;
        nombre = quotient;
        reste = 0;
    }



    int conv[8];
    int j = 7;
    for(i = 0; i < 8; i++)
    {
        conv[j] = tab[i];
        j--;
    }

    for(i=0; i< 8; i ++)
    {
        tab[i] = conv[i];
    }

    if(signe == 1){
        for(i = 0; i < 8; i++){
            if(tab[i] == 1)
                tab[i] = 0;
            else
                tab[i] = 1;
        }

        tab[0] = 1;
    }

}

