#ifndef ASTEROIDES_H
#define ASTEROIDES_H

#include "../code/dibujable.h"

#define CENTRO_ASTEROIDE_PEQUENO {ALTURA_ASTEROIDE_PEQUENO/2, ANCHURA_ASTEROIDE_PEQUENO/2}
#define CENTRO_ASTEROIDE_MEDIANO {ALTURA_ASTEROIDE_MEDIANO/2, ANCHURA_ASTEROIDE_MEDIANO/2}
#define CENTRO_ASTEROIDE_GRANDE {ALTURA_ASTEROIDE_GRANDE/2, ANCHURA_ASTEROIDE_GRANDE/2}


#define ANCHURA_ASTEROIDE_GRANDE 54
#define ALTURA_ASTEROIDE_GRANDE 54
#define ANCHURA_ASTEROIDE_MEDIANO 31
#define ALTURA_ASTEROIDE_MEDIANO 31
#define ANCHURA_ASTEROIDE_PEQUENO 18
#define ALTURA_ASTEROIDE_PEQUENO 18

#define MAX_ASTEROIDES 10

enum TipoAsteroide {
    ASTEROIDE_PEQUENO,
    ASTEROIDE_MEDIANO,
    ASTEROIDE_GRANDE
};

struct Asteroide {
    struct Dibujable* objeto;
    enum TipoAsteroide tipo;
};

extern const struct DibujableConstante Asteroide_grande;
extern const struct DibujableConstante Asteroide_mediano;
extern const struct DibujableConstante Asteroide_pequeno;

enum TipoAsteroide randomAsteroideType();

void dibujarAsteroide(HDC hdc, struct Asteroide* asteroide);

void dibujarAsteroides(HDC hdc, struct Asteroide* asteroides, uint8_t numAsteroides);

void colocarAsteroide(struct Asteroide* asteroide);

struct Asteroide* generarAsteroides(const struct Dibujable* terreno, uint8_t* numAsteroides);

struct Punto* randomPoint(uint32_t min_x, uint32_t max_x, uint32_t min_y, uint32_t max_y);


#endif // ASTEROIDES_H