/**
 * @file lunar_lander.h
 * @brief Contiene el codigo principal del juego y su maquina de estados
 * 
 * 
 */
#ifndef LUNAR_LANDER_H
#define LUNAR_LANDER_H

#include "partida.h"

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

void manejar_tecla(int tecla);

void manejar_instante();

void cambiar_estado(int nuevo_estado);

#endif // LUNAR_LANDER_H