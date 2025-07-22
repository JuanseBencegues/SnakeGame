#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
/* Paquetes de librerias que uso:
	libncurses5-dev ; libncursesw5-dev
	
Para compilar:
linkear librerias al compilador -> gcc snake.c -lncurses
	*/


typedef struct {
	int x;
	int y;
}	vec2;

int puntos = 0;

int main(){
	const int ANCHO_PANTALLA = 40;
	const int ALTO_PANTALLA = 20;
	
	// Inicializa la pantalla
	WINDOW *win = initscr();
	
	// Inputs del jugador
	keypad(win,true);
	nodelay(win,true);
	curs_set(0); //Esconde cursor
	
	// Frutas

	vec2 fruta = {rand() % ANCHO_PANTALLA, rand() % ALTO_PANTALLA};

	// Serpiente
	vec2 cabeza = {0,0};
	vec2 dir = {1,0};

	//------------- Loop de juego -------------

	while(true){
		int tecla_jugada = wgetch(win);
		if (tecla_jugada == KEY_LEFT){
			if (dir.x == 1) continue; 	//Evita hacer un giro de 180°
			dir.x = -1;
			dir.y = 0;
		}
		if (tecla_jugada == KEY_RIGHT){
			if (dir.x == -1) continue;
			dir.x = 1;
			dir.y = 0;
		}
		if (tecla_jugada == KEY_UP){
			if (dir.y == 1) continue;
			dir.x = 0;
			dir.y = -1;
		}
		if (tecla_jugada == KEY_DOWN){
			if (dir.y == -1) continue;
			dir.x = 0;
			dir.y = 1;
		}
		if (tecla_jugada == '\e'){
			break;
		}
		/*------- Actualizar elementos -------*/
		cabeza.x += dir.x;
		cabeza.y += dir.y;

		if (cabeza.x == fruta.x && cabeza.y == fruta.y){
			puntos += 100; 	//aumenta puntaje

			//Actualiza posición fruta
			fruta.x = rand() % ANCHO_PANTALLA;
			fruta.y = rand() % ALTO_PANTALLA;
		}
		
		/*------- Dibujar pantalla -------*/
		erase();	//Actualiza pantalla a mostrar, borrando lo anterior
		mvaddch(fruta.y, fruta.x * 2, '@');		//Dibuja fruta
		mvaddch(cabeza.y, cabeza.x * 2, 'O');	//Dibuja cabeza serpiente	
		usleep(125000);	//Establece tiempo de espera antes de dibujar de nuevo
	}

	endwin();

	return 0;
}
