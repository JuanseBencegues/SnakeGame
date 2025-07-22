#include <unistd.h>
#include <curses.h>
/* Paquetes de librerias que uso:
	libncurses5-dev ; libncursesw5-dev
	
Para compilar:
linkear librerias al compilador -> gcc snake.c -lncurses
	*/


typedef struct {
	int x;
	int y;
}	vec2;

int main(){
	#define ANCHO_PANTALLA 40;
	#define ALTO_PANTALLA 20;
	
	//Inicializa la pantalla
	WINDOW *win = initscr();
	
	//Inputs del jugador
	keypad(win,true);
	nodelay(win,true);
	curs_set(0); //Esconde cursor
	
	//Serpiente
	vec2 cabeza = {0,0};
	vec2 dir = {1,0};

	//Loop de juego
	while(true){
		int tecla_jugada = wgetch(win);
		if (tecla_jugada == KEY_LEFT){
			dir.x = -1;
			dir.y = 0;
		}
		if (tecla_jugada == KEY_RIGHT){
			dir.x = 1;
			dir.y = 0;
		}
		if (tecla_jugada == KEY_UP){
			dir.x = 0;
			dir.y = -1;
		}
		if (tecla_jugada == KEY_DOWN){
			dir.x = 0;
			dir.y = 1;
		}
		if (tecla_jugada == '\e'){
			break;
		}
		
		cabeza.x += dir.x;
		cabeza.y += dir.y;
		
		//Dibujar pantalla
		erase();
		mvaddch(cabeza.y, cabeza.x, 'O');
		usleep(125000);
	}

	endwin();

	return 0;
}
