#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

// QUEDA CREAR DRAW Y ACTUALIZAR MAIN
/* Paquetes de librerias que uso:
	libncurses5-dev ; libncursesw5-dev
	
Para compilar:
linkear librerias al compilador -> gcc snake.c -lncurses
	*/
#define PUNTAJE_MAXIMO 256
#define FRAME_TIME 110000

typedef struct {
	int x;
	int y;
}	vec2;

int puntos = 0;
char msj_puntuacion[16];

bool skip = false;
bool corriendo = true;

int ANCHO_PANTALLA = 25;
int ALTO_PANTALLA = 20;

// Inicializa la pantalla
WINDOW *win;

// Serpiente
vec2 cabeza = {0,0};
vec2 cuerpo[PUNTAJE_MAXIMO + 1];
vec2 dir = {1,0};

// Fruta
vec2 fruta;


void init(){
	srand(time(NULL));
	// Inicia ventana
	win = initscr();
	// Inputs del jugador
	keypad(win,true);
	noecho();
	nodelay(win,true);
	curs_set(0); //Esconde cursor

	// Inicia colores
	if (has_colors() == FALSE){	//Chequea si se pueden usar colores
		endwin();
		fprintf(stderr,"La terminal no soporta color\n");
		exit(1);
	}
	start_color();
	use_default_colors();
	init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_GREEN, -1);
	init_pair(3, COLOR_YELLOW, -1);

	// Inicializa fruta
	fruta.x = rand() % ANCHO_PANTALLA;
	fruta.y = rand() % ALTO_PANTALLA;

	// Actualiza contador de puntos
	sprintf(msj_puntuacion,"[ PUNTOS: %d ]",puntos);

}

void salir_juego(){
	//Salir de la aplicacion
	endwin();
	//limpia pantalla, posiciona cursor y lo muestra
	printf("\e[1;1H\e[2J");
	printf("\e[?25h");
	exit(0);
}


void reiniciar_juego(){
	//Reinicia variables
	cabeza.x = 0;
	cabeza.y = 0;
	dir.x = 1;
	dir.y = 0;
	puntos = 0;
	sprintf(msj_puntuacion,"[ PUNTOS: %d ]", puntos);
	corriendo = true;
}

void procesar_input(){
	int tecla_jugada = wgetch(win);
		if (tecla_jugada == KEY_LEFT){
			if (dir.x == 1){	//Evita hacer un giro de 180°
				return;
				skip = true;
			} 
			dir.x = -1;
			dir.y = 0;
		}
		if (tecla_jugada == KEY_RIGHT){
			if (dir.x == -1){	
				return;
				skip = true;
			} 
			dir.x = 1;
			dir.y = 0;
		}
		if (tecla_jugada == KEY_UP){
			if (dir.y == 1){	
				return;
				skip = true;
			} 
			dir.x = 0;
			dir.y = -1;
		}
		if (tecla_jugada == KEY_DOWN){
			if (dir.y == -1) {	
				return;
				skip = true;
			} 
			dir.x = 0;
			dir.y = 1;
		}
		if (tecla_jugada == ' '){
			if (!corriendo){
				reiniciar_juego();
			}
		}
		if (tecla_jugada == '\e'){
			corriendo = false;
			salir_juego();
		}
}

void dibujar_borde(int y, int x, int ancho, int alto){
	// Fila superior 
	mvaddch(y,x,ACS_ULCORNER);	//Esquina izquierda sup
	mvaddch(y,x + ancho * 2 + 1, ACS_URCORNER);  //Esquina derecha sup
	for (int i = 1; i < ancho * 2 + 1; i++){
		mvaddch(y, x + i, ACS_HLINE);	//linea superior
	}
	// Columnas
	for (int i = 1; i < alto + 1; i++){
		mvaddch(y + i, x, ACS_VLINE);
		mvaddch(y + i, x + ancho * 2 + 1, ACS_VLINE);
	}
	// Fila inferior
	mvaddch(y + alto + 1, x , ACS_LLCORNER);	//Esquina izquierda inf
	mvaddch(y + alto + 1, x , ACS_LRCORNER);	//Esquina derecha inf
	for (int i = 1; i < ancho * 2 + 1; i++){
		mvaddch(y + alto + 1, x + i, ACS_HLINE);	//linea inferior
	}

}

