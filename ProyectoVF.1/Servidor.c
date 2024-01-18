#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <mysql.h>
#include <time.h>
typedef struct { //se define estructura conectado
	
	char nombre[20];
	int socket;
	int posicion;
	
}TConectado;

typedef struct { // se define la lista de conectados
	
	TConectado conectados[100];
	int num; 
	
}TListaCon;


typedef struct { // se definen caracteristicas necesarias de cada jugador
	
	char nombre[20];
	int socket;
	int posicion;
	char color[20];
	
}TJugador;

typedef struct{ // se define estructura de la partida
	
	TJugador identidad[4];	
	int jugadores;
	int idpartida;
	
}TPartida; 

typedef struct{ // se define la lista de partidas 
	TPartida partida[100];
	int numero; 	
}TListaPartidas;

TListaPartidas Partidas;
TPartida identidades;
TListaCon lista;
int turno;
int turno2 = 0;
int turnodefinitivo;
int i;

int sockets[100];
char cadenafinal[200];
char cadenafinalpartida[200];
char cadenaposicionesfichas[200];
char cadenafinalpartidapos[200];
int contadorturno2 = 0;
int contador;
int numeroinvitados;
int socketsChat [80]; //Vector en el cual guardo sock_conn y a los invitados
int socketsChatfuturo [80];//Vector en el cual guardo sock_conn y a los invitados,pero lo utilizo en caso de que la peticion sea denegada,en ese caso muestro socketsChat

int numeroinvitados;
int numusuarioschat;

int Pon ( TListaCon *lista, char nombre [20], int socket)//Inserta conectado en la lista de conectados
{
	if(lista->num == 100) // ya hay 100 elementos
		return 0;
	else 
	{
		lista->conectados[lista->num].socket = socket;
		strcpy(lista->conectados[lista->num].nombre,nombre);
		lista->num++;
		printf("Introducido en la lista: %d\n", lista->num);
		return 1; // todo ha ido bien
		
	}
}



int PonPartida ( TListaPartidas *Partidas, char nombre [20], int socket, int posicion, int numeroPartida)//Insertar los datos del jugador en la Partida
{
	if(Partidas->numero == 100) // ya hay 100 elementos
		return 0;
	else 
	{
		
		Partidas->partida[Partidas->numero].identidad[Partidas->partida[Partidas->numero].jugadores].socket = socket;		
		Partidas->partida[Partidas->numero].identidad[Partidas->partida[Partidas->numero].jugadores].posicion = posicion;		
		strcpy(Partidas->partida[Partidas->numero].identidad[Partidas->partida[Partidas->numero].jugadores].nombre,nombre);		
		
		Partidas->partida[Partidas->numero].jugadores = Partidas->partida[Partidas->numero].jugadores +1;
		Partidas->partida[Partidas->numero].idpartida = numeroPartida;
		printf("jugadores insertados: %d\n", Partidas->partida[Partidas->numero].jugadores);
		return 1; // todo ha ido bien
	}
}

void CadenaPartida (TListaPartidas *Partidas, char cadenafinalpartida [100])//Devuelve en forma cadena Listado de jugadores y su numero separados por |
{
	
	char cadena[200];
	int cont=0;
	
	for (int i=0;i<Partidas->partida[Partidas->numero].jugadores;i++){
		
		sprintf (cadena,"%s|%s",cadena,Partidas->partida[Partidas->numero].identidad[i].nombre);		
		cont ++;		
	}
	
	sprintf(cadenafinalpartida,"%d%s",Partidas->partida[Partidas->numero].jugadores,cadena);
	printf("%s\n",cadenafinalpartida);
	
}
int Eliminar ( TListaCon *lista, char nombre [20]) //Elimina conectado de la lista de conectados a traves de su posicion y indica quien se queda su posicion
{
	
	int pos = DamePosicion(lista, nombre);
	printf("posicion a eliminar: %d y el nombre es: %s\n", pos, nombre);
	printf("Nombre del que se va: %s\n", lista->conectados[pos].nombre);
	
	if(pos == -1)
	{
		return pos;
	}
	else
	{
		for(int i = pos; i<lista->num-1; i++)
		{
			lista->conectados[i].socket = lista->conectados[i+1].socket;
			strcpy(lista->conectados[i].nombre, lista->conectados[i+1].nombre);
		}
		lista->num--;
		printf("Nombre del que se queda su posicion: %s\n", lista->conectados[pos].nombre);
		return 0;
	}
}
int DameSocket (TListaCon *lista, char nombre [20]) //busca en lista de conexiones (TListaCon) el socket asociado al conectado
{
	int encontrado = 0;
	int i = 0;
	
	printf("que buscas?? --> %s\n",nombre);
	
	while ((i < lista->num) && (encontrado!= 1))
	{
		printf("antes del if 1\n");
		if (strcmp ( lista->conectados[i].nombre,nombre) ==0)
		{
			encontrado = 1;
			printf("dentro del if, ademas socket= %d\n", lista->conectados[i].socket);
		}
		
		else
			i++;
	}
	
	if (encontrado)
	{
		printf("Has salido del bucle del socket: %d \n",lista->conectados[i].socket);
		return lista->conectados[i].socket;
		
	}
	
	
	else 
		return -1;
}

