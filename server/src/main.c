#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "comunication.h"
#include "conection.h"
#include <string.h>
#include <math.h>

int FIN = 0;

char* welcome_msg(char* msg)
{
  int len = strlen(msg)+ 1;
  char* hello = malloc(len + 12);
  char* texto = "Bienvenido ";
  memcpy(hello, texto, 11);
  memcpy(&hello[11], msg, len);
  return hello;
}

char* string_powers(int* poderes_actuales){
  char * string = malloc(60);
  strcpy(string, "");

  char poderes[3][20];
  int count = 0;

  for(int i = 0; i < 3; i++){

    if(poderes_actuales[i] == 0){
      strcpy(poderes[i], "Ninguno");

    }else if(poderes_actuales[i] == 1){
      strcpy(poderes[i], "Lanzar Doble");

    }
    else if(poderes_actuales[i] == 2){
      strcpy(poderes[i], "Robar méritos");

    }
    else if(poderes_actuales[i] == 3){
      strcpy(poderes[i], "Retar a Duelo");

    }
  }

  /* Append all the strings */
  for (int i = 0; i < 3; i++) {

    if (strcmp(poderes[i], "Ninguno") != 0){
      strcat(string, poderes[i]);
    }
    else{
      count ++;
    }

    if (i < 2 ){
      strcat(string, ", ");
    }

    if(count == 3){
      strcpy(string, "No tiene poderes");
    }
  }

  return string;
}
void enviar_tablero_actual(PlayersInfo* players_info, char* tablero, int jugador)
{
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  // char* pos = calloc(4, sizeof(char));
  char* pos = malloc(4*sizeof(char) + 5);
  sprintf(pos, "%d %d ", players_info->posicion_c1, players_info->posicion_c2);
  char* message2 = calloc(255, sizeof(char));
  sprintf(message2, "%s%s", pos, tablero);
  server_send_message(sockets_array[jugador], 9, message2);
  char* poderes_jugador1 = malloc(35);
  strcpy(poderes_jugador1, string_powers(players_info -> poderes_c1 ));
  char* poderes_jugador2 = malloc(35);
  strcpy(poderes_jugador2, string_powers(players_info -> poderes_c2 ));
  // Enviamos el estado del jugado, aqui comienza su turno
  char* data = malloc(255);
  sprintf(data, "%s:\nMéritos: %i\nEstrellas Ruz: %i\nPoderes: %s \n\n%s:\nMéritos: %i\nEstrellas Ruz: %i\nPoderes:%s \n\n", players_info->name_c1, players_info -> meritos_c1, players_info -> estrellas_c1, poderes_jugador1, players_info->name_c2, players_info-> meritos_c2, players_info -> estrellas_c2, poderes_jugador2);
  server_send_message(sockets_array[jugador], 10, data);

  free(pos);
  free(message2);
  free(poderes_jugador1);
  free(poderes_jugador2);
  free(data);
}

int poder_dados_dobles(PlayersInfo* players_info, int jugador, char * tablero)
{
  srand(time(NULL));
  int num = (rand() % 10 + 1) + (rand() % 10 + 1);
  int pos_array[2] = {players_info->posicion_c1, players_info->posicion_c2};
  int merits_array[2] = {players_info->meritos_c1, players_info->meritos_c2};
  printf("El jugador %d avanza %d con dados dobles\n", jugador, num);

  if (num + pos_array[jugador - 1] > players_info->largo_tablero)
  {
    printf("El jugador %d da la vuelta al tablero\n", jugador);
    merits_array[jugador - 1] += 20;
    return ((num + pos_array[jugador - 1]) % players_info->largo_tablero);
  }
  else{
    return (pos_array[jugador - 1] + num);
  }
}

char* se_acabo(char* razon, PlayersInfo* players_info)
{
  char* mensaje_final = calloc(255, sizeof(char));
  if(strcmp(razon, "rindio") == 0)
  {
    char* fin = "FIN DEL JUEGO!!!";
    if(players_info->perdedor == 1)
    {
      char* fin2 = "El jugador 1: ";
      char* fin3 = " se ha rendido.";
      char* fin4 = "Ha ganado el jugador 2: ";
      char* fin5 = "!!!!!";
      sprintf(mensaje_final, "\n\n%s\n\n%s%s%s\n\n%s%s%s\n", fin, fin2, players_info->name_c1, fin3, fin4, players_info->name_c2, fin5);
    }
    else{
      char* fin2 = "El jugador 2: ";
      char* fin3 = " se ha rendido.";
      char* fin4 = "Ha ganado el jugador 1: ";
      char* fin5 = "!!!!!";
      sprintf(mensaje_final, "\n\n%s\n\n%s%s%s\n\n%s%s%s\n", fin, fin2, players_info->name_c2, fin3, fin4, players_info->name_c1, fin5);
    }
  } else if(strcmp(razon, "gano") == 0)
  {
    char* fin = "FIN DEL JUEGO!!!";
    if(players_info->perdedor == 1)
    {
      char* fin2 = "El jugador 2: ";
      char* fin3 = " a conseguido las 3 estrellas Ruz";
      char* fin4 = "Ha ganado el jugador 2: ";
      char* fin5 = "!!!!!";
      sprintf(mensaje_final, "\n\n%s\n\n%s%s%s\n\n%s%s%s\n", fin, fin2, players_info->name_c2, fin3, fin4, players_info->name_c2, fin5);
    }
    else{
      char* fin2 = "El jugador 1: ";
      char* fin3 = " a conseguido las 3 estrellas Ruz";
      char* fin4 = "Ha ganado el jugador 1: ";
      char* fin5 = "!!!!!";
      sprintf(mensaje_final, "\n\n%s\n\n%s%s%s\n\n%s%s%s\n", fin, fin2, players_info->name_c1, fin3, fin4, players_info->name_c1, fin5);
    }
  }
  return mensaje_final;
}

