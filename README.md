# 🐍 Snake en C con ncurses

Este proyecto es una práctica de programación en lenguaje **C**, centrada en la exploración de nuevas librerías para construir interfaces de usuario en la terminal. Se trata de una implementación básica (y en desarrollo) del clásico juego **Snake**, utilizando la biblioteca `ncurses`.

---

## 📚 Objetivo del Proyecto

- Practicar la programación en C mediante un proyecto entretenido y visual.
- Aprender y aplicar el uso de la librería **ncurses** para construir interfaces interactivas en la terminal.
- Trabajar con estructuras de datos simples (`struct`) para representar coordenadas.
- Reforzar conceptos como control de flujo, manejo de entrada del usuario y estructuras de control.

---

## 🧩 Diseño del Programa

El diseño del juego está basado en algunos elementos clave:

- **Estructura `vec2`**: representa coordenadas (x, y) para objetos como la serpiente y la fruta.
- **Pantalla**: dimensiones fijas de 40×20 caracteres. El juego se renderiza en una terminal con caracteres simples.
- **Serpiente**:
  - Comienza con una cabeza (`vec2 cabeza`) y un arreglo de segmentos (`vec2 cuerpo[256]`).
  - Se desplaza por el tablero en una dirección dada por `vec2 dir`, que cambia con la entrada del jugador.
  - El cuerpo sigue el movimiento de la cabeza con una mecánica simple de desplazamiento de arreglos.

- **Fruta**:
  - Se genera en una posición aleatoria dentro del tablero.
  - Al ser "comida" por la serpiente, incrementa el puntaje y genera una nueva posición aleatoria.

- **Controles del jugador**:
  - Flechas del teclado (`KEY_UP`, `KEY_DOWN`, `KEY_LEFT`, `KEY_RIGHT`) para mover la serpiente.
  - La tecla `ESC` (`'\e'`) finaliza el juego.

---