int DamePosicion (TListaCon *lista, char nombre [20]) //Devuelve la posicion en la lista de conectados
{
	int encontrado = 0;
	int i = 0, pos;
	
	printf("que buscas?? --> %s\n",nombre);
	printf("lista num: %d\n",lista->num);
	while ((i < lista->num) && (encontrado!= 1))
	{
		printf("antes del if\n");
		if (strcmp (lista->conectados[i].nombre,nombre) ==0)
		{
			encontrado = 1;
			printf("dentro del if\n");
		}
		
		else
			i++;
	}
	
	if (encontrado)
	{
		printf("Has salido del bucle con num iteracion: %d\n", i);
		pos = i;
		return pos;
		
	}
	
	
	else 
		return -1;
	
}

void Cadena (TListaCon *lista, char cadenafinal [100])//Devuelve en formato cadena los conectados separados por un |
{
	
	char cadena[200];
	int cont=0;
	
	for (int i=0;i<lista->num;i++){
		printf("%s\n",lista->conectados[lista->num].nombre);
		printf("%s\n",lista->conectados[i].nombre);
		printf("cadena antes %s\n",cadena);
		sprintf (cadena,"%s|%s",cadena,lista->conectados[i].nombre);
		printf("cadena despues %s\n",cadena);
		cont ++;
	}
	
	sprintf(cadenafinal,"5/%d%s", lista->num, cadena);
	printf("%s\n",cadenafinal);
	
}


void CadenaPosicionesFichas (TListaPartidas *Partidas, char cadenaposicionesfichas [100])//Devuelve en formato cadena los jugadores  separados por un |
{
	
	char cadena2[200];
	int cont=0;
	sprintf (cadena2,"%d",Partidas->partida[0].identidad[i].posicion);
	
	for (int i=1;i<(Partidas->partida[0].jugadores);i++){
		
		sprintf (cadena2,"%s|%d",cadena2,Partidas->partida[0].identidad[i].posicion);		
		cont ++;
	}
	strcpy(cadenaposicionesfichas,cadena2);
	
	
}

void InsertarSQL (char cadenaInsertPartida[200], char tipoinsert[20]){ // Inserta los datos finales en la base de datos MYSQL
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [200];
	char res [80];
	
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	if (strcmp ( tipoinsert,"PARTIDA") ==0)
		sprintf(consulta,"INSERT INTO Partida (Id, Ganador, Perdedor, Fecha, PuntosGanador, PuntosPerdedor) VALUES (%s)", cadenaInsertPartida);
	if (strcmp ( tipoinsert,"RELACION") ==0)
		sprintf(consulta,"INSERT INTO Relacion (IdJugador, IdPartida, Puntos, Color) VALUES (%s)", cadenaInsertPartida);
	
	printf("Insertar la siguiente partida: %s\n",consulta);
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al introducir los datos en la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	mysql_close (conn);
	
	
}

int LogIn(char nombre[20],char contra[20]){//Hace el login del usuario y contraseña mysql
	
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	char res[80];
	
	
	//parte de mysql
	
	char nom[20], cnt[20]; //variables para comparar nombre y contraseña
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	strcpy(consulta,"SELECT Jugador.Nombre,Jugador.Pass FROM Jugador WHERE Jugador.Nombre = '");
	strcat(consulta, nombre);
	strcat(consulta, "'");
	
	
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	resultado = mysql_store_result (conn);
	
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) printf ("No se han obtenido datos en la consulta\n");
	
	else
	{
		strcpy(nom,row[0]);
		strcpy(cnt,row[1]);
		
		printf("nombre recibido: %s, contra recibida: %s\n", nom, cnt);
		row = mysql_fetch_row (resultado);
	}
	
	if(strcmp(nom,nombre) == 0  && strcmp(cnt,contra) == 0)
	{
		return 0;
	}
	
	else 
	   return 1;
	
	
}


int CheckNombre(char nombre[20],char contra[20]){ // funcion para ver si el nombre ya existe en la base de datos
	
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	char res[80];
	
	
	//parte de mysql
	
	char nom[20], cnt[20]; //variables para comparar nombre y contrase?a
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	strcpy(consulta,"SELECT Jugador.Nombre,Jugador.Pass FROM Jugador WHERE Jugador.Nombre = '");
	strcat(consulta, nombre);
	strcat(consulta, "'");
	
	
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	resultado = mysql_store_result (conn);
	
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) printf ("No se han obtenido datos en la consulta\n");
	
	else
	{
		strcpy(nom,row[0]);
		strcpy(cnt,row[1]);
		
		printf("nombre recibido: %s, contra recibida: %s\n", nom, cnt);
		row = mysql_fetch_row (resultado);
	}
	
	if(strcmp(nom,nombre) == 0)
	{
		return 0;
	}
	
	else 
	   return 1;
	
}


