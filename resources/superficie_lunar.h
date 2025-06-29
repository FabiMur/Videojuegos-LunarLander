#ifndef SUPERFICIE_LUNAR_H
#define SUPERFICIE_LUNAR_H


#include "../code/dibujable.h"

#define ANCHURA_TERRENO 1024
#define ALTURA_TERRENO 324

#define PLATAFORMA_X2 58
#define PLATAFORMA_X3 21
#define PLATAFORMA_X4 17
#define PLATAFORMA_X5 15

// Declaración anticipada de los structs que se referencian
struct DibujableConstante;
struct Palabra;

/**
 * @brief Estructura que representa una plataforma en el juego
 * 
 * @param linea Puntero a la estructura `Dibujable` que representa la línea de la plataforma.
 * @param bonificador Valor de bonificación asociado a la plataforma.
 * @param texto Puntero a la estructura `Texto` que representa el texto asociado a la plataforma.
 */
struct Plataforma {
    struct Dibujable* linea;
    uint8_t bonificador;
    struct Texto* texto;
};

extern const struct DibujableConstante Terreno;

#endif // SUPERFICIE_LUNAR_H