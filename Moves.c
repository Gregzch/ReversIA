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
    choosemove(verify,sizex,sizey,sock,array);
}
void choosemove(int **array, const int sizex, const int sizey,SOCKET sock, int **board)
{
    int i,j;
    int k = 155;
    char movex = 255,movey = 255;
    for(i=0; i<sizey; i++)
    {
        for(j=0; j<sizex; j++)
        {
            if(array[i][j] == 1)
            {
                array[i][j] += checkadv(board,array,sizex,sizey,j,i);
                if(testBord(i, j, sizex, sizey) == 1)
                            array[i][j] += 8;
            }
        }
    }
    for(i=0; i<sizey; i++)
    {
        for(j=0; j<sizex; j++)
        {
            if(array[i][j]!=0 && k > array[i][j])
            {
                    k = array[i][j];
                    movey = i;
                    movex = j;
                }
            }
        }

    movemessage(sock, movex, movey);

}

int checkadv(int **board, int **moves,const int sizex, const int sizey, int x, int y)
{
    int i,j,advcolor;
    int a,b;
    char verif;
    ///On définit la couleur des pions adverse
    if(color == 10)
    {
        advcolor = 1;
    }
    else
        advcolor = 10;

    ///Vérification des possibilités sur la ligne
    ///On vérifie que l'on possède un autre pion sur la ligne
    verif = 0;
    for(i=0; i<sizex; i++)
        if(board[y][i] == color)
            verif = 1;
    if(x>0 && board[y][x-1] == advcolor && verif == 1) //Si il existe une case à gauche
    {
        a=x-1;
        while(board[y][a]==advcolor && a > 0)
        {
            board[y][a] = color;
            a--;
        }
    }
    else if(x<sizex-1 && board[y][x+1] ==advcolor && verif == 1)
    {
        a = x+1;
        while(board[y][a] == advcolor && a < sizex-1)//Tant que les pieces voisines sont celle adverses
        {
            board[y][a] = color;
            a++;
        }
    }
    verif = 0;
    for(i=0; i<sizey; i++)
        if(board[i][x] == color)
            verif = 1;
    ///Vérification des possibilités sur la colonne
    if(y>0 && board[y-1][x] == advcolor && verif == 1)//Vers le haut
    {
        a = y-1;
        while(board[a][x] == advcolor && a>0)
        {
            board[a][x] = color;
            a--;
        }

    }
    else if(y<sizey-1 && board[y+1][x] == advcolor && verif == 1)//Vers le bas
    {
        a = y+1;
        while(board[a][x] == advcolor && a<sizey-1)
        {
            board[a][x] = color;
            a++;
        }
    }

    ///On vérifie les possibilités sur les diagonales
    verif = 0;
    i = y;
    j = x;
    while(j<sizex && i < sizey)
        {
            if(board[i][j] == color)
                verif = 1;
            i++;
            j++;
        }
    if(y<sizey-1 && x < sizex-1 && board[y+1][x+1] == advcolor && verif == 1)//Diagonale bas droite
    {
        a = y+1;
        b = x+1;
        while(board[a][b] == advcolor && a <sizey-1 && b < sizex-1)
        {
            board[a][b] = color;
            a++;
            b++;
        }
    }/*
    verif = 0;
    i = y;
    j = x;
    while(j>0 && i < sizey)
        {
            if(board[i][j] == color)
                verif = 1;
            i++;
            j--;
        }
     if(y<sizey-1 && x > 0 && board[y+1][x-1] == advcolor)//Diagonale bas gauche
    {
        board[a][b] = color;
        a = y+1;
        b = x-1;
        while(board[a][b] == advcolor && a <sizey-1 && b > 0)
        {
            board[a][b] = color;
            a++;
            b--;
        }

    }
     /*if(y> 0 && x > 0 && board[y-1][x-1] == advcolor)//Diagonale haut gauche
    {
        a = y-1;
        b = x-1;
        while(board[a][b] == advcolor && a >0 && b > 0)
        {
            board[a][b] = color;
            a--;
            b--;
        }
    }

     if(y> 0 && x <sizex-1 && board[y-1][x+1] == advcolor)//Diagonale haut droite
    {
        a = y-1;
        b = x+1;
        while(board[a][b] == advcolor && a >0 && b < sizex - 1)
        {
            board[a][b] = color;
            a--;
            b++;
        }
    }*/
    board[y][x] = color;
    int cmpt = 0;
    ///Calcul du nombre de coups possibles
    ///La stratégie étant de limiter le nombre de coups de l'adversaire
    for(i=0; i < sizey; i++)
    {
        for(j=0; j < sizex; j++)
            if(board[i][j] == advcolor)//Si la case est occupée par un pion adverse
            {
                ///Vérification des possibilités sur la ligne
                if(j>0 && board[i][j-1] == color) //Si il existe une case à gauche
                {
                    a=j-1;
                    while(board[i][a]==color && a > 0)
                    {
                        a--;
                    }
                    if(board[i][a] == 0)
                        cmpt++;
                }
                if(j<sizex-1 && board[i][j+1] ==color)
                {
                    a = j+1;
                    while(board[i][a] == color && a < sizex-1)//Tant que les pieces voisines sont celle adverses
                    {
                        a++;
                    }
                    if(board[i][a] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }
                ///Vérification des possibilités sur la colonne
                if(i>0 && board[i-1][j] == color)//Vers le haut
                {
                    a = i-1;
                    while(board[a][j] == color && a>0)
                    {
                        a--;
                    }
                    if(board[a][j] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }
                if(i<sizey-1 && board[i+1][j] == color)//Vers le bas
                {
                    a = i+1;
                    while(board[a][j] == color && a<sizey-1)
                    {
                        a++;
                    }
                    if(board[a][j] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }

                ///On vérifie les possibilités sur les diagonales
                if(i<sizey-1 && j < sizex-1 && board[i+1][j+1] == color)//Diagonale bas droite
                {
                    a = i+1;
                    b = j+1;
                    while(board[a][b] == color && a <sizey-1 && b < sizex-1)
                    {
                        a++;
                        b++;
                    }
                    if(board[a][b] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }
                if(i<sizey-1 && j > 0 && board[i+1][j-1] == color)//Diagonale bas gauche
                {
                    a = i+1;
                    b = j-1;
                    while(board[a][b] == color && a <sizey-1 && b > 0)
                    {
                        a++;
                        b--;
                    }
                    if(board[a][b] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }
                if(i> 0 && j > 0 && board[i-1][j-1] == color)//Diagonale haut gauche
                {
                    a = i-1;
                    b = j-1;
                    while(board[a][b] == color && a >0 && b > 0)
                    {
                        a--;
                        b--;
                    }
                    if(board[a][b] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }
                if(i> 0 && j <sizex-1 && board[i-1][j+1] == color)//Diagonale haut droite
                {
                    a = i-1;
                    b = j+1;
                    while(board[a][b] == color && a >0 && b < sizex - 1)
                    {
                        a--;
                        b++;
                    }
                    if(board[a][b] == 0)//Si la derniere piece du plateau est libre
                        cmpt++;
                }

            }


    }
    // printf("Coup joue x :%d  y :%d \nCoup : %d\n",x,y,cmpt);
    if(cmpt == 0)
        cmpt = 1;
    return cmpt;
}



void DivisionEuclidienne(int nombre, int tab[8])// Des pointeurs pour modifier les deux valeurs
{
    int quotient = 1, reste = 1;
    int i = 0;
    int signe = 0;

    for(i = 0; i < 8; i++)
        tab[i] = 0;

    if(nombre < 0)
    {
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

    if(signe == 1)
    {
        for(i = 0; i < 8; i++)
        {
            if(tab[i] == 1)
                tab[i] = 0;
            else
                tab[i] = 1;
        }

        tab[0] = 1;
    }

}
int testBord(int x, int y, int sizeX, int sizeY){
    int tab[sizeX][sizeY];
    sizeX -= 1;
    sizeY -= 1;

    int i , j;

    for(i = 0; i < sizeX; i ++){
        for(j = 0; j < sizeY; j ++)
            tab[i][j] = 0;
    }

    tab[1][0] = 1;
    tab[sizeX - 2][0] = 1;
    tab[0][1] = 1;
    tab[1][1] = 1;
    tab[sizeX - 2][1] = 1;
    tab[sizeX - 1][1] = 1;
    tab[0][sizeY - 2] = 1;
    tab[1][sizeY - 2] = 1;
    tab[sizeX - 2][sizeY - 2] = 1;
    tab[sizeX - 1][sizeY - 2] = 1;
    tab[1][sizeY - 1] = 1;
    tab[sizeX - 2][sizeY - 1] = 1;

    if(tab[x][y] == 1)
        return 1;
    else
        return 0;

}