int EliminarJugador(char nombre[20], char pass[20]) //Función para eliminar de la base de datos a los usuarios que se dan de baja
{
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	char res[80];
		
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) 
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		return -1;
		exit(1);
	}
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
		sprintf(consulta,"DELETE from Jugador where Nombre = '%s' AND Pass = '%s'",nombre,pass);
		err=mysql_query (conn, consulta);
		if (err!=0) {
			printf ("Error al introducir los datos en la base %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		mysql_close (conn);
	
	
}


int Register(char insertarjugador[200], char tipoconsulta[20]){ // funcion para Registrar en la base de datos a un jugador que no estaba en ella o para Eliminar a un usuario en funcion de la consulta que reciba
	
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	char res[80];
	
	
	
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
		
		//if (strcmp ( tipoconsulta,"DELETE") ==0)	
		//sprintf(consulta,"DELETE from Jugador where Nombre = %s and Pass = %s", insertarjugador);
		//sprintf(consulta,"DELETE from Jugador where Nombre = %s AND Pass = %s",nom,cnt);
	else if (strcmp ( tipoconsulta,"INSERT") ==0)
		sprintf(consulta,"INSERT INTO Jugador (Nombre, Pass, Edad) VALUES (%s)", insertarjugador);
	else
		return -1;
	//printf("Insertar la siguiente partida: %s\n",consulta);
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al introducir los datos en la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	mysql_close (conn);

	
	
}
// Consulta el procentaje de un color en las partidas jugadas, es una pregunta del formulario del cliente
void PorcentajeColor(char p[80], char respuesta[200])   {
	
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	char res[80];
	int total_jugadores;
	int jugador_color;
	float tanto_porciento;
	
	//arrancamos
	p = strtok(NULL, "/");
	char color[20];
	strcpy (color, p);
	//miramos por pantalla si realmente llega el color bien
	printf("color: %s\n", color);
	//Variable interna 
	
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) 
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL)
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	strcpy (consulta,"SELECT count(*) FROM Relacion");
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
		//En la columna 0 esta el valor total de jugadores y lo guardo en la variable total_jugadores
		printf ("Los jugadores que han jugado en las partidas es : %s\n", row[0]);
	total_jugadores = atol(row[0]);
	
	
	
	
	
	//Cuento el total de jugadores que han jugado una partida con el colorespecifico
	strcpy (consulta,"SELECT count(*) FROM Relacion WHERE Color = '");
	strcat (consulta, color);
	strcat (consulta,"'");
	
	err=mysql_query (conn, consulta);
	
	
	
	if (err!=0) 
	{
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	resultado = mysql_store_result (conn);
	
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) printf ("No se han obtenido datos en la consulta\n");
	else
	{
		printf ("El total de jugadores que han jugado con el %s es : %s\n", color, row[0]);
		jugador_color = atol(row[0]);
	}	
	
	tanto_porciento = ((float )jugador_color / (float) total_jugadores)*100;
	//respuesta = jugador_color;
	//strcpy(respuesta, jugador_color);
	sprintf(respuesta,"4/%.2f",tanto_porciento);
	printf ("El tanto porciento de jugadores %s es: %.2f %\n", color, respuesta);
}

// Consulta la edad media, es una pregunta del formulario del cliente
void EdadColor(char p[80], char respuesta[200]) {
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[200];
	char res[80];
	
	//Arrancamos atributo
	p = strtok(NULL, "/");
	char color[20];
	strcpy (color, p);
	//miramos por pantalla si realmente llega el color bien
	printf("color: %s\n", color);
	//Variable interna 
	int edad;
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	strcpy(consulta,"SELECT AVG(Jugador.Edad) FROM Jugador, Relacion, Partida WHERE Jugador.Id = Relacion.IdJugador AND Relacion.IdPartida = Partida.Id AND Relacion.Color ='");
	strcat(consulta, color);
	strcat(consulta, "'");
	
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	resultado = mysql_store_result (conn);
	
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) printf ("No se han obtenido datos en la consulta\n");
	else{
		edad = atoi(row[0]);
		printf("Edad recibida: %d\n", edad);
		row = mysql_fetch_row (resultado);
	}		
	sprintf(respuesta,"2/%d",edad); //convertimos a string el entero "edad" y se lo copiamos a respuesta
	
	
}




