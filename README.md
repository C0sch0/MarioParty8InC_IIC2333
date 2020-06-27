# Server Network to Client Player

Main server to clients socket communication.
Send image by socket in c.






### Integrantes

Antonia Christensen - 15636682

Diego Iruretagoyena - 14619164

Tamara Lues - 15638936

Claudio Prieto - 1664039J

### Instrucciones

Make: carpeta server tanto como de la carpeta client. 
```
./server -i <ip_address> -p <tcp_port>
```
Donde se debe **mantener el orden (IP antes del puerto)** y se deben **escribir las flags (-i y -p respectivamente)**. La **IP address** debe estar escrita de la forma **XX.XX.X.XX** (*human-readable*) y el **puerto** como un **número entero** de un puerto válido.
Luego se deben correr los dos clientes de la forma:
```
./client -i <ip_address> -p <tcp_port>
```
Se debe respetar las mismas consideraciones que en el caso del servidor, y es importante que **las IP address y los puertos entre clientes y el servidor coincidan**.

### Protocolo de comunicación Server-Client

#### Mensajes enviados por el Servidor

**ID : 0**
* El servidor envía un mensaje vacío al cliente para iniciar la comunicación (lo manda antes de entrar al while true de escuchar al socket del cliente).

**ID: 1**
* El servidor envía un mensaje de bienvenida al cliente 2, que incluye su nombre y le dice que es eljugador número 2 y que debe esperar hasta que sea su turno (cliente 1 está jugando).

**ID: 2**
* El servidor envía un mensaje de bienvenida al cliente 1, que incluye su nombre y le dice que es el jugador número 1 y que debe ingresar la ruta al archivo de tablero para usar en la partida.

**ID : 3**
* El servidor envía un mensaje vacío al cliente para generar el cambio de turno.

**ID: 4**
* El servidor envía el string que representa el tablero concatenandolo a un string de la forma "X Y ", donde X representa la posicion del jugador 1 e Y la posicion del jugador 2 para que el cliente pueda imprimir la "barra de progresión" de los jugadores sobre el tablero indicando en que casilla se encuentra cada uno.

**ID: 5**
* El servidor envía un string que incluye la información de ambos jugadores (nombre, número de méritos, número de estrellas Ruz y nombre de los poderes que posee cada uno) estilizado de tal forma que el cliente al recibirlo pueda imprimirlo directamente en pantalla.

**ID: 6**
* El servidor envía un string que contiene el menú de opciones que el jugador puede optar para jugar su turno (usar poderes, avanzar manual, avanzar automático o rendirse), pero personificado para cada jugador en particular (se le muestran las opciones de usar los poderes que el posee y si no posee no le aparece la opcion de usar poderes).

**ID : 7**
* El servidor envía un mensaje de fin del juego al cliente, indicando quien fue el jugador ganador y quien el perdedor y la razón de término del juego.

**ID : 8**
* El servidor envía un mensaje al cliente para pedirle el número de casillas (entre 1 y 10) que quiere avanzar de forma manual.

**ID : 9**
* El servidor envía el string que representa el tablero concatenandolo a un string de la forma "X Y ", donde X representa la posicion del jugador 1 e Y la posicion del jugador 2 para que el cliente pueda imprimir el tablero actualizado luego de haber realizado su jugada.

**ID: 10**
* El servidor envía un string que incluye la información actualizada de ambos jugadores (nombre, número de méritos, número de estrellas Ruz y nombre de los poderes que posee cada uno) al cliente para que vea los resultados luego de haber realizado su turno.

**ID: 11**
* El servidor envía un mensaje que al cliente diciendo que como en el turno anterior cayó en la casilla "Y", ahora pierde su turno y le cede el turno al otro cliente para que juegue.

**ID: 12**
* El servidor envía un mensaje que al cliente diciendo que se ha iniciado un duelo y le indica que debe elegir su estrategia para jugar.

**ID: 13**
* El servidor envía información de lo que esta ocurriendo en el juego en un string (alertas o notificaciones), sin esperar respuesta a cambio --> son avisos para que el juego sea más claro desde el punto de vista del cliente. (Ej: le avisa en que tipo de casilla quedó, si gana o pierde méritos, etc).

**ID: 15**
* El servidor envía un mensaje que al cliente que se encuentra en la Tienda notificando que cumple las condiciones para comprar un poder (tiene menos de 3 y al menos 20 méritos) para que se le oregunte que poder desea comprar.

**ID: 64**
* El servidor envía un mensaje al cliente indicándole que le esta enviando una imagen, por lo que debe llamar a la funcion get_img (para abrir el archivo y escribir los datos que se le envian) para luego poder ver esa imagen.

#### Mensajes enviados por el Cliente

**ID: 0**
* El cliente envia su nombre al servidor.

**ID: 1**
* El cliente le envía un mensaje vacío al servidor indicando que recibió el tablero actualizado y la información actualizada de los jugadores post-jugada y que está esperando hasta que sea su turno.

**ID: 3**
* El cliente 1 envia por primera vez al servidor el string tablero luego de leerlo del archivo, luego, al igual que el cliente 2, le envía mensajes vacíos porque este mensaje es el que inicia el turno del cliente, ya que le avisa al servidor que debe enviarle al cliente el tablero con las posiciones (que es lo que marca el inicio de su turno).

**ID: 4**
* El cliente envía un mensaje vacío al servidor en respuesta de haber recibido el tablero y las posiciones, indicandole que le mande la información de los jugadores para imprimirla en pantalla.

