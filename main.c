#include "simon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char *argv[]) {
    Juego* juego = calloc(1, sizeof(Juego));

    if(!sdl_inicializar(juego)) {
        finalizar(juego);
    }

    if(!cargar_medios(juego)) {
        finalizar(juego);
    }

    if(Mix_PlayMusic(juego->musica, -1)) {
        fprintf(stderr, "error al reproducir musica: %s\n", Mix_GetError());
    }

    loop_principal(juego);

    finalizar(juego);

    return 0;
}