void GanadorFecha (char p[80], char respuesta[200]){// consulta ganador de fecha que introduzca usuario
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	char res [80];
	
	// arrancamos 
	p = strtok(NULL, "/");
	char fecha[20];
	strcpy(fecha,p);
	p=strtok(NULL,"/");
	
	// hacemos mysql
	printf("fecha: %s\n", fecha); //2018-06-13
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	strcpy(consulta,"SELECT Partida.Ganador FROM Partida WHERE Partida.Fecha='");
	strcat(consulta, fecha);
	strcat(consulta, "'");
	printf("hay: %s\n",consulta);
	err=mysql_query (conn, consulta);
	
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	resultado = mysql_store_result (conn);
	
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) printf ("No se han obtenido datos en la consulta\n");
	else{
		
		sprintf(resultado ,"%s\n",row[0]);
		row = mysql_fetch_row (resultado);
	}		
	sprintf(respuesta,"3/%s\n",resultado);
	
	
}

int  NumeroPartida()   { // funcion para conocer la ultima partida por mysql
	
	
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	char res[80];
	int total_jugadores;
	int jugador_color;
	int numero_partida;
	float tanto_porciento;
	
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) 
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL)
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	strcpy (consulta,"SELECT count(*) FROM Partida");
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
	{
		printf ("el numero de Patidas es : %s\n", row[0]);
		numero_partida = atoi(row[0]);
	}
	
	return numero_partida +1;
	
}



