/**
 * @file fisicas.h
 * @brief Contiene las funciones necesarias para manejar las fisicas del juego
 */

#ifndef FISICAS_H
#define FISICAS_H

#include "dibujable.h"
#include "transformaciones.h"

#define intervalo_fisicas_ms 64
#define gravedad_m_ms -0.000001

struct objetoFisico {
    struct Dibujable* objeto;
    float velocidad[2];
    float aceleracion[2];
    int masa;
};

void destruirObjetoFisico(struct objetoFisico* objeto);

void calcularFisicas(struct objetoFisico* objeto);
/*
void propulsar();

void girar_izquierda();

void girar_derecha();
*/

#endif // FISICAS_H