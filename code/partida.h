/**
 * @file partida.h
 * @brief Contiene las funciones necesarias para manejar la partida y su estado
 */

#ifndef PARTIDA_H
#define PARTIDA_H

#include <wtypes.h>
#include "lunar_lander.h"
#include "transformaciones.h"
#include "../resources/nave.h"
#include "../resources/letras.h"

static struct Dibujable* nave;
static int combustible = 0;

void dibujarEscena(HDC hdc);

void inicializarPartida();

void anyadirMoneda();

void comenzarPartida();

void finalizarPartida();

void findeJuego();

#endif // PARTIDA_H