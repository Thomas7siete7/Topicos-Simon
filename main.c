#include "simon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int iniciarMelodias(dataMelodias* mel);
int agrandarMelodias(dataMelodias* mel);
int iniciarVecMelodias(vecMelodias* mel);
int agrandarVecMelodias(vecMelodias* mel);


int pushMelodia(vecMelodias* vm, const dataMelodias* src);


void liberarMelodias(dataMelodias* m);
void liberarVecMelodias(vecMelodias* vm);

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

/*
    dataMelodias melodias;
    vecMelodias vecMel;

    FILE* arch = fopen("Melodias/melodias.txt", "rt");
    if(arch == NULL)
    {
        printf("error, archivo no encontrado");
        return 1;
    }else{
        printf("cinco palabras, crack.\n");
    }

    iniciarMelodias(&melodias);
    iniciarVecMelodias(&vecMel);

    char linea[256];
    int i;
    while(fgets(linea, sizeof(linea), arch))
    {
        i = 1;
        fprintf(stderr, "%s\n", linea);

        melodias.cantNotas = 0;
        melodias.nombre[0] = '\0';

        melodias.cantMaxNotas = linea[i - 1] - '0';

        printf("\nlinea[i] (%c) != '\\n' && linea[i] (%c) != '0' && linea[i+1] (%c) != 0\n", linea[i], linea[i], linea[i+1] );


        while (linea[i] != '\0' && !(linea[i] == '0' && linea[i+1] == '0')) {

            printf("El caracter es: %c y el numero: %i\n", linea[i], linea[i]-'0');
            if(linea[i] != '0')
            {

                if (melodias.cantNotas >= melodias.max) {
                    if (agrandarMelodias(&melodias) == FALLO) {
                        fprintf(stderr, "No se pudo agrandar melodia\n");
                        fclose(arch);
                        liberarVecMelodias(&vecMel);
                        liberarMelodias(&melodias);
                        return 1;
                    }
                }

                *(melodias.melodia + melodias.cantNotas) = linea[i]-'0';
                melodias.cantNotas++;
            }
            i++;
        }


        if(linea[i] == '0' && linea[i + 1] == '0'){
            i += 2;
        }


        int j = 0;
        while(linea[i] != '\n' && linea[i] != '\0')
        {
            if (j < 49) {
                *(melodias.nombre + j) = linea[i];
                j++;
            }
            i++;
        }
        *(melodias.nombre + j) = '\0';


        if (pushMelodia(&vecMel, &melodias) == FALLO) {
            fprintf(stderr, "No pude guardar la melodia en el vector\n");
            fclose(arch);
            liberarVecMelodias(&vecMel);
            liberarMelodias(&melodias);
            return 1;
        }


        printf("Guardada: %s (%d notas)\n", vecMel.vec[vecMel.cantElem-1].nombre, vecMel.vec[vecMel.cantElem-1].cantNotas);


    }

    fclose(arch);

    //
    /*
    for (int k = 0; k < vecMel.cantElem; k++) {
        printf("Mel %d: %s\n", k, vecMel.vec[k].nombre);
        for (int r = 0; r < vecMel.vec[k].cantNotas; r++) {
            printf(" %d", vecMel.vec[k].melodia[r]);
        }
        printf("\n");
    }



    liberarVecMelodias(&vecMel);
    liberarMelodias(&melodias);*/

    return 0;
}