/*------------- Colisiones -------------*/
// Determinar si hay colision entre dos vectores
bool colision(vec2 a, vec2 b){
	if (a.x == b.x && a.y == b.y){
		return true;
	}
	else return false;
}

// Determinar si hay colision con el cuerpo de la serpiente
bool colision_cuerpo_serpiente(vec2 punto){
	for (int i = 0; i < puntos; i++){
		if (colision(punto, cuerpo[i])){
			return true;
		}
	}
	return false;
}

// Determina fin del juego
void fin_juego(){
	while (corriendo == false){
		procesar_input();
		// Mensajes de fin de juego
		mvaddstr(ALTO_PANTALLA / 2, ANCHO_PANTALLA - 16, "              Fin del Juego              ");
		mvaddstr(ALTO_PANTALLA / 2 + 1, ANCHO_PANTALLA - 16, "[ESPACIO] reiniciar, [ESC] salir...");
		attron(COLOR_PAIR(3));
		dibujar_borde(ALTO_PANTALLA / 2 - 1, ANCHO_PANTALLA - 17, 17, 2);
		attroff(COLOR_PAIR(3));

		usleep(FRAME_TIME);
	}
}

// Determina nueva posicion de fruta
vec2 spawn_fruta(){
	// La nueva fruta aparece a un pixel de los bordes y fuera del cuerpo de la serpiente
	vec2 fruta = {1 + rand() % (ANCHO_PANTALLA - 2), 1 + rand() % (ALTO_PANTALLA - 2)};
	while (colision(cabeza,fruta) || colision_cuerpo_serpiente(fruta)){
		fruta.x = 1 + rand() % (ANCHO_PANTALLA - 2);
		fruta.y = 1 + rand() % (ALTO_PANTALLA - 2);
	}
	return fruta;
} 

void actualiza(){
	// Actualizar cuerpo serpiente
	for (int i = puntos; i > 0; i--){
		cuerpo[i] = cuerpo[i - 1];
	}
	cuerpo[0] = cabeza; /*segmento siguiente a la cabeza va a ser igual a donde estaba la cabeza el paso anterior */
	
	// Mueve serpiente
	cabeza.x += dir.x;
	cabeza.y += dir.y;

	// Chequea colision con cuerpo o paredes
	if (colision_cuerpo_serpiente(cabeza) || cabeza.x < 0 || cabeza.y < 0 \
		|| cabeza.x >= ANCHO_PANTALLA || cabeza.y >= ALTO_PANTALLA){
		corriendo = false;
		fin_juego();
	}

	// Comer fruta
	if (colision(cabeza, fruta)){
		if (puntos < PUNTAJE_MAXIMO){
			puntos += 1;
			sprintf(msj_puntuacion,"[ PUNTOS: %d ]", puntos * 100);
		}
		else{
			// VICTORIA!
			printf("GANASTE!");
		}
		fruta = spawn_fruta();
	}

	usleep(FRAME_TIME);
}

void dibujar(){
	erase();  //Actualiza pantalla a mostrar, borrando lo anterior

	attron(COLOR_PAIR(1));
	mvaddch(fruta.y + 1, fruta.x * 2 + 1, '@');	//Dibuja fruta
	attroff(COLOR_PAIR(1));

	// Dibujar serpiente
	attron(COLOR_PAIR(2));
	for(int i = 0; i < puntos; i++){
			mvaddch(cuerpo[i].y + 1, cuerpo[i].x * 2 + 1, ACS_DIAMOND);
		}
	mvaddch(cabeza.y + 1, cabeza.x * 2 + 1, 'O');	//Dibuja cabeza serpiente	
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(3));
	dibujar_borde( 0, 0, ANCHO_PANTALLA, ALTO_PANTALLA);
	attroff(COLOR_PAIR(3));
	mvaddstr(0, ANCHO_PANTALLA - 5, msj_puntuacion);
}

int main(){
	init();	// Inicializa todo lo que hay que inicializar
	//------------- Loop de juego -------------
	while(true){
		procesar_input();	//toma entrada del jugador
		if (skip == true){
			skip = false;
			continue;
		}
		actualiza();	//posición de serpiente y puntos
		dibujar();		//Dibuja en pantalla
	}
	salir_juego();
	return 0;
}
