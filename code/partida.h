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

// Extern para poder acceder en otros sitios a las coordenadas de la nave
extern struct objetoFisico* nave;

extern int combustible;

extern uint16_t fisicas;

void dibujar_escena(HDC hdc);

void dibujarHUD(HDC hdc);

void rotar_nave(uint16_t direccion);

void manejar_instante_partida();

void inicializarPartida();

void anyadirMoneda();

void comenzarPartida();

void finalizarPartida();

void findeJuego();

void escalar_escena_partida(float factor_x, float factor_y);

void gestionar_colisiones();

void gestionar_colisiones_asteroides();


#endif // PARTIDA_H