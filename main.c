
#include "main.h"
#include "Moves.h"



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
    printf("\nMessage sent : ");
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

void movemessage(SOCKET sock, char movex, char movey)
{
    int i;
    char message[200];
    char CRC = 0;
    message[0] = SYNCRO;  // Synchro
    message[1] = 2; // Message Length
    message[2] = NEWMOVE_MESSAGE;  // new move message
    message[3] = movex;
    message[4] = movey;
    for (i = 2; i < 5; i++)
        CRC += message[i];
    message[5] = CRC;

    sendMessage(sock,message,6);
}


void readbegin(SOCKET sock)
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

void read_sendOK(SOCKET sock)
{
    char server_reply[5];
    char message[5];
    int i;
    char CRC = 0;
    if((recv(sock, server_reply, 5, 0)) < 0)
    {
        printf("\nRead failed");
    }
    else
    {
        printf("\nMessage received (5) : ");
        for(i=0;i<5;i++)
            {
               printf("%d ",server_reply[i]);
            }
        message[0] = SYNCRO;
        message[1] = 1;
        message[2] = OKNOK_MESSAGE;
        message[3] = 1;
        for (i = 2; i < 4; i++)
        CRC += message[i];
    message[4] = CRC;
    sendMessage(sock,message,5);
    }
}

int turn(SOCKET sock)
{
    char server_reply[1000];
    if((recv(sock, server_reply, 1000, 0)) < 0)
    {
        printf("\nRead failed");
    }
    else
    {
        int i;
        int taille = server_reply[1]+4;
        printf("\nMessage received (%d): ",taille);

        for(i = 0; i<taille; i++ )
        {
            printf(" %d ",server_reply[i]);
        }
        if(taille > 5)
            {
        int sizex = server_reply[5];
        int sizey = server_reply[6];
        fillcase(server_reply,sizex,sizey,sock);
        read_sendOK(sock);
        return 0;
            }
            else
                return 1;

    }
    ///85 Synchro
    ///taille du message
    ///Type de message
    ///server_reply[5] sizex 6 sizey
    ///server_reply 8 le plateau
    ///01 black ///10 white

}





//inet_addr
int main(int argc, char *argv[])
{
    int i = 0;
    SOCKET sock_client = init_connection();
    connect_message(sock_client, "Gregz");
    readbegin(sock_client);
    do{

    i += turn(sock_client);
    }while(i<2);
    closesocket(sock_client);

    return 0;
}