**ID: 5**
* El cliente envía un mensaje vacío al servidor en respuesta de haber la información de los jugadores, indicandole que le mande las opciones para elegir que hacer en el turno.

**ID: 6**
* El cliente envía la respuesta al menú de opciones del turno, indicando que quiere usar su poder de dado doble.

**ID: 7**
* El cliente envía la respuesta al menú de opciones del turno, indicando que quiere usar su poder robar méritos.

**ID: 8**
* El cliente envía la respuesta al menú de opciones del turno, indicando que quiere usar su poder duelo.

**ID: 9**
* El cliente envía la respuesta al menú de opciones del turno, indicando que quiere rendirse.

**ID: 10**
* El cliente envía la respuesta al menú de opciones del turno, indicando que quiere avanzar de forma aleatoria.

**ID: 11**
* El cliente envía la respuesta al menú de opciones del turno, indicando que quiere avanzar de forma manual.

**ID: 12**
* El cliente envía la respuesta a la pregunta de las casillas, indicando cuantas casillas quiere avanzar (del 1 al 10).

**ID: 13**
* El cliente envía la respuesta a la pregunta del duelo acerca de su estrategia, indicando que estrategia eligió ((1) piedra, (2) papel o (3) tijera).

**ID: 14**
* El cliente envía la respuesta a la pregunta de la tienda, indicando que poder quiere comprar ((1) dados dobles, (2) roba méritos o (3) duelo).

#### Funcionalidades implementadas

Al iniciar la partida, se le solicita al primer participante que ingresó la dirección del tablero.
Este será el cliente que comience jugando. Una vez que ingresa correctamente el tablero, se muestra el tablero,
(en el cual se indica la posición de cada jugador diferenciado por azul o rojo) y el estado actual de cada jugador (informando
sobre los méritos, poderes y estrellas cruz).

**MENÚ**

En cada turno, al jugador recibirá la información descrita anteriormente y el menú. Este menú muestra los poderes que el usuario
tiene y el respectivo código que debe elegir para utilizarlo, al igual que cada jugada. Cada jugada le envía un mensaje al servidor y es tratada
según la funcionalidad que se este solicitando.

*Decisión de Diseño:*

- No se maneja que el input sea correcto en el menú, es decir que apunte a un id válido según la jugada. Esto se debe a que los poderes van cambiando,
  lo cual cambia también los criterios de validación, por lo tanto, se decidió suponer el uso correcto del menú.


**JUGADAS**

Respecto a las funcionalidades que se presentan en el enunciado, estas fueron implementadas en su totalidad.
Cada vez que el cliente realiza una jugada de movimiento, se llama a la función **checkear_casilla(PlayersInfo *players_info, char* tablero, int jugador)**.
Con esta función se evalúa en que casilla quedo y se obtiene la letra de la posición. Con esta letra se entra en el if correspondiente y se ejecuta una acción. Avanzar en el tablero se puede solicitar en la modalidad *manual* o *aleatoria*. En el primer caso, se establece una segunda comunicación con el usuario para
conocer la cantidad a desplazar.

Ahora bien si, en el menú esta también la opción de *retirarse*. Para esto debe elegir la opción 4, en donde el servidor se encargad de informarle a los
dos jugarores que el juego el terminó y la razón de término.

Por ultimo, una jugada puede consistir en utilizar un poder. En caso de que el poder no requiera una segunda interacción con el usuario
entonces el servidor recibe el mensaje y cambia los atributos o ejecuta el poder. En el caso de elegir el poder de duelo, este le preguntando
al retador que elija entre las tres opciones disponibles. Se le pregunta al contrincantes y se evalúa la respuesta. Sea cual sea el
resultado el turno siguiente será para el que fue "retado" a duelo.

*Decisiones de Diseño:*

-  En el caso de que los usuarios queden en la misma casilla se realiza un *duelo*

-  Cuando se elige el poder de *Robar méritos* se elige una cantidad de méritos aleatorios entre 1 y 10. Si el usuario al cual se le robarán los créditos
   tienen menos de la cantidad elegida entonces al jugador (que gasto su poder) se le adicionan todos los méritos del usuario y el otro queda en 0. Es decir,
   si el número aleatorio es 7, y el cliente solo tiene 5, se le agregan los 5 al cliente dueño del poder y el otro queda con 0 méritos.

-  Al momento de caer en la misma casilla de un que el otro jugador, si esta corresponde por ejemplo la casilla de ganar méritos, se realiza el *duelo* y después
   se desplegará la información de los méritos.

-  En el caso borde de que dos jugadores lleguen a la casilla *'Y'* al primero que llegué a la casilla se le indicará que perdió el turno (usuario 1). Pero tras
   la llegada del segundo jugador, se realizara un *duelo* y se anula la pérdida de turno, por lo que le toca a el usuario es el usuario1 (el primero que llego a
   casilla).

- Cuando el usuario cae en la casilla *'E'* se realiza un turno normal, pero, si el usuario pasa este turno, se realiza el movimiento cíclico, es decir, se
  se comienza desde el inicio del tablero y se suman los 20 méritos correspondientes.

- A diferencia del menú, para el *duelo*, si se válida que el input de la jugada sera correcto.


**BONUS**

- Se implementó el bonus de envía de imágenes al finalizar el juego, las cuales se encontrarán en la carpeta del cliente.
# MarioParty8InC_IIC2333
