/**
 * @file fisicas.h
 * @brief Contiene las funciones necesarias para manejar las fisicas del juego
 */

#ifndef FISICAS_H
#define FISICAS_H

#include "dibujable.h"

#define intervalo_fisicas_ms 100
#define gravedad_m_ms 0.001625

enum fisicas {
    ACTIVADAS,
    DESACTIVADAS
};

struct objetoFisico {
    struct Dibujable* objeto;
    float velocidad[2];
    float aceleracion[2];
    int masa;
};

static int fisicas = DESACTIVADAS;

void destruirObjetoFisico(struct objetoFisico* objeto);

void calcularFisicas(struct objetoFisico* objeto);

void acelerar();

void girar_izquierda();

void girar_derecha();

#endif // FISICAS_H