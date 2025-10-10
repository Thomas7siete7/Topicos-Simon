#include "simon.h"

const int tecla[LADO_MATRIZ][LADO_MATRIZ] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

const SDL_Color colores[8] = {
    {255,   0,   0, 255}, // 0: Rojo
    {  0, 255, 255, 255}, // Cian
    {255, 255,   0, 255}, // Amarillo
    {  0, 255,   0, 255}, // Verde
    {255,   0, 255, 255}, // Rosa
    {128,   0, 128, 255},  // Púrpura
    {  0,   0, 255, 255}, // Azul
    {255, 128,   0, 255} // Naranja
};

bool sdl_inicializar(Juego* juego) {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "error al inicializar SDL2: %s\n", SDL_GetError());
        return false;
    }

    if(TTF_Init() < 0) {
        fprintf(stderr, "error al inicializar SDL_ttf: %s\n", TTF_GetError());
        return false;
    }

    int mix_init = Mix_Init(MIX_INIT_MP3);
    if(mix_init == 0) {
        fprintf(stderr, "error al inicializar SDL_mixer: %s\n", Mix_GetError());
        return false;
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        fprintf(stderr, "error al abrir dispositivo de audio: %s\n", Mix_GetError());
        return false;
    }

    Mix_VolumeMusic(VOLUMEN);
    Mix_Volume(-1, VOLUMEN);

    juego->ventana = SDL_CreateWindow(NOMBRE_VENTANA, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_VENTANA, ALTO_VENTANA, 0);
    if(juego->ventana == NULL) {
        fprintf(stderr, "error al crear ventana: %s\n", SDL_GetError());
        return false;
    }

    juego->renderer = SDL_CreateRenderer(juego->ventana, -1, 0);
    if(juego->renderer == NULL) {
        fprintf(stderr, "error al crear el renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *icono = SDL_LoadBMP("d-saturn.bmp");
    if (!icono) {
        fprintf(stderr, "error al cargar icono: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetWindowIcon(juego->ventana, icono);
    SDL_FreeSurface(icono);

    return true;
}

bool cargar_medios(Juego* juego) {
    SDL_Surface* sup_fondo = SDL_LoadBMP("diez.bmp");
    if(!sup_fondo) {
        fprintf(stderr, "error al cargar fondo.bmp: %s\n", SDL_GetError());
        return false;
    }
    juego->fondo = SDL_CreateTextureFromSurface(juego->renderer, sup_fondo);
    if(!juego->fondo) {
        fprintf(stderr, "error al crear textura para fondo: %s\n", SDL_GetError());
        return false;
    }
    SDL_FreeSurface(sup_fondo);

    juego->titulo.fuente = TTF_OpenFont("fnt/GallaeciaForte.ttf", TAM_TITULO);
    if(juego->titulo.fuente == NULL) {
        fprintf(stderr, "error al cargar fuente para titulos: %s\n", TTF_GetError());
        return false;
    }

    juego->normal.fuente = TTF_OpenFont("fnt/ModernDOS.ttf", TAM_NORMAL);
    if(juego->titulo.fuente == NULL) {
        fprintf(stderr, "error al cargar fuente para texto normal: %s\n", TTF_GetError());
        return false;
    }

    juego->chico.fuente = TTF_OpenFont("fnt/ModernDOS.ttf", TAM_CHICO);
    if(juego->titulo.fuente == NULL) {
        fprintf(stderr, "error al cargar fuente para texto chico: %s\n", TTF_GetError());
        return false;
    }

    juego->boton.fuente = TTF_OpenFont("fnt/pepgenius.ttf", TAM_CHICO);
    if(juego->boton.fuente == NULL) {
        fprintf(stderr, "error al cargar fuente para botones: %s\n", TTF_GetError());
        return false;
    }

    juego->musica = Mix_LoadMUS("snd/musica.mp3");
    if(juego->musica == NULL) {
        fprintf(stderr, "error al cargar musica.mp3: %s\n", Mix_GetError());
        return false;
    }

    juego->a_sonido = Mix_LoadWAV("snd/dbz.mp3");
    if(juego->a_sonido == NULL) {
        fprintf(stderr, "error al cargar dbz.mp3: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

void loop_principal(Juego* juego) {
    bool corriendo = true;
    EstadoJuego estado = MENU;
    char jugador[100] = "ingresá tu nombre...";
    char teclas[100] = "entre 3 y 8 teclas";
    char duracion[100] = "máximo 2000ms";
    char timbre[100] = "piano, cello, synth";
    SDL_Rect botonMusica = {970, 670, 10 ,10};

    datosJuego datosPartida;
    datosJuego datosJugador;

    inicializarPartida(&datosPartida, juego);
    inicializarPartida(&datosJugador, juego);

    dataMelodias melodias;
    iniciarMelodias(&melodias);

    bool partidaIniciada = 0;
    int numTeclas = 0;


    while (corriendo) {
        SDL_Event evento;
        while (SDL_PollEvent(&evento))
            {
                switch (evento.type) {
                    case SDL_QUIT:
                        corriendo = false;
                        break;
                    case SDL_KEYDOWN:
                        if (evento.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                            corriendo = false;
                        else if (evento.key.keysym.scancode == SDL_SCANCODE_M) {
                            if (Mix_PausedMusic()) Mix_ResumeMusic();
                            else Mix_PauseMusic();
                        }
                        break;
                    default:
                        break;
                }

                switch (estado) {
                    case MENU: manejoMenu(&evento, &estado, jugador); break;
                    case STATS: manejoEstadisticas(&evento, &estado); break;


                    case CONFIG: manejoConfiguracion(&evento, &estado, teclas, duracion, timbre, &datosPartida.modo); break;
                    case JUEGO:
                        Mix_PauseMusic();
                        if(datosPartida.modo == MODO_MOZART)
                        {
                            if(!partidaIniciada)
                            {
                                CalcularMozart(&melodias, "melodias.txt");
                                numTeclas = melodias.cantMaxNotas;
                            }

                        }else{
                            numTeclas = atoi(teclas);
                        }

                        if(!partidaIniciada){
                            convertirTxtInt(&datosPartida, duracion);
                            partidaIniciada = 1;
                        }

                        manejoJuego(&evento, &estado, &datosPartida, &datosJugador, juego, numTeclas, &datosPartida.duracion, &partidaIniciada, &melodias);
                        break;
                    case CREDITOS: manejoCreditos(&evento, &estado); break;
                    default: break;
                }
            }


        if (estado != JUEGO) {
            SDL_RenderClear(juego->renderer);
            SDL_RenderCopy(juego->renderer, juego->fondo, NULL, NULL);
        }


        switch (estado) {
            case MENU:
                renderizarMenu(juego->renderer, &juego->titulo, &juego->normal, jugador);
                break;
            case CREDITOS:
                renderizarCreditos(juego->renderer, &juego->titulo, &juego->boton, &juego->chico);
                break;
            case STATS:
                renderizarEstadisticas(juego->renderer, &juego->titulo, &juego->boton, &juego->chico);
                break;
            case CONFIG:
                renderizarConfiguracion(juego->renderer, &juego->titulo, &juego->boton, &juego->normal, teclas, duracion, timbre);
                break;
            case JUEGO:
                SDL_RenderClear(juego->renderer);
                SDL_RenderCopy(juego->renderer, juego->fondo, NULL, NULL);
                renderizarJuego(juego->renderer, &juego->boton, seleccionTablero(numTeclas), -1);
                break;
            default:
                break;
        }

        SDL_RenderPresent(juego->renderer);
        SDL_Delay(16);  // ~60 FPS
    }


    free(datosJugador.vec); // REVUSAR BIEN DONDE PONER ESTOS FREE !!
    free(datosPartida.vec);
    finalizar(juego);
}

void manejoMenu(SDL_Event* evento, EstadoJuego* estado, char* jugador) {
    int x_boton = (ANCHO_VENTANA - (ANCHO_BOTON * 3 + PADDING_HORIZONTAL * 2)) / 2;
    int x_campo = (ANCHO_VENTANA - ANCHO_CAMPO) / 2;
    int y_campo = ALTO_VENTANA - (ALTO_BOTON * 3) - PADDING_VERTICAL;
    int y_boton = ALTO_VENTANA - ALTO_BOTON - PADDING_VERTICAL;

    SDL_Rect campoNombre = {x_campo, y_campo, ANCHO_CAMPO, ALTO_BOTON};
    SDL_Rect botonStats = {x_boton, y_boton, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect botonConfig = {x_boton + ANCHO_BOTON + PADDING_HORIZONTAL, y_boton, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect botonCreds = {x_boton + (ANCHO_BOTON + PADDING_HORIZONTAL) * 2, y_boton, ANCHO_BOTON, ALTO_BOTON};
    static SDL_bool escribiendo = SDL_FALSE;

    if(evento->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = evento->button.x;
        int mouseY = evento->button.y;

        if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonConfig)) {
            printf("boton jugar presionado!\n");
            *estado = CONFIG;
        }

        else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonStats)) {
            printf("boton estadisticas presionado!\n");
            *estado = STATS;
        }

        else if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonCreds)) {
            printf("boton creditos presionado!\n");
            *estado = CREDITOS;
        }

        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &campoNombre)) {
            SDL_StartTextInput();
            escribiendo = SDL_TRUE;
            if(strcmp(jugador, "ingresa tu nombre...") == 0)
                jugador[0] = '\0';
            }
            else {
                escribiendo = SDL_FALSE;
                SDL_StopTextInput();
            }
        }

    if(evento->type == SDL_KEYDOWN) {
        if(escribiendo) {
            switch(evento->key.keysym.sym) {
            case SDLK_BACKSPACE:
                if(strlen(jugador) > 0)
                    jugador[strlen(jugador) - 1] = '\0';
                break;
            case SDLK_RETURN:
                escribiendo = SDL_FALSE;
                SDL_StopTextInput();
                break;
            default:
                break;
            }
        }
    }

    if(evento->type == SDL_TEXTINPUT) {
        if(escribiendo)
            if(strlen(jugador) + strlen(evento->text.text) < 99)
                strcat(jugador, evento->text.text);
    }
}

void renderizarMenu(SDL_Renderer* renderer, Texto* titulo, Texto* botones, char* jugador) {
    SDL_Rect campoTitulo;
    renderizarTitulo(renderer, titulo, "Universe Simon!", &campoTitulo);

    int x_boton = (ANCHO_VENTANA - (ANCHO_BOTON * 3 + PADDING_HORIZONTAL * 2)) / 2;
    int x_campo = (ANCHO_VENTANA - ANCHO_CAMPO) / 2;
    int y_campo = ALTO_VENTANA - (ALTO_BOTON * 3) - PADDING_VERTICAL;
    int y_boton = ALTO_VENTANA - ALTO_BOTON - PADDING_VERTICAL;

    SDL_Rect campoNombre = {x_campo, y_campo, ANCHO_CAMPO, ALTO_BOTON};
    SDL_Rect botonStats = {x_boton, y_boton, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect botonConfig = {x_boton + ANCHO_BOTON + PADDING_HORIZONTAL, y_boton, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect botonCreds = {x_boton + (ANCHO_BOTON + PADDING_HORIZONTAL) * 2, y_boton, ANCHO_BOTON, ALTO_BOTON};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &campoNombre);

    SDL_SetRenderDrawColor(renderer, 36, 19, 30, 1); //diez
    SDL_RenderFillRect(renderer, &botonStats);
    SDL_RenderFillRect(renderer, &botonConfig);
    SDL_RenderFillRect(renderer, &botonCreds);

    renderizarTxt(renderer, botones, "ESTADISTICAS", &botonStats, COLOR_TEXTO);
    renderizarTxt(renderer, botones, "JUGAR", &botonConfig, COLOR_TEXTO);
    renderizarTxt(renderer, botones, "CREDITOS", &botonCreds, COLOR_TEXTO);
    renderizarTxt(renderer, botones, jugador, &campoNombre, COLOR_TEXTO);
}

void  manejoEstadisticas(SDL_Event* evento, EstadoJuego* estado) {
    SDL_Rect botonAtras = {20, 20, 10, 10};

    if(evento->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = evento->button.x;
        int mouseY = evento->button.y;

        if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonAtras)) {
            printf("boton atras presionado!\n");
            *estado = MENU;
        }
    }
}