char* string_powers_menu( int* poderes_actuales){
  char * string = malloc(65);
  strcpy(string, "");

  char poderes[3][20];
  int count = 0;

  for(int i = 0; i < 3; i++){
    if(poderes_actuales[i] == 0){
      strcpy(poderes[i], "Ninguno");

    }else if(poderes_actuales[i] == 1){
      strcpy(poderes[i], "(1) Lanzar Doble");

    }
    else if(poderes_actuales[i] == 2){
      strcpy(poderes[i], "(2) Robar méritos");

    }
    else if(poderes_actuales[i] == 3){
      strcpy(poderes[i], "(3) Retar a Duelo");
    }
  }
  /* Append all the strings */

  for (int i = 2; i >= 0; i--) {
    if (strcmp(poderes[i], "Ninguno") != 0){
      strcat(string, poderes[i]);
    }
    else{
      count ++;
    }
    if (i >= 0 ){
      strcat(string, "\n");
    }
    if(count == 3){
      strcpy(string, "No tiene poderes");
    }

  }

  return string;

}

int discount_merits(PlayersInfo *players_info, int player, int quantity){
  int merits_array[2] = {players_info->meritos_c1, players_info->meritos_c2};
  if (merits_array[player - 1] >= quantity){
    return (merits_array[player - 1] - quantity);
  }
  return 0;

}

void ganar_poder(PlayersInfo *players_info, int jugador, int en_tienda) {
  // revisar si tiene menos de 3
  // if especifico activado, dar ese poder. Si no, dar aleatorio
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  int* powers_array[2] = {players_info->poderes_c1, players_info->poderes_c2};
  int merits_array[2] = {players_info->meritos_c1, players_info->meritos_c2};
  int* poderes_actuales = powers_array[jugador];

  int count_current_powers = 0;

  for(int i = 0; i < 3; i++){
    if(poderes_actuales[i] != 0){
      count_current_powers ++;
    }
  }

  if (en_tienda == 1)
  {
    char* tienda = malloc(255);
    char* tienda_msg = "------------------------------------------------------------------\n------------------- TIENDA -------------------\n-------------------------------------------------------------------\n";
    strcpy(tienda, tienda_msg);
    server_send_message(sockets_array[jugador], 13, tienda);

    free(tienda);

    if (count_current_powers == 3)
    {
      char* limite = malloc(255);
      char* limite_msg = "------------------------------------------------------------------\n------------------- Ya tienes 3 poderes, no puedes comprar mas ----------\n-------------------------------------------------------------------\n";
      strcpy(limite, limite_msg);
      server_send_message(sockets_array[jugador], 13, limite);

      free(limite);
    }
    else{
      if (merits_array[jugador] < 10)
        {
          char* limite = malloc(255);
          char* limite_msg = "------------------------------------------------------------------\n------------------- No tienes meritos suficientes  -------------------\n-------------------------------------------------------------------\n";
          strcpy(limite, limite_msg);
          server_send_message(sockets_array[jugador], 13, limite);

          free(limite);
        }
      else{
        // Hacer una llamada al usuario para que de su input con msg 15

        char* vaci_o = malloc(255);
        char* vaci_o_msg = "------------------------------------------------------------------\n------------------- No tienes meritos suficientes  -------------------\n-------------------------------------------------------------------\n";
        strcpy(vaci_o, vaci_o_msg);
        server_send_message(sockets_array[jugador], 15, vaci_o);
        char *respuesta_tienda = NULL;
        respuesta_tienda = server_receive_payload(sockets_array[jugador]);
        free(vaci_o);
        char resp = respuesta_tienda[1];

        printf("COMPRARON : %c\n", resp);
        if (!jugador)
        {
          players_info -> meritos_c1 -=  10;
        }
        else{
          players_info -> meritos_c2 -=  10;
        }

        for(int i = 0; i < 3; i++){
          if(poderes_actuales[i] == 0){
            if (resp == '1'){
              poderes_actuales[i] = 1;
            }
            if (resp == '2'){
              poderes_actuales[i] = 2;
            }
            if (resp == '3'){
              poderes_actuales[i] = 3;
            }
            break;
          }
        }
      }
    }
  }
  else{
    if (count_current_powers == 3){
      char* limite = malloc(255);
      char* limite_msg = "------------------------------------------------------------------\n------------------- Ya tienes 3 poderes, no puedes ganar otro  ----------\n-------------------------------------------------------------------\n";
      strcpy(limite, limite_msg);
      server_send_message(sockets_array[jugador], 13, limite);

      free(limite);
      }

      else{
        char* ganar_poder_msj = malloc(255);
        char* ganar_poder_msj_ = "------------------------------------------------------------------\n------------------- Ganas un poder aleatorio ! -------------------\n-------------------------------------------------------------------\n";
        strcpy(ganar_poder_msj, ganar_poder_msj_);
        server_send_message(sockets_array[jugador], 13, ganar_poder_msj);

        free(ganar_poder_msj);

        int random_ = (rand() % 3) + 1;
        for(int i = 0; i < 3; i++){
        if(poderes_actuales[i] == 0){
          poderes_actuales[i] = random_;
          break;
        }
      }
    }
  }
}

