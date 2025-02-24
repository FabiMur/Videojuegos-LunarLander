/**
 * @file fisicas.h
 * @brief Contiene las funciones necesarias para manejar las fisicas del juego
 */

#ifndef FISICAS_H
#define FISICAS_H

enum fisicas {
    ACTIVADAS,
    DESACTIVADAS
};

static int fisicas = DESACTIVADAS;

void acelerar();

void girar_izquierda();

void girar_derecha();

#endif // FISICAS_H