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

void dibujar_escena(HDC hdc);

void rotar_nave(uint8_t direccion);

void manejar_instante_partida();

void inicializarPartida();

void anyadirMoneda();

void comenzarPartida();

void finalizarPartida();

void findeJuego();

#endif // PARTIDA_H