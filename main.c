

#include <stdlib.h>
#include <sys/types.h>

#define MINGW32
#ifdef MINGW32
// Libraries for Windows
#include<winsock2.h>
#include <windows.h>
#include <time.h>
#else
// Libraries for Linux

#endif // MINGW32
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
//typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


// messages constants
#define BUFFER_SIZE 1024
#define PORT 8888
#define IP_SERVER 127.0.0.1
#define MAX_CLIENT 1
#define BLACK_PLAYER 0x01
#define WHITE_PLAYER 0x02
#define NOK_RESPONSE 0x00
#define OK_RESPONSE 0x01
#define SYNCRO 0x55
#define FIELDSIZE 8


#define OKNOK_MESSAGE 0x02
#define CONNECT_MESSAGE 0x01
#define PLAYEROK_MESSAGE 0x10
#define NEWMOVE_MESSAGE 0x03
#define END_MESSAGE 0x04
#define NEXTTURN_MESSAGE 0x05
#define STATUS1_MESSAGE 0x06
#define STATUS2_MESSAGE 0x07
#define CONTROL_MESSAGE 0x08
#define PING_MESSAGE 0x11
///192.168.0.100 IP de l'IA prof
int color = 10;

typedef struct
{
    int port;
    char name;
} Client;

typedef struct
{
    SOCKET sock;
    char name[BUFFER_SIZE];
} Cliente;


int init_connection(void)
{
    SOCKET sock_client;

    SOCKADDR_IN server;
    WSADATA wsa;

    printf("\nInitialising Winsock...");
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }


    //Create socket
    sock_client = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_client == INVALID_SOCKET)
    {
        printf("Could not create socket");
        exit (EXIT_FAILURE);
    }
    puts("Client Socket created");

    // Define the socket remote address : 192.168.0.111:8888
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);


    //Connect to remote server
    if (connect(sock_client, (SOCKADDR *) &server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        exit (EXIT_FAILURE);
    }
    puts("Connected\n");


    return sock_client;
}

void sendMessage(SOCKET sock, const char *ecrit, int nbOctet)
{
    //Variables locales
    int l = 0;
    int fin;

    //En cas d'erreur lors de l'envoi
    if (send (sock, ecrit, nbOctet, 0) < 0)
    {
        printf ("Error : When sending new message\n");

    }

    //Affichage du message envoyé
    printf("Message sent : ");
    fin = (int) ecrit[1]+4;
    for (l = 0; l < fin ; l++)
    {
        printf(" %d ", ecrit[l]);
    }
    printf("\n");

}

int connect_message(SOCKET sock, char *name)
{

    //Variables locales
    char message[100], server_reply[2000];
    int e, i, playerColor;
    char CRC = 0;



    // Create the "Connect Message"
    message[0] = SYNCRO;  // Synchro
    message[1] = strlen(name)+1; // Message Length


    message[2] = CONNECT_MESSAGE;  // Connect message
    message[3] = strlen(name);  // Name Length

    //Récupération du nom
    for(i=4; i < (strlen(name)+4); i++)
        message[i] = name[i-4];


    //Calcul du CRC lors du choix du nom
    for (e = 2; e < (strlen(name)+4); e++)
        CRC += message[e];
    message[strlen(name)+4] = CRC;

    // Send "connect message" to the GM
    sendMessage(sock, message, (strlen(name)+5));
}

int readbegin(SOCKET sock)
{
    char first[5];
    int i;
    recv(sock,first,10,0);//Reception de la couleur du joueur et début de la partie
    printf("Message received (5): ");
    for(i = 0; i<5; i++ )
    {
        printf(" %d ",first[i]);
    }
    if(first[3] == 1)
    {
        printf("\nConnect Message Ok - Black Player");
        color = 1;
    }
    else
    {
        printf("\nConnect Message Ok - White Player\n");
        color = 10;
    }
}

void turn(SOCKET sock)
{
    char server_reply[1000];
    if((recv(sock, server_reply, 1000, 0)) < 0)
    {
        printf("Read failed");
    }
    else
    {
        int i;
        int taille = ((server_reply[5]*server_reply[6])/4) + 8;
        printf("\nMessage received (%d): ",taille);

        for(i = 0; i<taille; i++ )
        {
            printf(" %d ",server_reply[i]);
        }
    }
    ///85 Synchro
    ///taille du message
    ///Type de message
    ///car[5] sizex 6 size y 8 la board
    ///01 black ///10 white
    //convertDecimalToBinary(server_reply,server_reply[5],server_reply[6]);

}


void fillcase(/*int **array,*/ const int sizex, const int sizey)
{
    int i,j;
    int **cases = malloc(sizeof(int*) * sizey);
    for(i = 0; i < sizey; i++)
        cases[i] = malloc(sizeof(int) * sizex);
    int array[sizey][sizex];
    for(i=0; i<sizex; i++)
    {
        for(j=0; j<sizey; j++)
        {
            array[i][j] = 0;

        }

    }
    array[3][3] = 1;
    array[3][4] = 1;
    array[3][5] = 10;
    array[4][4] = 10;
    array[4][5] = 1;
    array[4][3] = 10;
    for(i=0; i<sizey; i++)
    {
        for(j=0; j<sizex; j++)
        {
            cases[i][j] = array[i][j];
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



//inet_addr
int main(int argc, char *argv[])
{
    /*int start;
    SOCKET sock_client = init_connection();
    connect_message(sock_client, "Gregz");
    //do{
    start = readbegin(sock_client);
    turn(sock_client);
    closesocket(sock_client);
    //}while(start!=1);*/
    fillcase(8,8);


    return 0;
}
