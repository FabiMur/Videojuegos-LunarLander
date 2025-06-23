#include "lunar_lander.h"
#include "partida.h"
#include "fisicas.h"
#include "opciones.h"

extern int tiempo;
extern int tiempo_ms;

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
            if(obtenerValorFlag(FLAG_AI)) {
                anyadirMoneda();
                anyadirMoneda();
                anyadirMoneda();
                comenzarPartida();
                estado = JUGANDO;
                break;
            }
            if(estado_teclas[MONEDA]){
                printf("Moneda insertada\n");
                anyadirMoneda();
                estado_teclas[MONEDA] = 0;
                estado = MENU;
            }
            break;

        case MENU:
            if(obtenerValorFlag(FLAG_AI)) {
                comenzarPartida();
                estado = JUGANDO;
                break;
            }
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
            if(!obtenerValorFlag(FLAG_AI)) {
                if(estado_teclas[ARRIBA] && combustible >= combustible_motor){
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
            }
            break;

        default:
            break;
    }
}

void manejar_instante(){
    if(estado == JUGANDO){
        tiempo_ms += intervalo_fisicas_ms;
        if(tiempo_ms >= 1000){
            tiempo += tiempo_ms / 1000;
            tiempo_ms %= 1000;
        }
        manejar_instante_partida();
    }
}

void cambiar_estado(int nuevo_estado){
    estado = nuevo_estado;
}

void escalar_escena(float factor_x, float factor_y) {
    escalar_escena_partida(factor_x, factor_y);
}