#include "comunication.h"

int client_receive_id(int client_socket)
{
    // Se obtiene solamente el ID del mensaje
    int id = 0;
    recv(client_socket, &id, 1, 0);
    return id;
}

char *client_receive_payload(int client_socket)
{
    // Se obtiene el largo del payload
    int len = 0;
    recv(client_socket, &len, 1, 0);
    // Se obtiene el payload
    char *payload = malloc(len);
    int received = recv(client_socket, payload, len, 0);
    // Se retorna
    return payload;
}

void client_send_message(int client_socket, int pkg_id, char *message)
{
    int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo.
    //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.

    // Se arma el paquete
    char msg[1 + 1 + payloadSize];
    msg[0] = pkg_id;
    msg[1] = payloadSize;
    memcpy(&msg[2], message, payloadSize);
    // Se envía el paquete
    //printf("envio un mensaje al servidor mes: %s, id:%d\n", message, pkg_id);
    send(client_socket, msg, 2 + payloadSize, 0);
}

void get_img(int client_socket){
    FILE * _img = fopen("server_image.jpg", "wb");
    while (1) {
        unsigned char paquetes_totales;
        unsigned char actual;
        unsigned char tamano;
        recv(client_socket, &paquetes_totales, 1, 0);
        recv(client_socket, &actual, 1, 0);
        recv(client_socket, &tamano, 1, 0);
        char * buffer = malloc(tamano);
        recv(client_socket, buffer, tamano, 0);
        fwrite(buffer, 1, tamano, _img);
        free(buffer);
        if (paquetes_totales == actual){
        break;
        }
      unsigned char ID;
      recv(client_socket, &ID, 1, 0);
    }

    fclose(_img);
}