void renderizarEstadisticas(SDL_Renderer* renderer, Texto* titulo, Texto* boton, Texto* info_texto) {
    SDL_Rect statsTitulo;
    SDL_Rect botonAtras = {20, 20, 10, 10};

    renderizarTitulo(renderer, titulo, "ESTADISTICAS", &statsTitulo);
    renderizarAtras(renderer, boton, &botonAtras);

    //falta impresion de archivo estadisticas
}

void manejoCreditos(SDL_Event *evento, EstadoJuego* estado) {
    SDL_Rect botonAtras = {20, 20, 10, 10};

    if(evento->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = evento->button.x;
        int mouseY = evento->button.y;

        if(SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonAtras)) {
            printf("boton atras presionado!\n");
            *estado = MENU;
        }
    }
}

void renderizarCreditos(SDL_Renderer* renderer, Texto* titulo, Texto* boton, Texto* chico) {
    SDL_Rect credsTitulo;
    SDL_Rect grupo = {500, 200, 0, 0};
    SDL_Rect nombreMLF = {500, 300, 0, 0};
    SDL_Rect nombreTMR = {500, 350, 0, 0};
    SDL_Rect nombreMA = {500, 400, 0, 0};
    SDL_Rect copyright = {500, 600, 0, 0};
    SDL_Rect botonAtras = {20, 20, 10, 10};

    renderizarTitulo(renderer, titulo, "CREDITOS", &credsTitulo);
    renderizarAtras(renderer, boton, &botonAtras);
    renderizarTxt(renderer, chico, "GRUPO SATURNO:", &grupo, COLOR_TITULO);
    renderizarTxt(renderer, chico, "Maria Luz Fraschetto", &nombreMLF, COLOR_TITULO);
    renderizarTxt(renderer, chico, "Thomas Marin Rombola", &nombreTMR, COLOR_TITULO);
    renderizarTxt(renderer, chico, "Matias Antunez", &nombreMA, COLOR_TITULO);
    renderizarTxt(renderer, chico, "copyright © Grupo Saturno, Septiembre - Octubre 2025.", &copyright, COLOR_TEXTO);
}


