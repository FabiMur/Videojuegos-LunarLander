/**
 * @file fisicas.h
 * @brief Contiene las funciones necesarias para manejar las fisicas del juego
 */

#ifndef FISICAS_H
#define FISICAS_H

#include "dibujable.h"
#include "transformaciones.h"
#include "../resources/asteroides.h"


#define intervalo_fisicas_ms 16
#define pixels_por_metro 32
#define gravedad_m_ms -0.0005
#define desaceleracion_horizontal 0.002
#define propulsor_m_ms 0.0020
#define combustible_motor 3

struct objetoFisico {
    struct Dibujable* objeto;
    float velocidad[2];
    float aceleracion[2];
    int masa;
	int16_t rotacion;
};

void destruirObjetoFisico(struct objetoFisico* objeto);

void calcularFisicas(struct objetoFisico* objeto);

void calcualarFisicasAsteroides(struct Asteroide* asteroides, uint8_t numAsteroides);

void calcularFisicasTrozosNave(struct objetoFisico* trozos[], uint8_t num_trozos);

void propulsar();

void activar_propulsor();

void desactivar_propulsor();

void girar_izquierda();

void girar_derecha();

uint16_t obtener_propulsor();

#endif // FISICAS_H
