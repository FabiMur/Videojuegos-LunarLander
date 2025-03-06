#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"

static int estado = PEDIR;

void manejar_tecla(int tecla){
    switch(estado){
        case PEDIR:
            if(tecla == MONEDA){
                printf("Moneda insertada\n");
                inicializarPartida();
                anyadirMoneda();
                estado = MENU;
            }
            break;

        case MENU:
            if(tecla == MONEDA){
                printf("Moneda insertada\n");
                anyadirMoneda();
            }
            else if(tecla == ESPACIO){
                printf("Partida Comenzada\n");
                comenzarPartida();
                estado = JUGANDO;
            }
            break;

        case JUGANDO:
            if(tecla == MONEDA){
                anyadirMoneda();
            }
            else if(tecla == ARRIBA){
                propulsar();
            }
            else if(tecla == IZQUIERDA){
                // girar_izquierda();
            }
            else if(tecla == DERECHA){
                // girar_derecha();
            }
            break;

        default:
            break;
    }
}

void manejar_instante(){
    if(estado == JUGANDO){
        manejar_instante_partida();
    }
}

void pintar_pantalla(HDC hdc){
    if(estado == JUGANDO){
        dibujar_escena(hdc);
    }
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}