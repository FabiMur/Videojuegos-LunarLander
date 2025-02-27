#include "caracteres.h"

const struct DibujableConstante Letra_A_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, ALTURA_CARACTER_MAX}, {0, 5}, {5, 0},{ANCHURA_CARACTER_MAX, 5},
        {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}, 
        {0, 9}, {ANCHURA_CARACTER_MAX, 9} // Puente A
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {5, 6}
    },
    7,
    5
};

const struct DibujableConstante Letra_B_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, ALTURA_CARACTER_MAX}, {0, 0}, {7, 0}, {ANCHURA_CARACTER_MAX, 2},
        {ANCHURA_CARACTER_MAX, 6}, {7, 8}, {ANCHURA_CARACTER_MAX, 10},
        {ANCHURA_CARACTER_MAX, 14}, {7, ALTURA_CARACTER_MAX}, {0, 8}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 0},
        {9, 5}
    },
    10,
    10
};


const struct DibujableConstante Letra_I_Base = {
    CENTRO_CONST_CARACTER,
    (struct Punto[]){
        {0, 0}, {ANCHURA_CARACTER_MAX, 0}, {5, 0}, {0, ALTURA_CARACTER_MAX},
        {5, ALTURA_CARACTER_MAX}, {ANCHURA_CARACTER_MAX, ALTURA_CARACTER_MAX}
    },
    (struct UnionAristas[]){
        {0, 1}, {2, 4}, {3, 5}
    },
    6,
    3
};