void manejoConfiguracion(SDL_Event* evento, EstadoJuego* estado, char* teclas, char* duracion, char* timbre, TipoModoJuego* modo)
{
    SDL_Rect botonAtras = {20, 20, 10, 10};

    SDL_Rect txtTeclas = {PADDING_HORIZONTAL * 2, 200, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect txtDuracion = {PADDING_HORIZONTAL * 2, txtTeclas.y + PADDING_VERTICAL, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect txtTimbre = {PADDING_HORIZONTAL * 2, txtTeclas.y + (PADDING_VERTICAL * 2), ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect txtModo = {PADDING_HORIZONTAL * 2, txtTeclas.y + (PADDING_VERTICAL * 3), ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect botonJugar = {(ANCHO_VENTANA - (ANCHO_BOTON / 1.5)) / 2,
                           ALTO_VENTANA - ALTO_BOTON - PADDING_VERTICAL,
                           ANCHO_BOTON / 1.5, ALTO_BOTON};

    static CampoActivo campo = NINGUNO;
    SDL_Rect campoTeclas = {PADDING_HORIZONTAL * 3 + txtTeclas.w, txtTeclas.y, ANCHO_CAMPO * 1.35, ALTO_BOTON};
    SDL_Rect campoDuracion = {PADDING_HORIZONTAL * 3 + txtDuracion.w, txtTeclas.y + PADDING_VERTICAL, ANCHO_CAMPO * 1.35, ALTO_BOTON};
    SDL_Rect campoTimbre = {PADDING_HORIZONTAL * 3 + txtTimbre.w, txtTeclas.y + (PADDING_VERTICAL * 2), ANCHO_CAMPO * 1.35, ALTO_BOTON};

    SDL_Rect botonMozart   = {campoTimbre.x, campoTimbre.y + PADDING_VERTICAL, ANCHO_BOTON * 0.98, ALTO_BOTON};
    SDL_Rect botonSchonberg = {botonMozart.x + botonMozart.w + PADDING_HORIZONTAL, botonMozart.y, ANCHO_BOTON * 0.98, ALTO_BOTON};
    SDL_Rect botonUniverse = {campoTimbre.x, botonMozart.y + PADDING_VERTICAL, ANCHO_BOTON * 0.98, ALTO_BOTON};
    SDL_Rect botonDesafio  = {botonUniverse.x + botonUniverse.w + PADDING_HORIZONTAL, botonUniverse.y, ANCHO_BOTON * 0.98, ALTO_BOTON};

    // --- EVENTOS DE MOUSE ---
    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = evento->button.x;
        int mouseY = evento->button.y;

        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonAtras)) {
            printf("botón atrás presionado!\n");
            *estado = MENU;
        }

        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonJugar)) {
            if (configuracionCompleta(teclas, duracion, timbre, modo)) {
                printf("¡Juego iniciado!\n");
                *estado = JUEGO;
            } else {
                printf("Complete todos los campos antes de jugar.\n");
            }
        }

        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &campoTeclas)) {
            campo = TECLAS;
            SDL_StartTextInput();
            if (strcmp(teclas, "entre 3 y 8 teclas") == 0)
                teclas[0] = '\0';
        }

        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &campoDuracion)) {
            campo = DURACION;
            SDL_StartTextInput();
            if (strcmp(duracion, "máximo 2000ms") == 0)
                duracion[0] = '\0';
        }

        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &campoTimbre)) {
            campo = TIMBRE;
            SDL_StartTextInput();
            if (strcmp(timbre, "piano, cello, synth") == 0)
                timbre[0] = '\0';
        }

        // --- SELECCIÓN DE MODO DE JUEGO ---
        else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonSchonberg)) {
            *modo = MODO_SCHONBERG;
            printf("Modo seleccionado: SCHÖNBERG\n");
        } else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonMozart)) {
            *modo = MODO_MOZART;
            printf("Modo seleccionado: MOZART\n");
        } else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonUniverse)) {
            *modo = MODO_UNIVERSE;
            printf("Modo seleccionado: UNIVERSE\n");
        } else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &botonDesafio)) {
            *modo = MODO_DESAFIO;
            printf("Modo seleccionado: DESAFÍO\n");
        }

        else {
            campo = NINGUNO;
            SDL_StopTextInput();
        }
    }

    // --- EVENTOS DE TECLADO ---
    if (evento->type == SDL_KEYDOWN) {
        if (campo != NINGUNO) {
            char* txtCampo = NULL;
            if (campo == TECLAS)
                txtCampo = teclas;
            else if (campo == DURACION)
                txtCampo = duracion;
            else if (campo == TIMBRE)
                txtCampo = timbre;

            switch (evento->key.keysym.sym) {
                case SDLK_BACKSPACE:
                    if (strlen(txtCampo) > 0)
                        txtCampo[strlen(txtCampo) - 1] = '\0';
                    break;
                case SDLK_RETURN:
                    if (validarCampo(campo, txtCampo)) {
                        campo = NINGUNO;
                        SDL_StopTextInput();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // --- INPUT DE TEXTO ---
    if (evento->type == SDL_TEXTINPUT) {
        if (campo != NINGUNO) {
            char* txtCampo = NULL;
            if (campo == TECLAS)
                txtCampo = teclas;
            else if (campo == DURACION)
                txtCampo = duracion;
            else if (campo == TIMBRE)
                txtCampo = timbre;

            if (strlen(txtCampo) + strlen(evento->text.text) < 99) {
                strcat(txtCampo, evento->text.text);
            }
        }
    }
}


bool configuracionCompleta(const char* teclas, const char* duracion, const char* timbre, TipoModoJuego* modo) {
    if(!validarModo(modo))return false;

    if(*modo != MODO_MOZART )
    {
        if (!validarCampo(TECLAS, teclas)) return false;
    }

    if (!validarCampo(DURACION, duracion)) return false;

    //if (!validarCampo(TIMBRE, timbre)) return false;
    return true;
}

bool validarModo(TipoModoJuego* modo)
{
    if(*modo < MODO_SCHONBERG || *modo > MODO_DESAFIO)
    {
        return false;
    }
    return true;
}

bool validarCampo(CampoActivo campo, const char* texto) {

    if (texto == NULL || strlen(texto) == 0)
    return false;


    if (campo == TECLAS) {
        int valor = atoi(texto);
        for (int i = 0; texto[i] != '\0'; i++) {
            if (texto[i] < '0' || texto[i] > '9') {
                printf("ERROR: El campo Teclas solo debe contener numeros.\n");
                return false;
            }
        }

        if (valor < 3 || valor > 8) {
            printf("ERROR: Ingrese un valor entre 3 y 8 teclas.\n");
            return false;
        }

    } else if (campo == DURACION) {
        int valor = atoi(texto);
        for (int i = 0; texto[i] != '\0'; i++) {
            if (texto[i] < '0' || texto[i] > '9') {
                printf("ERROR: El campo Duracion solo debe contener numeros positivos.\n");
                return false;
            }
        }

        if (valor > 2000 || valor < 1) {
            printf("ERROR: La duracion debe ser un numero mayor que cero y menor o igual que 2000ms.\n");
            return false;
        }

    } else if (campo == TIMBRE) {
        const char* opciones[] = {"piano", "cello", "synth"};
        bool esValido = false;

        for (int i = 0; i < 4; i++) {
            if (strcasecmp(texto, opciones[i]) == 0) {
                esValido = true;
                break;
            }
        }

        if (!esValido) {
            printf("ERROR: Timbre no valido. Use: 'piano', 'cello', 'synth'.\n");
            return false;
        }
    }



    return true;
}

void renderizarConfiguracion(SDL_Renderer* renderer, Texto* titulo, Texto* boton, Texto* normal, char* teclas, char* duracion, char* timbre) {
    SDL_Rect configTitulo;
    SDL_Rect botonAtras = {20, 20, 10, 10};

    SDL_Rect txtTeclas = {PADDING_HORIZONTAL * 2, 200, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect txtDuracion = {PADDING_HORIZONTAL * 2, txtTeclas.y + PADDING_VERTICAL, ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect txtTimbre = {PADDING_HORIZONTAL * 2, txtTeclas.y + (PADDING_VERTICAL * 2), ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect txtModo = {PADDING_HORIZONTAL * 2, txtTeclas.y + (PADDING_VERTICAL * 3), ANCHO_BOTON, ALTO_BOTON};
    SDL_Rect botonJugar = {(ANCHO_VENTANA - (ANCHO_BOTON / 1.5)) / 2, ALTO_VENTANA - ALTO_BOTON - PADDING_VERTICAL, ANCHO_BOTON / 1.5, ALTO_BOTON};

    SDL_Rect campoTeclas = {PADDING_HORIZONTAL * 3 + txtTeclas.w, txtTeclas.y, ANCHO_CAMPO * 1.35, ALTO_BOTON};
    SDL_Rect campoDuracion = {PADDING_HORIZONTAL * 3 + txtDuracion.w, txtTeclas.y + PADDING_VERTICAL, ANCHO_CAMPO * 1.35, ALTO_BOTON};
    SDL_Rect campoTimbre = {PADDING_HORIZONTAL * 3 + txtTimbre.w, txtTeclas.y + (PADDING_VERTICAL * 2), ANCHO_CAMPO * 1.35, ALTO_BOTON};

    SDL_Rect botonMozart = {campoTimbre.x, campoTimbre.y + PADDING_VERTICAL, ANCHO_BOTON * 0.98, ALTO_BOTON};
    SDL_Rect botonSchonberg = {botonMozart.x + botonMozart.w + PADDING_HORIZONTAL, botonMozart.y, ANCHO_BOTON * 0.98, ALTO_BOTON};
    SDL_Rect botonUniverse = {campoTimbre.x, botonMozart.y + PADDING_VERTICAL, ANCHO_BOTON * 0.98, ALTO_BOTON};
    SDL_Rect botonDesafio = {botonUniverse.x + botonUniverse.w + PADDING_HORIZONTAL, botonUniverse.y, ANCHO_BOTON * 0.98, ALTO_BOTON};

    SDL_SetRenderDrawColor(renderer, 36, 19, 30, 1);
    SDL_RenderFillRect(renderer, &botonJugar);
    SDL_RenderFillRect(renderer, &botonMozart);
    SDL_RenderFillRect(renderer, &botonSchonberg);
    SDL_RenderFillRect(renderer, &botonUniverse);
    SDL_RenderFillRect(renderer, &botonDesafio);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &campoTeclas);
    SDL_RenderFillRect(renderer, &campoDuracion);
    SDL_RenderFillRect(renderer, &campoTimbre);

    renderizarTitulo(renderer, titulo, "CONFIGURACION", &configTitulo);
    renderizarAtras(renderer, boton, &botonAtras);
    renderizarTxt(renderer, normal, "Número de teclas:", &txtTeclas, COLOR_TITULO);
    renderizarTxt(renderer, normal, teclas, &campoTeclas, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "Duración inicial:", &txtDuracion, COLOR_TITULO);
    renderizarTxt(renderer, normal, duracion, &campoDuracion, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "Timbre de sonido:", &txtTimbre, COLOR_TITULO);
    renderizarTxt(renderer, normal, timbre, &campoTimbre, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "Modo de juego:", &txtModo, COLOR_TITULO);
    renderizarTxt(renderer, normal, "MOZART", &botonMozart, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "SCHÖNBERG", &botonSchonberg, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "UNIVERSE", &botonUniverse, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "DESAFÍO", &botonDesafio, COLOR_TEXTO);
    renderizarTxt(renderer, normal, "¡JUGAR!", &botonJugar, COLOR_TEXTO);
}

const int (*seleccionTablero(const int numTeclas))[LADO_MATRIZ] {
    switch(numTeclas) {
        case 3: return tresTeclas;
        case 4: return cuatroTeclas;
        case 5: return cincoTeclas;
        case 6: return seisTeclas;
        case 7: return sieteTeclas;
        case 8: return ochoTeclas;
        default: return NULL;
    }
}

void inicializarPartida(datosJuego* datos, Juego* juego) {
    srand(time(NULL));

    datos->cantElem = 0;
    datos->lim = TAM_VEC;
    datos->vec = malloc(datos->lim * sizeof(int));
    datos->nivel = 0;
    if(!datos->vec) {
        printf("error al inicializar vectores de partida\n");
        finalizar(juego);
    }
}

int agrandarVec(datosJuego* datos){
    if(datos->vec == NULL)
        return FALLO;

    datos -> lim = 2 * datos -> lim;
    int* temp = realloc(datos -> vec, datos -> lim * sizeof(int));

    if(temp == NULL)
        return FALLO;

    datos -> vec = temp;
    return TODO_OK;
}

int vaciarDatos(datosJuego* datos) {
    if(datos->vec == NULL)
        return FALLO;

    datos->cantElem = 0;
    datos ->nivel = 0;
    return TODO_OK;
}

int calcularToque(SDL_Event* evento, const int (*tablero)[LADO_MATRIZ]) {
    int pixelAncho = LADO_MATRIZ * TAM_PIXEL;
    int pixelAlto  = LADO_MATRIZ * TAM_PIXEL;

    int offsetX = (ANCHO_VENTANA - pixelAncho) / 2;
    int offsetY = (ALTO_VENTANA - pixelAlto) / 2;

    int relativeX = evento->button.x - offsetX;
    int relativeY = evento->button.y - offsetY;

    int celdaX = relativeX / TAM_PIXEL;
    int celdaY = relativeY / TAM_PIXEL;

    if (celdaX >= 0 && celdaX < LADO_MATRIZ && celdaY >= 0 && celdaY < LADO_MATRIZ) {
        return tablero[celdaY][celdaX];
    }

    return -1;
}


int calcularLuces(const int numTeclas) {
    return (rand() % numTeclas) + 11;
}

int guardarLuces(datosJuego* datos, const int luz) {
    if(datos-> vec == NULL)
        return FALLO;

    if(datos->cantElem == datos->lim)
        if(agrandarVec(datos) != TODO_OK)
            return FALLO;

    if(luz < 11)
    {
        return FALLO;
    }

    printf("nivel antes: %i\n", datos ->nivel);

    int* ult = datos->vec + datos->cantElem;
    *ult = luz;
    datos -> cantElem++;
    datos -> nivel++;

    printf("nivel ahora: %i\n", datos ->nivel);

    return TODO_OK;
}

int compararAvance(datosJuego* partida, datosJuego* jugador) {
    for(int i = 0; i < jugador->cantElem; i++)
        if(jugador->vec[i] != partida->vec[i])
            return FALLO;

    return TODO_OK;
}

void CalcularMozart(dataMelodias* melodias, char* nombre)
{
    char path[256];
    int n = snprintf(path, sizeof(path), "Melodias/%s", nombre);
    if (n < 0 || n >= (int)sizeof(path)) {
        fprintf(stderr, "Error: ruta demasiado larga\n");
        return;
    }
    FILE* arch = fopen(path, "rt");
    if (!arch) {
        fprintf(stderr, "Error: archivo no encontrado\n");
        return;
    }

    char linea[256];
    if (!fgets(linea, sizeof(linea), arch)) {
        fprintf(stderr, "Error: no se pudo leer la línea\n");
        fclose(arch);
        return;
    }


    int i = 0;

    while (linea[i] && (linea[i] < '0' || linea[i] > '9'))
        i++;

    if (linea[i] == '\0') {
        fprintf(stderr, "Línea inválida: no se encontraron números.\n");
        fclose(arch);
        return;
    }

    melodias->cantMaxNotas = linea[i] - '0';
    printf("cantMaxNotas: %i\n", melodias->cantMaxNotas);

    i++;

    while (linea[i] != '\0' && !(linea[i] == '0' && linea[i + 1] == '0')) {

        if (linea[i] > '0' && linea[i] <= '9') {
            if (melodias->cantNotas >= melodias->max) {
                if (agrandarMelodias(melodias) == FALLO) {
                    fprintf(stderr, "No se pudo agrandar melodia\n");
                    liberarMelodias(melodias);
                    fclose(arch);
                    return;
                }
            }

            *(melodias->melodia + melodias->cantNotas) = 10 + (linea[i] - '0');
            melodias->cantNotas++;
        }
        i++;
    }


    if (linea[i] == '0' && linea[i + 1] == '0')
        i += 2;


    int k = 0;
    while (linea[i] && linea[i] != '\n' && k < 49) {
        melodias->nombre[k++] = linea[i++];
    }
    melodias->nombre[k] = '\0';

    fclose(arch);
}


void manejoJuego(SDL_Event* evento, EstadoJuego* estado, datosJuego* partida, datosJuego* jugador, Juego* juego, int numTeclas, int* duracion, bool* partidaIniciada, dataMelodias* melodias) {
    static bool iniciado = false;


    if(!iniciado)
    {
        if(partida -> modo == MODO_SCHONBERG)
        {
            guardarLuces(partida, calcularLuces(numTeclas));
        }
        else if(partida -> modo == MODO_MOZART)
        {
            guardarLuces(partida, *(melodias -> melodia + partida -> nivel));
        }
    }
    if(partida->modo == MODO_MOZART)
    {
        printf("el numero de teclas antes era de: %i\n", numTeclas);
        numTeclas = melodias->cantMaxNotas;
        printf("el numero de teclas ahora era de: %i\n", numTeclas);
    }
    const int (*tablero)[LADO_MATRIZ] = seleccionTablero(numTeclas);
    if (!tablero) return;

    if (!iniciado) {
        pintarIluminados(juego, numTeclas, partida, tablero, *duracion);

        iniciado = true;
    }
    printf("La melodia es:\n");
    for(int i = 0; i<melodias->cantNotas; i++)
    {
        printf("%i", melodias->melodia[i]);
    }
    printf("fin melodia\n");

    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        int clic = calcularToque(evento, tablero);
        if (clic > 10 && clic < 19) {
            printf("jugador:\n");
            guardarLuces(jugador, clic);
            renderizarJuego(juego->renderer, &juego->boton, tablero, clic);
            SDL_RenderPresent(juego->renderer);
            SDL_Delay(150);

            int resultado = compararAvance(partida, jugador);
            if (resultado == FALLO) {
                SDL_Rect rectGO = {ANCHO_VENTANA / 2 - 200, ALTO_VENTANA / 2 - 100, 400, 200};
                renderizarTxt(juego->renderer, &juego->titulo, "GAME OVER", &rectGO, COLOR_TITULO);
                SDL_RenderPresent(juego->renderer);
                SDL_Delay(1000);
                vaciarDatos(partida);
                vaciarDatos(jugador);
                if(partida -> modo == MODO_MOZART)
                {
                    liberarMelodias(melodias);
                    iniciarMelodias(melodias);
                }

                iniciado = false;
                *partidaIniciada = 0;
                *estado = CONFIG;
                return;
            }


            if(resultado == TODO_OK && jugador -> cantElem == partida -> cantElem) {
                vaciarDatos(jugador);
                *duracion *= DISMINUIRPORCENTAJE;

                if(partida -> modo == MODO_SCHONBERG)
                {
                    guardarLuces(partida, calcularLuces(numTeclas));
                }
                else if(partida -> modo == MODO_MOZART)
                {

                    printf("melodias:\n");
                    for(int i = 0; i<melodias->cantNotas; i++)
                    {
                        printf("%i", melodias->melodia[i]);
                    }
                    guardarLuces(partida, melodias -> melodia[partida -> nivel]);
                }
                pintarIluminados(juego, numTeclas, partida, tablero, *duracion);
            }
        }
    }


    while (SDL_PollEvent(evento)) {}
    SDL_FlushEvents(SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP);
}


void renderizarJuego(SDL_Renderer* renderer, Texto* boton, const int (*tablero)[LADO_MATRIZ], int iluminar) {
    SDL_Rect botonAtras = {20, 20, 10, 10};
    renderizarAtras(renderer, boton, &botonAtras);

    int ladoTablero = LADO_MATRIZ * TAM_PIXEL;
    int centroX = (ANCHO_VENTANA - ladoTablero) / 2;
    int centroY = (ALTO_VENTANA - ladoTablero) / 2;

    for (int y = 0; y < LADO_MATRIZ; y++) {
        for (int x = 0; x < LADO_MATRIZ; x++) {
            int puntoX = centroX + x * TAM_PIXEL;
            int puntoY = centroY + y * TAM_PIXEL;

            SDL_Rect rect = {puntoX, puntoY, TAM_PIXEL, TAM_PIXEL};
            int valor = tablero[y][x];

            if (valor == iluminar) {
                switch (valor) {
                    case 11: SDL_SetRenderDrawColor(renderer, 247, 77, 77, 255); break;
                    case 12: SDL_SetRenderDrawColor(renderer, 102, 218, 242, 255); break;
                    case 13: SDL_SetRenderDrawColor(renderer, 227, 232, 93, 255); break;
                    case 14: SDL_SetRenderDrawColor(renderer, 101, 240, 135, 255); break;
                    case 15: SDL_SetRenderDrawColor(renderer, 235, 96, 207, 255); break;
                    case 16: SDL_SetRenderDrawColor(renderer, 211, 89, 235, 255); break;
                    case 17: SDL_SetRenderDrawColor(renderer, 84, 98, 209, 255); break;
                    case 18: SDL_SetRenderDrawColor(renderer, 219, 132, 88, 255); break;
                    default: break;
                }
            } else {
                switch (valor) {
                    case 19: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
                    case 20: continue;
                    case 11: SDL_SetRenderDrawColor(renderer, 162, 82, 82, 255); break;
                    case 12: SDL_SetRenderDrawColor(renderer, 82, 148, 162, 255); break;
                    case 13: SDL_SetRenderDrawColor(renderer, 159, 162, 82, 255); break;
                    case 14: SDL_SetRenderDrawColor(renderer, 82, 162, 102, 255); break;
                    case 15: SDL_SetRenderDrawColor(renderer, 162, 82, 146, 255); break;
                    case 16: SDL_SetRenderDrawColor(renderer, 149, 82, 162, 255); break;
                    case 17: SDL_SetRenderDrawColor(renderer, 82, 91, 162, 255); break;
                    case 18: SDL_SetRenderDrawColor(renderer, 54, 42, 45, 255); break;
                    default: break;
                }
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


void pintarIluminados(Juego* juego, const int numTeclas, datosJuego* partida, const int (*tablero)[LADO_MATRIZ], int duracion) {
    int luz;

    if(partida -> modo == MODO_MOZART && partida -> nivel == partida -> lim)
    {
        printf("La performance ha sido completada.");
        return;
    }
    for (int i = 0; i < partida->nivel; i++) {
        luz = partida->vec[i];

        renderizarJuego(juego->renderer, &juego->boton, tablero, -1);
        SDL_RenderPresent(juego->renderer);
        SDL_Delay(duracion);

        renderizarJuego(juego->renderer, &juego->boton, tablero, luz);
        SDL_RenderPresent(juego->renderer);
        SDL_Delay(duracion);
    }
}


void renderizarAtras(SDL_Renderer* renderer, Texto* info_boton, SDL_Rect* rect) {
    SDL_Surface* sup = TTF_RenderText_Blended(info_boton->fuente, "J", COLOR_TITULO);
    if(!sup) {
        fprintf(stderr, "error al crear superficie para el boton atras: %s\n", TTF_GetError());
        return;
    }

    info_boton->img = SDL_CreateTextureFromSurface(renderer, sup);
    if(!info_boton->img) {
        fprintf(stderr, "error al crear textura para boton atras: %s\n", SDL_GetError());
        return;
    }

    info_boton->rect.x = rect->x + (rect->w - sup->w) / 2;
    info_boton->rect.y = rect->y + (rect->h - sup->h) / 2;
    info_boton->rect.w = sup->w;
    info_boton->rect.h = sup->h;

    SDL_RenderCopy(renderer, info_boton->img, NULL, &info_boton->rect);
    SDL_FreeSurface(sup);
    SDL_DestroyTexture(info_boton->img);
}

void renderizarTxt(SDL_Renderer* renderer, Texto* texto, const char* mensaje, SDL_Rect* rect, SDL_Color color) {
    if(mensaje == NULL || strlen(mensaje) == 0)
        return;

    SDL_Surface* sup = TTF_RenderText_Blended(texto->fuente, mensaje, color);
    if(!sup) {
        fprintf(stderr, "error al crear superficie para el texto: %s\n", TTF_GetError());
        return;
    }

    texto->img = SDL_CreateTextureFromSurface(renderer, sup);
    if(!texto->img) {
        fprintf(stderr, "error al crear textura para texto: %s\n", SDL_GetError());
        return;
    }

    texto->rect.x = rect->x + (rect->w - sup->w) / 2;
    texto->rect.y = rect->y + (rect->h - sup->h) / 2;
    texto->rect.w = sup->w;
    texto->rect.h = sup->h;

    SDL_RenderCopy(renderer, texto->img, NULL, &texto->rect);
    SDL_FreeSurface(sup);
    SDL_DestroyTexture(texto->img);
}

void renderizarTitulo(SDL_Renderer* renderer, Texto* texto, const char* mensaje, SDL_Rect* rect) {
    if(mensaje == NULL || strlen(mensaje) == 0)
        return;

    SDL_Surface* sup = TTF_RenderText_Blended(texto->fuente, mensaje, COLOR_TITULO);
    if(!sup) {
        fprintf(stderr, "error al crear superficie para el texto: %s\n", TTF_GetError());
        return;
    }

    texto->img = SDL_CreateTextureFromSurface(renderer, sup);
    if(!texto->img) {
        fprintf(stderr, "error al crear textura para texto: %s\n", SDL_GetError());
        return;
    }

    texto->rect.w = sup->w;
    texto->rect.h = sup->h;
    texto->rect.x = (ANCHO_VENTANA - texto->rect.w) / 2;
    texto->rect.y = PADDING_TITULO;

    SDL_RenderCopy(renderer, texto->img, NULL, &texto->rect);
    SDL_FreeSurface(sup);
    SDL_DestroyTexture(texto->img);
}

void finalizar(Juego* juego) {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    Mix_FreeMusic(juego->musica);
    Mix_FreeChunk(juego->a_sonido);
    SDL_DestroyTexture(juego->titulo.img);
    TTF_CloseFont(juego->titulo.fuente);
    SDL_DestroyTexture(juego->fondo);
    SDL_DestroyRenderer(juego->renderer);
    SDL_DestroyWindow(juego->ventana);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
    free(juego);
    exit(EXIT_SUCCESS);
}


void convertirTxtInt(datosJuego* datos, char* text){
    datos -> duracion = atoi(text);
}

int iniciarMelodias(dataMelodias* mel)
{
    mel -> melodia = malloc(256 * sizeof(int));
    mel -> nombre = malloc(50 * sizeof(char));
    mel -> nombre[0] = '\0';
    mel -> cantNotas = 0;
    mel -> max = 256;
    mel -> cantMaxNotas = 0;

    if(mel -> melodia == NULL || mel -> nombre == NULL)
    {
        return FALLO;
    }

    mel->nombre[0] = '\0';
    return TODO_OK;
}

int agrandarMelodias(dataMelodias* mel)
{
    if(mel -> melodia == NULL)
    {
        return FALLO;
    }

    int nuevoMax = mel->max + mel->max/2;
    if (nuevoMax <= 0) nuevoMax = mel->max + 1;

    int* temp = realloc(mel -> melodia, nuevoMax * sizeof(int));
    if(temp == NULL)
    {
        return FALLO;
    }

    mel -> max = nuevoMax;
    mel -> melodia = temp;

    return TODO_OK;
}

int iniciarVecMelodias(vecMelodias* mel)
{
    mel -> vec = malloc(256 * sizeof(dataMelodias));
    mel -> cantElem = 0;
    mel -> max = 256;

    if(mel -> vec == NULL)
    {
        return FALLO;
    }

    return TODO_OK;
}

int agrandarVecMelodias(vecMelodias* mel)
{
    if(mel -> vec == NULL)
    {
        return FALLO;
    }

    int nuevoMax = mel->max + mel->max/2;
    if (nuevoMax <= 0) nuevoMax = mel->max + 1;

    dataMelodias* temp = realloc(mel -> vec, nuevoMax * sizeof(dataMelodias));
    if(temp == NULL)
    {
        return FALLO;
    }

    mel -> max = nuevoMax;   // actualizar capacidad, NO cantElem
    mel -> vec = temp;

    return TODO_OK;
}

int pushMelodia(vecMelodias* vm, const dataMelodias* src)
{
    if (vm->cantElem == vm->max) {
        if (agrandarVecMelodias(vm) == FALLO) return FALLO;
    }

    dataMelodias* dst = &vm->vec[vm->cantElem];

    dst->cantNotas    = src->cantNotas;
    dst->cantMaxNotas = src->cantMaxNotas;
    dst->max          = (src->cantNotas > 0) ? src->cantNotas : 1;

    dst->melodia = malloc(dst->max * sizeof(int));
    if (!dst->melodia) return FALLO;
    memcpy(dst->melodia, src->melodia, src->cantNotas * sizeof(int));

    size_t nlen = strlen(src->nombre);
    dst->nombre = malloc(nlen + 1);
    if (!dst->nombre) { free(dst->melodia); return FALLO; }
    memcpy(dst->nombre, src->nombre, nlen + 1);

    vm->cantElem++;
    return TODO_OK;
}

void liberarMelodias(dataMelodias* m)
{
    free(m->melodia);
    free(m->nombre);

    m->cantNotas = 0;
    m->cantMaxNotas = 0;
}

void liberarVecMelodias(vecMelodias* vm)
{
    if (!vm || !vm->vec) return;
    for (int i = 0; i < vm->cantElem; i++) {
        liberarMelodias(&vm->vec[i]);
    }
    free(vm->vec);
    vm->vec = NULL;
    vm->cantElem = 0;
    vm->max = 0;
}

