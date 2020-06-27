#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int server_receive_id(int client_socket);
char *server_receive_payload(int client_socket);
void server_send_message(int client_socket, int pkg_id, char *message);
void enviar_segmento_img(int socket, int paquetes_total, int indice_paquete, int tamano, FILE * file);
void send_img(int winner, int loser);