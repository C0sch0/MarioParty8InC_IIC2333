#include "comunication.h"

//LINKS REFERENCIAS
//https://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
//https://pubs.opengroup.org/onlinepubs/009695399/functions/send.html

int server_receive_id(int client_socket)
{
    // Se obtiene solamente el ID del mensaje
    int id = 0;
    recv(client_socket, &id, 1, 0);
    //printf("id = %d\n", id);
    return id;
}

char *server_receive_payload(int client_socket)
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

void server_send_message(int client_socket, int pkg_id, char *message)
{
    int payloadSize = strlen(message) + 1;
    //printf("payload size: %d\n", payloadSize);
    // Se arma el paquete
    char msg[1 + 1 + payloadSize];
    msg[0] = pkg_id;
    msg[1] = payloadSize;
    memcpy(&msg[2], message, payloadSize);
    // Se envía el paquete
    send(client_socket, msg, 2 + payloadSize, 0);
}


void enviar_segmento_img(int socket, int paquetes_total, int indice_paquete, int tamano, FILE * file){
  unsigned char * segmento = malloc(4);
  segmento[0] = 64;
  segmento[1] = paquetes_total;
  segmento[2] = indice_paquete;
  segmento[3] = tamano;
  printf("aqui toy 1\n");
  send(socket, segmento, 4, 0);

  char * buffer = malloc(tamano);
  printf("aqui toy 2\n");

  fread(buffer, 1, tamano, file);
  printf("aqui toy 3\n");

  send(socket, buffer, tamano, 0);
  printf("aqui toy 4\n");
  
  printf("enviando segmento img\n");
}

void send_img(int winner, int loser){
    int tamano_img_winner = 61009;
    FILE * img_winner = fopen("images/winner.png", "rb");
    if (img_winner){
        int paquetes_totales = tamano_img_winner / 255;
        int ultimo_paquete = tamano_img_winner % 255;
        if (ultimo_paquete != 0){
            paquetes_totales++;
        } 
        for (int i = 1; i<paquetes_totales; i++){
            enviar_segmento_img(winner, paquetes_totales, i, 255, img_winner);
        }
        if (ultimo_paquete != 0){
            enviar_segmento_img(winner, paquetes_totales, paquetes_totales, ultimo_paquete, img_winner);
        }
        else {
            enviar_segmento_img(winner, paquetes_totales, paquetes_totales, 255, img_winner);
        }
    }
    else{
        printf("img not found !!\n");
    }

    fclose(img_winner);


    int tamano_img_loser = 49571;
    FILE * img_loser = fopen("images/loser.png", "rb");
    if (img_loser){
        int paquetes_totales = tamano_img_loser / 255;
        int ultimo_paquete = tamano_img_loser % 255;
        if (ultimo_paquete != 0){
            paquetes_totales++;
        } 
        for (int i = 1; i<paquetes_totales; i++){
            enviar_segmento_img(winner, paquetes_totales, i, 255, img_loser);
        }
        if (ultimo_paquete != 0){
            enviar_segmento_img(winner, paquetes_totales, paquetes_totales, ultimo_paquete, img_loser);
        }
        else {
            enviar_segmento_img(winner, paquetes_totales, paquetes_totales, 255, img_loser);
        }
    }
    else{
        printf("img not found !!\n");
    }

    fclose(img_loser);



}