#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"


char* get_input()
{
    char *response = malloc(255);
    int pos = 0;
    while (1)
    {
        char c = getchar();
        if (c == '\n')
            break;
        response[pos] = c;
        pos++;
    }
    response[pos] = '\0';
    return response;
}

char* leer_tablero(char* ruta_archivo)
{
  FILE* tablero = fopen(ruta_archivo, "r");
  if (!tablero)
  {
    printf("Ingrese direccion tablero correcta\n");
    return NULL;
  }
  int num_casillas = 0;
  fscanf(tablero, "%d", &num_casillas);
  char *tab = malloc(num_casillas + 1);
  fscanf(tablero, "%s", tab);
  return tab;
}

void print_tablero(char* mensaje)
{
  char* posic_1 = strtok(mensaje, " ");
  int p1 = atoi(posic_1);
  char* posic_2 = strtok(NULL, " ");
  int p2 = atoi(posic_2);
  char* tablero = strtok(NULL, " ");
  int len = strlen(tablero);
  printf("--- TABLERO ACTUAL ---\n");
  for(int i = 1; i <= len; i++)
  {
    printf("-");
  }
  printf("\n");
  for(int i = 1; i <= len; i++)
  {
    if(i == p1)
    {
      printf("\033[0;31m");
      printf("1");
      printf("\033[0m");
    }
    else
    {
      printf(" ");
    }
  }
  printf("\n");

  printf("%s\n",tablero);

  for(int i = 1; i <= len; i++)
  {
    if(i == p2)
    {
      printf("\033[0;34m");
      printf("2");
      printf("\033[0m");
    }
    else
    {
      printf(" ");
    }

  }
  printf("\n\n");

  for(int i = 1; i <= len; i++)
  {
    printf("*");

  }
  printf("\n");
  for(int i = 1; i <= len; i++)
  {
    printf("-");
  }
  printf("\n\n");
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
      printf("Input debe coindicir con: ./server -i <ip_address> -p <tcp_port>\n");
      return 0;
    }

    char* IP = argv[2];
    int PORT = atoi(argv[4]);

    // Se prepara el socket
    int server_socket = prepare_socket(IP, PORT);

    // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
    while (1)
    {
        int msg_code = client_receive_id(server_socket);

        if(msg_code == 0)
        {//primer mensaje vacio y retornamos el nombre del jugador
          char *message = client_receive_payload(server_socket);
          free(message);
          printf("¿Cual es su nombre?\n");
          char *response = get_input();
          client_send_message(server_socket,0, response);

          free(response);

        }

        if (msg_code == 1)
        {   //Recibimos un mensaje de bienvenida del servidor
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);

            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket,3, empty);

            free(empty);
        }

        if (msg_code == 2)
        {  //Nos piden el tablero txt
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);
            char* tablero = NULL;
            while(!tablero){
              char *response = get_input();
              tablero = leer_tablero(response);
              free(response);
            }

            client_send_message(server_socket,3, tablero);
            free(tablero);
        }

        if (msg_code == 3)
        {
            // para cambiar de turno!
            char *message = client_receive_payload(server_socket);
            free(message);
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket,3, empty);

            free(empty);

        }

        if (msg_code == 4)
        {   //Recibimos el tablero
            char *message = client_receive_payload(server_socket);
            print_tablero(message);
            free(message);
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket,4, empty);

            free(empty);
        }

        if (msg_code == 5)
        {   //Recibimos la información de nuestro estado como jugador y el tablero
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket,5, empty);

            free(empty);
        }

        if (msg_code == 6)
        {   //Recibimos las opciones
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);
            // El jugador elije una jugada y se la envía al Servidor
            int jugada = getchar() - '0' + 5;
            getchar();
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket, jugada, empty);

            free(empty);
        }
        if (msg_code == 7)
        {   //Recibimos mensaje final y se acaba el juego
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);
            break;
        }
        if (msg_code == 8)
        {   //Recibimos la pregunta de las casillas
            char *message = client_receive_payload(server_socket);
            printf("%s", message);
            free(message);
            char* jugada = get_input();
            client_send_message(server_socket, 12, jugada);

            free(jugada);
        }

        if (msg_code == 9)
        {   //Recibimos el tablero
            char *message = client_receive_payload(server_socket);
            print_tablero(message);
            free(message);
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket,1, empty);

            free(empty);
        }

        if (msg_code == 10)
        {   //Recibimos la información de nuestro estado como jugador y el tablero
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            printf("\n\n\n AHORA TIENES QUE ESPERAR EL TURNO DEL OTRO JUGADOR ...\n\n");
            client_send_message(server_socket,1, empty);

            free(empty);
        }

        if (msg_code == 11)
        {
            // para cambiar de turno cuando perdieron el turno
            char *message = client_receive_payload(server_socket);
            printf("%s\n", message);
            free(message);
            char* empty = malloc(255);
            char* requ = "";
            strcpy(empty, requ);
            client_send_message(server_socket,3, empty);

            free(empty);

        }
        if (msg_code == 12)
        {
          /* Nos han enviado un duelo */
          char *message = client_receive_payload(server_socket);
          printf("%s\n", message);
          free(message);
          printf("=========== duelo ===========\n");
          printf("1. PIEDRA  2. PAPEL 3. TIJERA ?\n");
          printf("=========== ====== ===========\n");
          char *response = malloc(sizeof(char));
          response = get_input();
          printf("INGRESA RESPUESTA (1,2,3):\n");
          printf("Has elegido: %s y ahora veremos los resultados...\n", response);
          while((response[0] != '1') && (response[0] != '2') && (response[0] != '3')){
            response = get_input();
            printf("INGRESA RESPUESTA (1,2,3):\n");
            printf("Has elegido: %s y ahora veremos los resultados...\n", response);
          }
          client_send_message(server_socket, 13, response);
          free(response);
        }

        if (msg_code == 13)
        {
          /* Imprime esto */
          char *message = client_receive_payload(server_socket);
          printf("%s\n", message);
          free(message);
        }
        // if (msg_code == 14)
        // {
        //     char *message = client_receive_payload(server_socket);
        //     printf("%s\n", message);
        //     free(message);
        //     char* empty = malloc(255);
        //     char* requ = "";
        //     strcpy(empty, requ);
        //     client_send_message(server_socket, 4, empty);
        //
        //     free(empty);
        // }
        if (msg_code == 15)
        {
          /* Tienda - Elegir poder */
          char *message_ = client_receive_payload(server_socket);
          free(message_);
          char *response_;
          printf("QUE PODER QUIERES ? 1.Lanzar Doble 2.Robar Meritos 3.Duelo\n");
          while((response_[0] != '1') && (response_[0] != '2') && (response_[0] != '3')){
            response_ = get_input();
            printf("INGRESA RESPUESTA (1,2,3):\n");
            printf("Has elegido: %s \n", response_);
          }
          client_send_message(server_socket, 14, response_);
          free(response_);
        }
        else if (msg_code == 64){
          get_img(server_socket);
        }
    }

    // Se cierra el socket
    close(server_socket);
    return 0;
}
