/**
 * @file partida.h
 * @brief Contiene las funciones necesarias para manejar la partida y su estado
 */

#ifndef PARTIDA_H
#define PARTIDA_H

#include "lunar_lander.h"
#include "fisicas.h"
#include "transformaciones.h"
#include "../resources/nave.h"
#include "../resources/caracteres.h"

/**
 * @brief Enumeración para el estado de las fisicas
 * 
 * DESACTIVADAS: 0
 * ACTIVADAS: 1
 */
typedef enum {
    DESACTIVADAS,
    ACTIVADAS
} EstadoFisicas;

extern int combustible;

extern uint8_t fisicas;

void dibujar_escena(HDC hdc);

void rotar_nave(uint8_t direccion);

void manejar_instante_partida();

void inicializarPartida();

void anyadirMoneda();

void comenzarPartida();

void finalizarPartida();

void findeJuego();

void escalar_escena_partida(float factor_x, float factor_y);

#endif // PARTIDA_H