#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"

static int estado = PEDIR;
static int estado_teclas[5] = {
    0,
    0,
    0,
    0,
    0
};

void pulsar_tecla(int tecla){
    estado_teclas[tecla] = 1;
}

void levantar_tecla(int tecla){
    estado_teclas[tecla] = 0;
}

void manejar_teclas(){
    switch(estado){
        case PEDIR:
            if(estado_teclas[MONEDA]){
                printf("Moneda insertada\n");
                anyadirMoneda();
                estado_teclas[MONEDA] = 0;
                estado = MENU;
            }
            break;

        case MENU:
            if(estado_teclas[MONEDA]){
                printf("Moneda insertada\n");
                estado_teclas[MONEDA] = 0;
                anyadirMoneda();
            }
            else if(estado_teclas[ESPACIO]){
                printf("Partida Comenzada\n");
                comenzarPartida();
                estado = JUGANDO;
            }
            break;

        case JUGANDO:
            if(estado_teclas[MONEDA]){
                anyadirMoneda();
            }
            if(estado_teclas[ARRIBA]){
                activar_propulsor();
                propulsar();
            } else {
                desactivar_propulsor();
            }

            if(estado_teclas[IZQUIERDA]){
                girar_izquierda();
            }
            if(estado_teclas[DERECHA]){
                girar_derecha();
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

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}

void escalar_escena(float factor_x, float factor_y) {
    escalar_escena_partida(factor_x, factor_y);
}