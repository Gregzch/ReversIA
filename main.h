#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
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
int color;

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


#endif // MAIN_H_INCLUDED
