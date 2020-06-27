#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "comunication.h"

typedef struct players_info
{
    int socket_c1;
    int socket_c2;
    char* name_c1;
    char* name_c2;
    int posicion_c1;
    int posicion_c2;
    int ganador;
    int perdedor;
    int largo_tablero;

    // Juego
    int meritos_c1;
    int estrellas_c1;
    int poderes_c1[3];
    int poderes_act_c1;
    int pierde_turno_c1;

    int meritos_c2;
    int estrellas_c2;
    int poderes_c2[3];
    int poderes_act_c2;
    int pierde_turno_c2;


} PlayersInfo;

PlayersInfo *prepare_sockets_and_get_clients(char *IP, int port);