int  consultaIdJugador(char idJugadorSQL[200])   {//consulta el Id del jugador para poderlo insertar en la tabla relacion de mysql
	
	
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	char res[80];
	int total_jugadores;
	int jugador_color;
	int numero_partida;
	float tanto_porciento;
	int idJug;
	
	
	conn = mysql_init(NULL);
	
	if (conn==NULL) 
	{
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M4_BBDDjuego",0, NULL, 0);
	
	if (conn==NULL)
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	sprintf(consulta,"SELECT Id from Jugador where Nombre = '%s'", idJugadorSQL);
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
	{
		printf ("el numero de Patidas es : %s\n", row[0]);
		idJug = atoi(row[0]);
	}
	
	return idJug;
	
}

int CrearConversacion(TListaPartidas *Partidas, int num, int sockets[20], int socket_host, char nombre_host[80])//Transmite las invitaciones // socket host es el socket del q crea la partida	
{
	
	char enviarIDPartidas [80];
	int id = Partidas->numero;
	char invitacion[512];
	
	printf("numero de jugadores : %d \n",num);		
	
	if(id==100)
		
	{
		return -1;		
	}
	
	
	
	
	sprintf(enviarIDPartidas,"10/%d",id);//
	write (socket_host,enviarIDPartidas,strlen(enviarIDPartidas));//Le enviamos el id partidas tambien al invitador
	
	printf("respartida : %d \n", &Partidas->numero);
	sprintf(invitacion,"7/Invitado/%s/%d/",nombre_host,id);//Nombre de quien me invita y id de la partida
	
	for(int i=0;i<num;i++)
		
	{		
		write(sockets[i+1],invitacion,strlen(invitacion));
		printf("Envio Peticion al socket: %d con id partidas : %d \n",sockets[i+1],id);		
	}
	
	printf("socketsChat al salir de la funcion CrearConversacion : %d/%d/%d/%d",sockets[0],sockets[1],sockets[2],sockets[3]);
	return 0;
	
}




pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *AtenderCliente(void *socket){ // funcion donde atendemos las peticiones del cliente 
	
	int sock_conn;
	int *s;
	s= (int *) socket;
	sock_conn= *s;
	
	
	char peticion[512];
	char respuesta[512];
	int ret;
	int res;
	char minombre [70];
	char conectados[250];
	char temporal[250];
	char Invitador[80];
	int socketInvitador;
	int misocket;
	int IDpartida;
	int num;
	
	
	
	int terminar =0;
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar ==0)
	{
		// Ahora recibimos la petici?n
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que añadirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		
		printf ("Peticion: %s\n",peticion);
		
		// vamos a ver que quieren
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		// Ya tenemos el c?digo de la petici?n
		char nombre[20];
		
		if (codigo == 0)
		{ //salimos del bucle
			
			
			p=strtok(NULL,"/");
			strcpy(nombre,p);
			printf("Lista conectados antes de elimiar : %s\n",cadenafinal);			
			int res = Eliminar(&lista,nombre); 
			if(res==0)
			{	 
				Cadena(&lista,cadenafinal);
				printf("Lista conectados despues de elimiar : %s\n",cadenafinal);
				printf("Se ha eliminado correctamente\n");
				
				for(int j=0;j<i;j++)
				{
					
					write(sockets[j],cadenafinal, strlen(cadenafinal));						
				}
			}
			else 
			{
				printf("No hay elemento a eliminar!\n");
			}
			
			terminar = 1;
			
		}
		
		else if (codigo == 1) // consulta 1: nombre y contraseña en la base de datos?
		{	
			
			//arrancamos los atributos nombre y contraseña 
			p = strtok(NULL, "/");
			char nombre[20];
			strcpy (nombre, p);
			p = strtok(NULL, "/");
			char contra[20];
			strcpy (contra, p);
			printf ("Nombre: %s, Contra: %s\n", nombre, contra);
			
			res=LogIn(nombre,contra); 
			
			if (res==0)
				strcpy(respuesta,"1/SI"); 
			else
				strcpy(respuesta,"1/NO"); 
			//LogIn(p, respuesta); 
			
			if(strcmp(respuesta,"1/SI")==0)
			{
				res = Pon(&lista,nombre,sock_conn);
				
				if(res==1)
				{
					printf("Se ha a?adido correctamente--\n");
					
					Cadena(&lista,cadenafinal);
					/*lista.conectados[i].socket*/
					for(int i=0;i<lista.num;i++){
						write(lista.conectados[i].socket,cadenafinal, strlen(cadenafinal));	
						printf("Enviado al socket %d \n", lista.conectados[i].socket );
						printf("Enviado la cadena %s \n", cadenafinal );
					}
				}
				else 
				{
					printf("Lista llena !! merluzo\n");
				}
			}
		}
		else if (codigo==2)//Consulta 2:  Consulta la edad media, es una pregunta del formulario del cliente
		{
			EdadColor(p,respuesta);
		}
		
		else if (codigo== 3) // consulta 3: ganador esa fecha 
		{ 
			GanadorFecha(p, respuesta);
		}
		else if (codigo==4)//Consulta 4:   Consulta el procentaje de un color en las partidas jugadas
		
		{
			PorcentajeColor(p,respuesta);
		}

				else if (codigo == 6) //Respuesta Acepta o Rechaza
		{
			char respuesta2[500];
			char respuesta3[500];
			char respuesta4[500];
			char respuesta5[500];
			char respuesta6[500];
			respuesta2[0] = '\0';
			respuesta3[0] = '\0';
			respuesta4[0] = '\0';
			respuesta5[0] = '\0';
			respuesta6[0] = '\0';
			
			turno = 0;
			int rechazar = 0;
			int acepta;
			p = strtok (NULL,"/");
			strcpy(minombre,p);
			
			misocket = DameSocket(&lista,minombre);
			printf("Mi nombre : %s y Socket : %d \n",minombre,misocket);
			
			p = strtok (NULL,"/");
			strcpy(Invitador,p);
			socketInvitador = DameSocket(&lista,Invitador);
			printf("Nombre Invitador : %s y Socket : %d\n",Invitador,socketInvitador);
			
			p = strtok (NULL,"/");
			acepta =atoi(p);
			
			if (misocket != -1 && socketInvitador!=-1)
			{
				
				
				if(acepta==1) // Si lo que me ha enviado es 1 enviamos aceptar sino enviamos que la ha rechazado
				{
					
					
					int posicion = 0;
					int numPartida = NumeroPartida();						
					printf(" 6 numero partida invitado  : %d, %s, %d\n",numPartida, minombre, misocket);	
					int resPartida = PonPartida(&Partidas,minombre,misocket, posicion, numPartida);
					CadenaPartida(&Partidas,cadenafinalpartida);
					
					printf(" barra 0 : %s\n",respuesta2);					
					sprintf(respuesta2,"9/crear/%s",cadenafinalpartida); //Lo enviamos para mostrar el chat en el cliente					
					write (socketInvitador,respuesta2,strlen(respuesta2));
					printf("Respuesta2 9crear inv enviada : %s\n",respuesta2);
					
					
					
					sprintf(respuesta4,"6/Acepta/%s/",minombre);					
					write(socketInvitador,respuesta4,strlen(respuesta4));					
					printf("Respuesta4 aceptar inv enviada : %s\n",respuesta4);
					
					sprintf(respuesta3,"9/crear/%s",cadenafinalpartida); //Lo enviamos para mostrar el chat en el cliente
					write (misocket,respuesta3,strlen(respuesta3));
					printf("Respuesta2 9crear mysoc enviada : %s\n",respuesta3);
					
					
				}
				else
				{
					rechazar = 1;
					sprintf (respuesta2,"6/Rechaza/%s",nombre);
					write(socketInvitador,respuesta2,strlen(respuesta2));
					
				}
			}
			
			else
			{
				printf("Error al encontrar sockets");
			}
		}
		
		
		
		else if (codigo==7) // Invitacion
		{
			respuesta[0]='\0';
			
			p= strtok (NULL,"/");
			strcpy(Invitador,p);
			
			int socketinvitador = DameSocket(&lista,Invitador);
			printf("socket invitador: %d\n", socketinvitador);
			socketsChatfuturo[0] = socketinvitador; //Guardamos los sockets en el vector SocketsChat
			
			p = strtok( NULL, "/");
			numeroinvitados=atoi(p);
			
			printf("Numero de invitados: %d\n", numeroinvitados);
			nombre[0]='\0';
			
			
			for (int i=0;i<numeroinvitados;i++)
			{
				p = strtok( NULL, ",");
				strcpy(nombre,p);
				printf("Nombre: %s\n",nombre);
				
				int err = DameSocket(&lista,nombre);
				socketsChatfuturo[i+1]= err; // Mas 1 Ya que en la primera posicion esta el socket del que invita
				
			}
			
			int posicion = 0;
			int numPartida = NumeroPartida();						
			printf("7 datos invitador partida  : %d, %s , %d \n",numPartida, Invitador, socketinvitador);	
			int resPartida = PonPartida (&Partidas,Invitador,socketinvitador, posicion, numPartida);
			
			pthread_mutex_lock;
			
			int res = CrearConversacion(&Partidas,numeroinvitados,socketsChatfuturo,socketinvitador,Invitador); //Funcion con la que nos podremos intercanviar mensajes
			pthread_mutex_unlock;
			
			
			if (res !=0)
				printf ("error\n");
			else
				printf("Enviamos solicitudes \n");
			
			
		}
		
		
		
		else if (codigo == 8) //Quieren enviar un mensaje, nosotros entendemos un mensaje como una notificacion, o asi la trataremos en adelante
		{
			char respuesta2[500];	
			
			p = strtok( NULL, "/");
			
			char mensaje[200];
			
			strcpy(mensaje,p);
			
			char nom[20];
			
			p = strtok(NULL, "/");
			
			strcpy(nom, p);
			
			
			pthread_mutex_lock;
			
			
			sprintf(respuesta2, "8/%s/%s/", mensaje, nom);
			
			for(int i=0;i<lista.num;i++){
				printf("respuesta2 --> %s\n", respuesta2);
				write(sockets[i],respuesta2, strlen(respuesta2));						
			}
			pthread_mutex_unlock;
		}
		
		
		else if (codigo == 9) //Se envia inicio de partida
		{
			char respuesta106[500];	
			
			
			CadenaPartida(&Partidas,cadenafinalpartida);
			printf(" cadenafinal partida: %s\n",cadenafinalpartida);	
			
			// CASOS A PROBAR CON POSICIONES INICIALES
			
			/*			Partidas.partida[0].identidad[0].posicion =0;*/
			/*			Partidas.partida[0].identidad[1].posicion =51;*/
			/*			Partidas.partida[0].identidad[2].posicion =34;*/
			/*			Partidas.partida[0].identidad[3].posicion =17;*/
			
			/*				Partidas.partida[Partidas.numero].identidad[0].posicion =64;*/
			/*				Partidas.partida[Partidas.numero].identidad[1].posicion =100;*/
			/*				Partidas.partida[Partidas.numero].identidad[2].posicion =92;*/
			/*				Partidas.partida[Partidas.numero].identidad[3].posicion =17;*/
			
			// Posiciones Iniciales
			Partidas.partida[Partidas.numero].identidad[0].posicion =130;
			Partidas.partida[Partidas.numero].identidad[1].posicion =131;
			Partidas.partida[Partidas.numero].identidad[2].posicion =132;
			Partidas.partida[Partidas.numero].identidad[3].posicion =133;
			//Colores iniciales
			strcpy (Partidas.partida[Partidas.numero].identidad[0].color ,"rojo");
			strcpy (Partidas.partida[Partidas.numero].identidad[1].color ,"verde");
			strcpy (Partidas.partida[Partidas.numero].identidad[2].color ,"azul");
			strcpy (Partidas.partida[Partidas.numero].identidad[3].color ,"amarillo");
			
			
			printf(" Lapartida es la numero : %d\n",Partidas.numero);			
			
			sprintf(respuesta106,"11/%s",cadenafinalpartida); //Enviamos los paticipantes 11/numeroparticipantes|nombre1|nombre2					
			// envioa todos los jugadores de la partida
			for(int i=0;i<Partidas.partida[Partidas.numero].jugadores;i++){
				
				write(Partidas.partida[Partidas.numero].identidad[i].socket,respuesta106, strlen(respuesta106));
				
			}
			
			printf("Respuesta enviada al iniciar : %s\n",respuesta106);
		}
		else if (codigo == 10) //Llega el turno y la posicion del jugador que ha tirado el dado (turno)
		{
			char respuesta103[100];	
			char respuesta104[100];
			char respuesta105[100];
			
			int posicionturno;
			int contadorturno2;
			contadorturno2 = contadorturno2 +1 ;
			
			int jugadorturno;
			p = strtok (NULL,"/");
			posicionturno =atoi(p);
			p = strtok (NULL,"/");
			jugadorturno =atoi(p);
			p = strtok (NULL,"/");
			int diferencia;
			diferencia = atoi(p);
			printf(" posicionturno %d\n",posicionturno);
			printf(" jugadorturno : %d\n",jugadorturno);
			
			
			posicionturno = Partidas.partida[Partidas.numero].identidad[jugadorturno].posicion + posicionturno;
			Partidas.partida[Partidas.numero].identidad[jugadorturno].posicion = posicionturno;
			printf(" posicion de la lista : %d\n",Partidas.partida[Partidas.numero].identidad[jugadorturno].posicion);
			printf(" posicion turno : %d\n",posicionturno);
			printf(" jugadores : %d\n",Partidas.partida[Partidas.numero].jugadores);
			
			//calculo del turno siguiente y turno2 del jugador actual de la partida
			turno = (turno + 1) % Partidas.partida[Partidas.numero].jugadores;
			if (turno == 1)
			{
				turno2 = 0;
			}
			else
			{
				turno2 = (turno2 + 1) % Partidas.partida[Partidas.numero].jugadores;
			}	
			
			
			//Posicion de cada participantes	
			sprintf(respuesta105,"%d|%d|%d|%d",Partidas.partida[Partidas.numero].identidad[0].posicion,Partidas.partida[Partidas.numero].identidad[1].posicion,Partidas.partida[Partidas.numero].identidad[2].posicion,Partidas.partida[Partidas.numero].identidad[3].posicion);
			strcpy(respuesta104,respuesta105);
			
			//turno + Posicion de cada participantes
			
			sprintf(respuesta103,"12/%d/%d/%s/",turno,turno2,respuesta104); 				
			printf(" Envio de turno y Posiciones2 : %s\n",respuesta103);	
			
			
			for(int i=0;i<Partidas.partida[Partidas.numero].jugadores;i++){			
				write(Partidas.partida[Partidas.numero].identidad[i].socket,respuesta103, strlen(respuesta103));
				
			}
		}
		else if (codigo == 15) // se envia fin partida
		{
			char respuesta108[100];	
			char ganador[20];;
			char cadenainsertar[500];
			char cadenainsertar2[500];
			int partidas;
			char perdedor[20];
			char fechaganador[100];
			char fechaganador2[100];
			int puntosGanador;
			int idJugador;
			int puntosPerdedor;
			int puntos1;
			int puntos2;
			int puntos3;
			int puntos4;
			p = strtok (NULL,"/");
			sprintf(ganador,p);
			printf("ganador es: %s\n",ganador);
			p = strtok (NULL,"/");
			sprintf(fechaganador,p);
			printf("fecha ganador %s\n", fechaganador);
			p = strtok (NULL,"/");
			sprintf(perdedor,p);
			printf("perdedor %s\n", perdedor);
			p = strtok (NULL,"/");
			puntosPerdedor = atoi (p);
			printf("puntos perdedor %d\n", puntosPerdedor);
			p = strtok (NULL,"/");
			puntos1 = atoi (p);
			printf("puntos 1 %d\n", puntos1);
			p = strtok (NULL,"/");
			puntos2 = atoi (p);
			printf("puntos 2 %d\n", puntos2);
			p = strtok (NULL,"/");
			puntos3 = atoi (p);
			printf("puntos 3 %d\n", puntos3);
			p = strtok (NULL,"/");
			puntos4 = atoi (p);
			printf("puntos 4 %d\n", puntos4);
			
			Partidas.partida[Partidas.numero].identidad[0].posicion =puntos1;
			Partidas.partida[Partidas.numero].identidad[1].posicion =puntos2;
			Partidas.partida[Partidas.numero].identidad[2].posicion =puntos3;
			Partidas.partida[Partidas.numero].identidad[3].posicion =puntos4;
			
			
			puntosGanador = 125;
			
			partidas = Partidas.partida[Partidas.numero].idpartida;
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			//Insertar datos de la Partida en mysql
			sprintf (cadenainsertar, "%d,'%s','%s','%s',%d,%d",partidas,ganador,perdedor,fechaganador, puntosGanador, puntosPerdedor);
			InsertarSQL(cadenainsertar, "PARTIDA");
			
			
			// Insertar los datos de la relaciones
			for(int i=0;i<Partidas.partida[Partidas.numero].jugadores;i++)
			{
				
				printf("jugador %s\n", Partidas.partida[Partidas.numero].identidad[i].nombre);
				idJugador = consultaIdJugador(Partidas.partida[Partidas.numero].identidad[i].nombre);
				printf("idJugador %d\n", idJugador);
				
				sprintf (cadenainsertar2, "%d,'%d','%d','%s'",idJugador,partidas,Partidas.partida[Partidas.numero].identidad[i].posicion,Partidas.partida[Partidas.numero].identidad[i].color);
				InsertarSQL(cadenainsertar2, "RELACION");
				
			}
			//Enviar a todos los jugadores de la partida un FIN PARTIDA
			strcpy(respuesta108,"15/FIN PARTIDA");
			
			for(int i=0;i<Partidas.partida[Partidas.numero].jugadores;i++){
				
				write(Partidas.partida[Partidas.numero].identidad[i].socket,respuesta108, strlen(respuesta108));
				
			}
			
			
			//Se incrementa el numero de partida para la siguiente
			Partidas.numero++;
			
		}
		
		
		else if (codigo == 16) // Realiza el REGISTER de la base de datos, chekea si existe primero un usuario con el mismo nombre 
		{	
			
			
			char insertarjugador[200];
			p = strtok(NULL, "/");
			char nombre[20];
			strcpy (nombre, p);
			p = strtok(NULL, "/");
			char contra[20];
			strcpy (contra, p);
			int edad = 23;
			printf ("Nombre: %s, Contra: %s\n", nombre, contra);
			// Chekea si existe el nombre
			res=CheckNombre(nombre,contra); 
			if (res==0)
				strcpy(respuesta,"13/No se puede Insertar, usuario ya existe"); 
			//Mandar mensaje no se puede insertar ya existe
			else
			{
				sprintf (insertarjugador, "'%s','%s',%d",nombre,contra,edad);
				res=Register(insertarjugador, "INSERT");
				strcpy(respuesta,"13/Insertado correctamente, ya puede hacer login");
				//Realiza el Insert
			}
			
		}
		
		else if (codigo == 17) // Realiza el DELETE del usuario, primero comprueba si existe
		{	
			
			//arrancamos los atributos nombre y contra 
			char borrarjugador[200];
			p = strtok(NULL, "/");
			char nombre[20];
			strcpy (nombre, p);
			p = strtok(NULL, "/");
			char contra[20];
			strcpy (contra, p);
			int edad = 23;
			printf ("Nombre: %s, Contra: %s\n", nombre, contra);
			//chekea si existe el usuarioy pass en la tabla jugadores
			res=LogIn(nombre,contra); 
			if (res==0)
			{
				res=EliminarJugador(nombre,contra);
				if (res != -1)
				{
				printf("usuario %s borrado",nombre);
				strcpy(respuesta,"16/1");
				}
					
			}
			else
				strcpy(respuesta,"16/0"); 

			
		}
		
	
		
		
		else if (codigo == 18) //Se envia inicio de partida
		{
			char respuesta100[500];	
			char respuesta101[500];
			char respuesta102[500];
			
			p = strtok (NULL,"/");
			int Posicion1;
			Posicion1 = atoi(p);
			p = strtok (NULL,"/");
			int Posicion2;
			Posicion2 = atoi(p);
			p = strtok (NULL,"/");
			int Posicion3;
			Posicion3 = atoi(p);
			p = strtok (NULL,"/");
			int Posicion4;
			Posicion4 = atoi(p);
			
			
			printf("Posicion 1 2 3 4  : %d , %d , %d , %d\n",Posicion1, Posicion2, Posicion3, Posicion4);
			
			
			Partidas.partida[Partidas.numero].identidad[0].posicion =Posicion1;
			Partidas.partida[Partidas.numero].identidad[1].posicion =Posicion2;
			Partidas.partida[Partidas.numero].identidad[2].posicion =Posicion3;
			Partidas.partida[Partidas.numero].identidad[3].posicion =Posicion4;
			
			//Posicion de cada participantes	
			sprintf(respuesta102,"%d|%d|%d|%d",Partidas.partida[Partidas.numero].identidad[0].posicion,Partidas.partida[Partidas.numero].identidad[1].posicion,Partidas.partida[Partidas.numero].identidad[2].posicion,Partidas.partida[Partidas.numero].identidad[3].posicion);
			strcpy(respuesta101,respuesta102);
			
			//turno + Posicion de cada participantes
			
			sprintf(respuesta100,"14/%d/%s/",turno,respuesta101); 				
			printf(" Envio de turno y Posiciones2 : %s\n",respuesta100);				
			
			
			
			for(int i=0;i<Partidas.partida[Partidas.numero].jugadores;i++){
				
				write(Partidas.partida[Partidas.numero].identidad[i].socket,respuesta100, strlen(respuesta100));
				
			}
			
			
		}
		
		if ((codigo !=0)&&(codigo !=6)&&(codigo!=8)&&(codigo!=9)&&(codigo!=10)&&(codigo!=15)&&(codigo!=18))
		{
			
			printf ("Respuesta enviada: %s %d\n", respuesta, sock_conn);
			//Enviamos respuesta
			write (sock_conn,respuesta, strlen(respuesta));
		}
		
		if ((codigo ==1)||(codigo==2)|| (codigo==3)|| (codigo==4)|| (codigo==5)|| (codigo==6)|| (codigo==7))
		{
			pthread_mutex_lock( &mutex ); //No me interrumpas ahora
			contador = contador +1;
			pthread_mutex_unlock( &mutex); //ya puedes interrumpirme
			
		}
		
		
	}
	
	
	close(sock_conn); 
}



int main(int argc, char *argv[]) // inicializaciones para conectarse con el cliente ip y puertos 
{
	int id =0;
	Partidas.numero =0;
	Partidas.partida[0].jugadores =0;
	Partidas.partida[1].jugadores=0;
	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9040);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	
	pthread_t thread;
	i=0;
	
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		
		sockets[i] =sock_conn;
		//sock_conn es el socket que usaremos para este cliente
		
		// Crear thead y decirle lo que tiene que hacer
		
		pthread_create (&thread, NULL, AtenderCliente,&sockets[i]);
		i=i+1;
		
		
		
	}
	
	
	
}