void duelo(PlayersInfo *players_info, int jugador_iniciador, int modalidad) {
  // terminando el turno en un casilla D, (2)Poder Duelo y (3) misma casilla que su contrincante
  // server se encarga de enviar un prompt a cada usuario, preguntando (piedra,tijera,papel)
  int oponente;
  if (jugador_iniciador == 0)
  {
    oponente = 1;
  }
  else{
    oponente = 0;
  }

  printf("DUELO\n");
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};

  char *client_response_1 = NULL;
  char *client_response_2 = NULL;

  char* d_case = malloc(255);
  char* d_requ;

  if (modalidad == 0){
    d_requ = " ----- Has usado el poder DUELO ----- \n ----- DUELO DUELO DUELO-----";
  }
  else if (modalidad == 1){
    d_requ = " ----- Has caido en la misma casilla que tu oponente ! -----";
  }
  else{
    d_requ = " ----- Has caido en la casilla D ----- \n ----- DUELO DUELO DUELO-----";
  }

  sprintf(d_case, "%s", d_requ);
  server_send_message(sockets_array[jugador_iniciador], 13, d_case);


  char* request = malloc(255);
  char* requ = " ----- Ha comenzado un duelo ! Debes elegir tu estrategia -----";
  sprintf(request, "\n\n%s\n", requ);
  server_send_message(sockets_array[jugador_iniciador], 12, request);

  while(!client_response_1){
    client_response_1 = server_receive_payload(sockets_array[jugador_iniciador]);
    if (client_response_1[1] != '1' && client_response_1[1] != '2' && client_response_1[1] != '3')
    {
      client_response_1 = NULL;
    }
  }

  if (client_response_1)
  {
    server_send_message(sockets_array[oponente], 12, request);
    while(!client_response_2){
      client_response_2 = server_receive_payload(sockets_array[oponente]);
      // printf("CLIENTE: %c\n", client_response_2[1]);
      if (client_response_2[1] != '1' && client_response_2[1] != '2' && client_response_2[1] != '3')
      {
        client_response_2 = NULL;
      }
    }
  }
  char resp_1 = client_response_1[1];
  char resp_2 = client_response_2[1];

  int ganador = 3;

  // chequeos cachipun 1. PIEDRA  2. PAPEL 3. TIJERA ?
  if (resp_1 == resp_2){
    ganador = 0;
  }
  if ((resp_1 == '1' && resp_2 == '3') || (resp_1 == '3' && resp_2 == '2') || (resp_1 == '2' && resp_2 == '1'))
  {
    ganador = 2;
  }
  else if ((resp_1 == '3' && resp_2 == '1') || (resp_1 == '2' && resp_2 == '3') || (resp_1 == '1' && resp_2 == '2'))
  {
    ganador = 1;
  }

  resp_1 = '-';
  resp_2 = '-';

  // Enviamos resultados
  char* winner = " ----- Has ganado el duelo :) Meritos +10 -----";
  char* loser = " ----- Has perdido el duelo :/ Meritos -10 -----";
  char* tie = " ----- Has empatado el duelo :} Meritos +5 -----";
  char* result_1 = malloc(255);
  char* result_2 = malloc(255);

  if (ganador == 0)
  {
    printf("EMPATE\n");
    sprintf(result_1, "\n%s\n", tie);
    players_info -> meritos_c1 += 5;
    players_info -> meritos_c2 += 5;
    server_send_message(sockets_array[0], 13, result_1);
    server_send_message(sockets_array[1], 13, result_1);
  }

  if (ganador == 1)
  {
    printf("GANO JUGADOR 1\n");
    sprintf(result_1, "\n%s\n", winner);
    sprintf(result_2, "\n%s\n", loser);
    players_info->meritos_c1 += 10;
    players_info->meritos_c2 = discount_merits(players_info, 2, 10);
    server_send_message(sockets_array[0], 13, result_1);
    server_send_message(sockets_array[1], 13, result_2);
  }

  if(ganador == 2){
    printf("GANO JUGADOR 2\n");
    sprintf(result_1, "\n%s\n", loser);
    sprintf(result_2, "\n%s\n", winner);
    players_info->meritos_c2 += 10;
    players_info->meritos_c1 = discount_merits(players_info, 1, 10);

    server_send_message(sockets_array[1], 13, result_2);
    server_send_message(sockets_array[0], 13, result_1);
  }
  free(request);
  free(d_case);
  free(result_1);
  free(result_2);
}

