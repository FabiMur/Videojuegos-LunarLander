/**
 * @file lunar_lander.h
 * @brief Contiene el codigo principal del juego y su maquina de estados
 * 
 * 
 */
#ifndef LUNAR_LANDER_H
#define LUNAR_LANDER_H

#include "partida.h"
#include <wtypes.h>

enum tecla {
    ARRIBA,
    IZQUIERDA,
    DERECHA,
    ESPACIO,
    MONEDA
};

enum estado {
    PEDIR,
    MENU,
    JUGANDO
};

void pulsar_tecla(int tecla);

void levantar_tecla(int tecla);

void manejar_teclas();

void manejar_instante();

void cambiar_estado(int nuevo_estado);

void escalar_escena(float factor_x, float factor_y);

int obtener_estado(void);

#endif // LUNAR_LANDER_H