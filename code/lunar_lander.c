#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"

static int estado = PEDIR;

void manejar_tecla(int tecla){
    switch(estado){
        case PEDIR:
            if(tecla == MONEDA){
                inicializarPartida();
                anyadirMoneda();
                estado = MENU;
            }
            break;

        case MENU:
            if(tecla == MONEDA){
                anyadirMoneda();
            }
            else if(tecla == ESPACIO){
                comenzarPartida();
                estado = JUGANDO;
            }
            break;

        case JUGANDO:
            if(tecla == MONEDA){
                anyadirMoneda();
            }
            else if(tecla == ARRIBA){
                acelerar();
            }
            else if(tecla == IZQUIERDA){
                girar_izquierda();
            }
            else if(tecla == DERECHA){
                girar_derecha();
            }
            break;

        default:
            break;
    }
}

void manejar_instante(){
    if(estado == JUGANDO){
        // Actualizar fisicas
    }
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}