void checkear_casilla(PlayersInfo *players_info, char* tablero, int jugador) {

  int posicion;
  int posicion_contrincante;
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  int pos_array[2] = {players_info->posicion_c1, players_info->posicion_c2};


  if (!jugador) {
    posicion = players_info -> posicion_c1;
    posicion_contrincante = players_info -> posicion_c2;
  } else {
    posicion = players_info -> posicion_c2;
    posicion_contrincante = players_info -> posicion_c1;
  }

  char casilla_actual = tablero[posicion - 1];

  int duelo_;
  if (posicion == posicion_contrincante)
  {
    duelo(players_info, jugador,  1);
    if (!jugador)
    {
      char* emptymsg = malloc(255);
      char* empty__ = "";
      strcpy(emptymsg, empty__);
      server_send_message(sockets_array[1], 3, emptymsg);

      free(emptymsg);
    }
    else{
      char* emptymsg = malloc(255);
      char* empty__ = "";
      strcpy(emptymsg, empty__);
      server_send_message(sockets_array[0], 3, emptymsg);

      free(emptymsg);
    }
  }

  // Revisamos que tipo de casilla es y aplicamos las reglas definidas

  if ('O' == casilla_actual) {

    printf("// Casilla Meritos: Jugador gana 5 meritos\n");
    char* meritos = malloc(255);
    char* meritos_msg = "------------------------------------------------------------------\n-------- Has caido en la Casilla Meritos ! Ganas 5 meritos ------\n------------------------------------------------------------------\n";
    strcpy(meritos, meritos_msg);
    server_send_message(sockets_array[jugador], 13, meritos);

    free(meritos);

    if (!jugador) {
      players_info -> meritos_c1 += 5;
    } else {
      players_info -> meritos_c2 += 5;
    }
  }
  // else if (strcmp("X", significado) == 0) {
  else if ('X' == casilla_actual) {
    // Casilla Distraccion: Jugador pierde 5 meritos (No puede ser < 0)

    char* distraccion = malloc(255);
    char* distraccion_ = "------------------------------------------------------------------\n-------- Has caido en la X ! Pierdes 5 meritos -------\n-------------------------------------------------------------------\n";
    strcpy(distraccion, distraccion_);
    server_send_message(sockets_array[jugador], 13, distraccion);

    free(distraccion);

    int puntos;


    if (!jugador) {
      puntos = players_info -> meritos_c1;
    } else {
      puntos = players_info -> meritos_c2;
    }
    if (puntos < 5){
      if (!jugador) {
        players_info -> meritos_c1 = 0;
      }
      else {
        players_info -> meritos_c2 = 0;
      }
    }

    else{
      if (!jugador) {
        players_info -> meritos_c1 -= 5;
      }
      else {
        players_info -> meritos_c2 -= 5;
      }
    }

  }

  else if ('R' == casilla_actual) {
    // Casilla Cristian Ruz: Se transforma de forma automatica 20 meritos
    // en una Estrella Ruz, si tiene la cantidad de meritos

    char* ruz = malloc(255);
    char* ruz_msg = "------------------------------------------------------------------\n-------- Has caido en Casilla Cristian Ruz ! Compras una Estrella por 20 meritos ! -------\n-------------------------------------------------------------------\n";
    strcpy(ruz, ruz_msg);
    server_send_message(sockets_array[jugador], 13, ruz);

    free(ruz);

    int puntos;
    if (!jugador) {
      puntos = players_info -> meritos_c1;
    } else {
      puntos = players_info -> meritos_c2;
    }
    if (puntos > 19)
    {
      if (!jugador) {
        players_info -> meritos_c1 -= 20;
        players_info -> estrellas_c1 += 1;
        // check if >3 stars
      }
      else {
        players_info -> meritos_c2 -= 20;
        players_info -> estrellas_c1 += 1;
      }
    }
    else{
      char* no_merits = malloc(255);
      char* no_merits_msg = "------------------------------------------------------------------\n-------- No tienes 20 meritos -------\n-------------------------------------------------------------------\n";
      strcpy(no_merits, no_merits_msg);
      server_send_message(sockets_array[jugador], 13, no_merits);

      free(no_merits);
    }
  }

  else if ('P' == casilla_actual) {
    // Casilla Poder: Jugador gana un poder aleatorio
    // * Limite de 3 poderes
    ganar_poder(players_info, jugador, 0);
  }

  else if ('T' == casilla_actual) {
    // Casilla Tienda: Jugador puede comprar un poder a un precio de 10 Meritos
    ganar_poder(players_info, jugador, 1);

  }

  else if ('F' == casilla_actual) {
    // Casilla Super F: Jugador vuelve a la casilla de Inicio
    if (!jugador) {
      players_info -> posicion_c1 = 1;
    } else {
      players_info -> posicion_c2 = 1;
    }

    char* request = malloc(255);
    char* num_jug = "------------------------------------------------------------------\n-------- SUPER F ! Has sido enviado de vuelta al Inicio !! -------\n-------------------------------------------------------------------\n";
    sprintf(request, "\n%s\n\n", num_jug);
    server_send_message(sockets_array[jugador], 13, request);

    free(request);

  }

  else if ('A' == casilla_actual) {
    printf("// Casilla Sala Alvaro Campos: Jugador Gana 30 Meritos\n");

    char* alvaro_camp = malloc(255);
    char* alvaro_camp_msg = "------------------------------------------------------------------\n-------- Casilla Sala Alvaro Campos | Ganas 30 Meritos  ! -------\n-------------------------------------------------------------------\n";
    sprintf(alvaro_camp, "\n%s\n", alvaro_camp_msg);
    server_send_message(sockets_array[jugador], 13, alvaro_camp);

    free(alvaro_camp);

    if (!jugador) {
      players_info -> meritos_c1 += 30;
    } else {
      players_info -> meritos_c2 += 30;
    }
  }

  else if ('D' == casilla_actual) {
    // Casilla Duelo: Jugador reta a duelo al otro jugador
    duelo(players_info,  jugador, 2);
    if (!jugador)
    {
      char* emptymsg = malloc(255);
      char* empty__ = "";
      strcpy(emptymsg, empty__);
      server_send_message(sockets_array[1], 3, emptymsg);

      free(emptymsg);
    }
    else{
      char* emptymsg = malloc(255);
      char* empty__ = "";
      strcpy(emptymsg, empty__);
      server_send_message(sockets_array[0], 3, emptymsg);

      free(emptymsg);
    }
  }

  else if ('Y' == casilla_actual) {
  // Casilla Fonda Don Yadran: Jugador pierde proximo turno

    char* don_yadran = malloc(255);
    char* don_yadran_msg = "------------------------------------------------------------------\n ------- Pierde el proximo turno por ir a la Fonda Don Yadran  -------\n-------------------------------------------------------------------\n";
    sprintf(don_yadran, "\n%s\n", don_yadran_msg);
    server_send_message(sockets_array[jugador], 13, don_yadran);

    free(don_yadran);

    if (!jugador) {
      players_info -> pierde_turno_c1 = 1;
    } else {
      players_info -> pierde_turno_c2 = 1;
    }
  }
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

    // Se crea el servidor y se obtienen los sockets de ambos clientes.
    PlayersInfo *players_info = prepare_sockets_and_get_clients(IP, PORT);

    // Le enviamos al primer cliente un mensaje de bienvenida
    char *welcome = "";

    server_send_message(players_info->socket_c1, 0, welcome);
    server_send_message(players_info->socket_c2, 0, welcome);

    // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
    int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
    int my_attention = 0;
    int initilizer = 0;
    char* tablero = calloc(255, sizeof(char));

    // Seteamos los parámetros de los jugadores

    // Jugador 1
    players_info -> meritos_c1 = 0;
    players_info -> estrellas_c1 = 0;

    players_info -> posicion_c1 = 1;
    players_info -> poderes_act_c1 = 0;

    players_info -> poderes_c1[0] = 0;
    players_info -> poderes_c1[1] = 0;
    players_info -> poderes_c1[2] = 0;

    players_info -> pierde_turno_c1 = 0;

    // Jugador 2
    players_info -> meritos_c2 = 0;
    players_info -> estrellas_c2 = 0;

    players_info ->posicion_c2 = 1;
    players_info -> poderes_act_c2 = 0;
    players_info -> pierde_turno_c2 = 0;

    players_info -> poderes_c2[0] = 0;
    players_info -> poderes_c2[1] = 0;
    players_info -> poderes_c2[2] = 0;

    while (1)
    {
        // Se obtiene el paquete del cliente
        int msg_code = server_receive_id(sockets_array[my_attention]);
        printf("msg code inicial= %d\n", msg_code);

        if(msg_code == 0 && FIN == 0) //mensajes de bienvenida
        {
          char *client_message = server_receive_payload(sockets_array[my_attention]);
          int len = strlen(client_message) + 1;
          if(my_attention == 0)
          {
            players_info->name_c1 = malloc(len);
            strcpy(players_info->name_c1, client_message);
          }

          if(my_attention == 1)
          {
            players_info->name_c2 = malloc(len);
            strcpy(players_info->name_c2, client_message);
          }

          if(my_attention == 0)
          { char* hello = welcome_msg(players_info->name_c1);
            //enviamos el request del tablero al jugador 1
            char* request = malloc(255);
            char* requ = "Por favor ingrese la ruta al tablero a utilizar.";
            char* num_jug = ", eres el jugador 1!";
            sprintf(request, "\n%s%s\n\n%s\n", hello, num_jug, requ);
            server_send_message(sockets_array[my_attention], 2, request);

            free(hello);
            free(request);
          }
          else{
            char* hello = welcome_msg(players_info->name_c2);
            char* request = malloc(255);
            char* num_jug = ", eres el jugador 2!";
            char* turno = "Espera que es el turno del otro jugador...";
            sprintf(request, "\n%s%s\n\n%s\n\n", hello, num_jug, turno);
            server_send_message(sockets_array[my_attention], 1, request);

            free(hello);
            free(request);
          }
          my_attention = (my_attention + 1) % 2;
        }

        else if(msg_code == 1) //descartar mensajes vacios cuando imprimo tablero actualzado
        {
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);
        }

        else if(msg_code == 3)
        { // Enviamos el tablero
          char *message = server_receive_payload(sockets_array[my_attention]);
          if(initilizer == 0){
            memcpy(tablero, message, strlen(message));
            players_info -> largo_tablero = strlen(tablero);
            printf("Largo tablero -> %d\n", players_info -> largo_tablero);
            initilizer++;
          }
          else{
            free(message);
          }
          if(my_attention == 0){
            if(players_info->pierde_turno_c1 == 1)
            {
              char* empty = malloc(255);
              char* requ = "\n\nPERDISTE TU TURNO POR CAER EN LA CASILLA Y\n\n";
              strcpy(empty, requ);
              players_info->pierde_turno_c1 = 0;
              enviar_tablero_actual(players_info, tablero, my_attention);
              server_send_message(sockets_array[my_attention], 11, empty);
              my_attention = (my_attention + 1) % 2;

              free(empty);
            }
            else
            {
              char* pos = malloc(4*sizeof(char) + 5);
              sprintf(pos, "%d %d ", players_info->posicion_c1, players_info->posicion_c2);
              char* message2 = calloc(255, sizeof(char));
              sprintf(message2, "%s%s", pos, tablero);
              server_send_message(sockets_array[my_attention], 4, message2);

              free(pos);
            }
          }
          else{
            if(players_info->pierde_turno_c2 == 1)
            {
              char* empty = malloc(255);
              char* requ = "\n\nPERDISTE TU TURNO POR CAER EN LA CASILLA Y\n\n";
              strcpy(empty, requ);
              players_info->pierde_turno_c2 = 0;
              enviar_tablero_actual(players_info, tablero, my_attention);
              server_send_message(sockets_array[my_attention], 11, empty);
              my_attention = (my_attention + 1) % 2;

              free(empty);
            }
            else
            {
              char* pos = malloc(4*sizeof(char) + 5);
              sprintf(pos, "%d %d ", players_info->posicion_c1, players_info->posicion_c2);
              char* message2 = calloc(255, sizeof(char));
              sprintf(message2, "%s%s", pos, tablero);
              server_send_message(sockets_array[my_attention], 4, message2);
              free(pos);
            }
          }
        }

        else if(msg_code == 4)
        {
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);

          char* poderes_jugador1 = malloc(35);
          strcpy(poderes_jugador1, string_powers(players_info -> poderes_c1 ));

          char* poderes_jugador2 = malloc(35);
          strcpy(poderes_jugador2, string_powers(players_info -> poderes_c2 ));

          // Enviamos el estado del jugado, aqui comienza su turno
          char* data = malloc(255);

          //sprintf(data, "%s Méritos: %i      %s Méritos: %i\n  Estrellas Ruz: %i        Estrellas Ruz: %i\n  Poderes: %s              Poderes: %s",players_info->name_c1,players_info -> meritos_c1, players_info->name_c2, players_info -> meritos_c2, players_info -> estrellas_c1,  players_info -> estrellas_c2,poderes_jugador1, poderes_jugador2);

          sprintf(data, "%s:\nMéritos: %i\nEstrellas Ruz: %i\nPoderes: %s \n\n\n%s:\nMéritos: %i\nEstrellas Ruz: %i\nPoderes:%s \n\n", players_info->name_c1, players_info -> meritos_c1, players_info -> estrellas_c1, poderes_jugador1, players_info->name_c2, players_info-> meritos_c2, players_info -> estrellas_c2, poderes_jugador2);

          //sprintf(data, "Stats\n--------%s---------  |---------%s---------\nMéritos: %i              |  Méritos: %i\nEstrellas Ruz: %i        |  Estrellas Ruz: %i\nPoderes:                |  Poderes: [Falta mostrar poderes!]\n", players_info->name_c1, players_info->name_c2, players_info -> meritos_c1, players_info-> meritos_c2, players_info -> estrellas_c1, players_info -> estrellas_c2);

          server_send_message(sockets_array[my_attention], 5, data);

          free(data);
          free(poderes_jugador1);
          free(poderes_jugador2);
        }

        else if(msg_code == 5)
        { char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);

          char * poderes_j = malloc(66);

          if(my_attention == 0){
            strcpy(poderes_j, string_powers_menu(players_info -> poderes_c1));
          }

          else{
            strcpy(poderes_j, string_powers_menu(players_info -> poderes_c2 ));

          }

          char* opciones = malloc(255);

          sprintf(opciones, "MENÚ \n\nElige una de las siguientes opciones\n\n---   Utilizar un poder   ---\n%s \n\n---   Rendirse   ---\n(4) Me rindo!\n\n---   Avanzar   ---\n(5) Aleatorio |(6) Manualmente\n", poderes_j);
          server_send_message(sockets_array[my_attention], 6, opciones);

          free(opciones);
          free(poderes_j);
        }

        else if(msg_code == 6)
        {
          // Eligió el poder de dado doble
          char *message = server_receive_payload(sockets_array[my_attention]);
          free(message);
          if(my_attention == 0)
          {
            players_info->posicion_c1 = poder_dados_dobles(players_info, 1, tablero);
            checkear_casilla(players_info, tablero, my_attention);
            for(int i = 0; i<3;i++){
              if(players_info -> poderes_c1[i] == 1){
                players_info -> poderes_c1[i] = 0;
                break;
              }
            }
            players_info -> poderes_act_c1 -= 1;

          }
          if(my_attention == 1)
          {
            players_info->posicion_c2 = poder_dados_dobles(players_info, 2, tablero);
            checkear_casilla(players_info, tablero, my_attention);
            for(int i = 0; i<3;i++){
              if(players_info -> poderes_c2[i] == 1){
                players_info -> poderes_c2[i] = 0;
                break;
              }
            }
            players_info -> poderes_act_c2 -= 1;

          }
          enviar_tablero_actual(players_info, tablero, my_attention);
          char* empty = malloc(255);
          char* requ = "";
          strcpy(empty, requ);
          server_send_message(sockets_array[my_attention], 3, empty);
          my_attention = (my_attention + 1) % 2;

          free(empty);
        }

        else if(msg_code == 7)
        {
          // Eligió el poder de robar méritos

          // Al activarlo el jugador le roba méritos al otro jugador. La cantidad de méritos robada es un
          // número aleatorio entre 1 y 10 incluidos. La cantidad de m´eritos de cada jugador nunca puede ser menor que 0.

          srand(time(NULL));
          int robar = rand()%10 + 1;
          printf("El jugador %d le trata de robar %d meritos al jugador %d\n", my_attention + 1, robar, (1 - my_attention) + 1);

          if(my_attention == 0)
          {
            // El cliente 1 le roba a 2
            if(players_info->meritos_c2 < robar){
              players_info->meritos_c2 = 0;
              players_info->meritos_c1 += players_info->meritos_c2;
            }
            else{
              players_info->meritos_c2 -= robar;
              players_info->meritos_c1 += robar ;
            }

            players_info -> poderes_act_c1 -= 1;

            for(int i = 0; i<3;i++){
              if(players_info -> poderes_c1[i] == 2){
                players_info -> poderes_c1[i] = 0;
                break;
              }
            }

          }
          else if(my_attention == 1)
          {
            // El cliente 2 le roba a 1

            if(players_info->meritos_c1 < robar){
              players_info->meritos_c2 += players_info->meritos_c1;
              players_info->meritos_c1 = 0;
            }
            else{
              players_info->meritos_c1 -= robar;
              players_info->meritos_c2 += robar ;
            }

            players_info -> poderes_act_c2 -= 1;

            for(int i = 0; i<3;i++){
              if(players_info -> poderes_c2[i] == 2){
                players_info -> poderes_c2[i] = 0;
                break;
              }
            }

          }

          // Cambio de turno
          enviar_tablero_actual(players_info, tablero, my_attention);
          char* empty = malloc(255);
          char* requ = "";
          strcpy(empty, requ);
          server_send_message(sockets_array[my_attention], 3, empty);
          my_attention = (my_attention + 1) % 2;

          free(empty);
        }
        else if (msg_code == 8){
          duelo(players_info,  my_attention, 0);
          enviar_tablero_actual(players_info, tablero, my_attention);
          if (my_attention == 0)
          {
            char* emptymsg = malloc(255);
            char* empty__ = "";
            strcpy(emptymsg, empty__);
            server_send_message(sockets_array[1], 3, emptymsg);

            free(emptymsg);
          }
          else{
            char* emptymsg = malloc(255);
            char* empty__ = "";
            strcpy(emptymsg, empty__);
            server_send_message(sockets_array[0], 3, emptymsg);

            free(emptymsg);
          }
          my_attention = (my_attention + 1) % 2;
        }
        else if(msg_code == 9)
        { // El cliente eligió Rendirse

          char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);
          int winner;
          int loser;
          if(my_attention == 0)
          {
            players_info -> perdedor = 1;
            winner = 0;
            loser = 1;
          }
          else
          {
            players_info -> perdedor = 2;
            winner = 1;
            loser = 0;
          }

          char* mensaje_fin = se_acabo("rindio", players_info);
          FIN = 1;
          send_img(sockets_array[winner], sockets_array[loser]);
          server_send_message(sockets_array[my_attention], 7, mensaje_fin);
          my_attention = (my_attention + 1) % 2;
          server_send_message(sockets_array[my_attention], 7, mensaje_fin);

          free(mensaje_fin);
        }

        else if(msg_code == 10)
        {
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);
          // avanzar aleatorio

          srand(time(NULL));
          int random_number = (rand() % 10) + 1;
          printf("El jugador %d avanza %d de forma aleatoria\n", my_attention + 1, random_number);

          // Actualizamos la posicion del jugador respectivo en el tablero
          if (!my_attention) { //Jugador 1
            players_info -> posicion_c1 += random_number;

          } else {             //Jugador 2
            players_info -> posicion_c2 += random_number;
          }

          // Revisamos si superan el final del tablero y los colocamos al inicio
          // Agregar que pasa si dan una vuelta al tablero
          if (players_info -> posicion_c1 > strlen(tablero)){
            players_info -> posicion_c1 = players_info -> posicion_c1 % strlen(tablero);
            players_info -> meritos_c1 += 20;
            printf("El jugador %d da la vuelta al tablero y gana 20 Meritos\n", my_attention + 1);
          }

          if (players_info -> posicion_c2 > strlen(tablero)){
            players_info -> posicion_c2 = players_info -> posicion_c2 % strlen(tablero);
            players_info -> meritos_c2 += 20;
            printf("El jugador %d da la vuelta al tablero y gana 20 Meritos\n", my_attention + 1);
          }

          // Revisamos la casilla en la que esta el jugador
          checkear_casilla(players_info, tablero, my_attention);

          // Cambio de turno
          enviar_tablero_actual(players_info, tablero, my_attention);
          char* empty = malloc(255);
          char* requ = "";
          strcpy(empty, requ);
          server_send_message(sockets_array[my_attention], 3, empty);
          my_attention = (my_attention + 1) % 2;

          free(empty);
        }

        else if(msg_code == 11)
        {
          printf("MANUAL\n");
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          printf("mensaje que viene: %s\n", client_message);
          free(client_message);
          //checkear_casilla(players_info, tablero, my_attention);
          // avanzar manual --> pregunto cuanto
          char* opciones = malloc(255);
          char* requ = "Elige un numero del 1 a 10 representando el numero de casillas que quieres avanzar:\n";
          strcpy(opciones, requ);
          server_send_message(sockets_array[my_attention], 8, opciones);

          free(opciones);
        }

        else if(msg_code == 12)
        {
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          int num = atoi(client_message);
          printf("input recibido : %d\n", num);
          free(client_message);
          //chequeo si me gano los meritos
          if(my_attention == 0)
          {
            if(num + players_info->posicion_c1 > players_info->largo_tablero)
            {
              players_info->meritos_c1 += 20;
              players_info->posicion_c1 = (num + players_info->posicion_c1) % players_info->largo_tablero;
            }
            else
            {
              players_info->posicion_c1 = (num + players_info->posicion_c1);
            }
          }
          else
          {
            if(num + players_info->posicion_c2 > players_info->largo_tablero)
            {
              players_info->meritos_c2 += 20;
              players_info->posicion_c2 = (num + players_info->posicion_c2) % players_info->largo_tablero;
            }
            else
            {
              players_info->posicion_c2 = (num + players_info->posicion_c2);
            }
          }
          //llamo a la funcionq que chequea las casillas con la casilla que me toco
          checkear_casilla(players_info, tablero, my_attention);

          // Cambio de turno
          enviar_tablero_actual(players_info, tablero, my_attention);
          char* empty = malloc(255);
          char* requ = "";
          strcpy(empty, requ);
          server_send_message(sockets_array[my_attention], 3, empty);
          my_attention = (my_attention + 1) % 2;

          free(empty);
        }

        else if (msg_code == 13)
        {
          //recibo respuesta del duelo
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);
          printf("\n DUELO !\n");
        }
        else if (msg_code == 14)
        {
          //recibo respuesta de la tienda
          char *client_message= server_receive_payload(sockets_array[my_attention]);
          free(client_message);
          printf("\n Tienda !\n");
        }

        // Revisamos si alguno de los jugadores gano
        if (players_info -> estrellas_c1 > 2) {
          // Gana el jugador 1
          players_info -> perdedor = 2;
          FIN = 1;

          // Enviamos el mensaje a ambos jugadores
          char* mensaje_fin = se_acabo("gano", players_info);

          server_send_message(sockets_array[my_attention], 7, mensaje_fin);
          my_attention = (my_attention + 1) % 2;
          server_send_message(sockets_array[my_attention], 7, mensaje_fin);
          send_img(sockets_array[0], sockets_array[1]);

          free(mensaje_fin);

        } else if (players_info -> estrellas_c2 > 2) {
          // Gana el jugador 2
          players_info -> perdedor = 1;
          FIN = 1;

          // Enviamos el mensaje a ambos jugadores
          char* mensaje_fin = se_acabo("gano", players_info);

          server_send_message(sockets_array[my_attention], 7, mensaje_fin);
          my_attention = (my_attention + 1) % 2;
          server_send_message(sockets_array[my_attention], 7, mensaje_fin);
          send_img(sockets_array[1], sockets_array[0]);

          free(mensaje_fin);
        }

        if(FIN == 1)
        {
          close(sockets_array[0]);
          close(sockets_array[1]);
          free(tablero);
          free(players_info->name_c1);
          free(players_info->name_c2);
          return 0;
        }
        printf("msg code final= %d\n", msg_code);
    }
    close(sockets_array[0]);
    close(sockets_array[1]);
    free(tablero);
    return 0;
}
