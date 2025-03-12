#ifndef SUPERFICIE_LUNAR_H
#define SUPERFICIE_LUNAR_H


#include "../code/dibujable.h"

#define ANCHURA_TERRENO 1024
#define ALTURA_TERRENO 324

#define PLATAFORMA_X2 50
#define PLATAFORMA_X3 14
#define PLATAFORMA_X4 10
#define PLATAFORMA_X5 8

extern const struct DibujableConstante Terreno;

struct Plataforma {
    struct DibujableConstante* linea;
    uint8_t bonificador;
    struct Palabra* palabra;
};

#endif // SUPERFICIE_LUNAR_H
