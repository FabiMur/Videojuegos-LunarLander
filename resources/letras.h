#ifndef LETRAS_H
#define LETRAS_H

#include "../code/dibujable.h"

#define ALTURA_MAX 16
#define ANCHURA_MAX 10 
#define CENTRO_CONST_LETRA {5, 8}

const struct DibujableConstante Letra_A_Base = {
    CENTRO_CONST_LETRA,
    (struct Punto[]){{0, 16}, {0, 5}, {5, 0}, {10, 5}, {10, 15}, {0, 9}, {10, 9}},
    (struct UnionAristas[]){{0, 1}, {1, 2}, {2, 3}, {3, 4}, {5, 6}},
    7,
    5
};

const struct DibujableConstante Letra_B_Base = {
    CENTRO_CONST_LETRA,
    (struct Punto[]){{0, 16}, {0, 0}, {7, 0}, {10, 2}, {10, 6}, {7, 8}, {10, 10}, {10, 14}, {7, 16}, {0, 8}},
    (struct UnionAristas[]){{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 0}, {9, 5}},
    10,
    10
};


const struct DibujableConstante Letra_I_Base = {
    CENTRO_CONST_LETRA,
    (struct Punto[]){{0, 0}, {ANCHURA_MAX, 0}, {5, 0}, {0, ALTURA_MAX}, {5, ALTURA_MAX}, {ANCHURA_MAX, ALTURA_MAX}},
    (struct UnionAristas[]){{0, 1}, {2, 4}, {3, 5}},
    6,
    3
};





#endif // LETRAS_H