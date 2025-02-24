#include "nave.h"

const struct DibujableConstante Nave_Base = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {0, 18}, {2, 18}, {3, 18}, {4, 12}, {4, 10}, // 0-4
        {8, 10}, {6, 8}, {6, 2}, {8, 0}, {14, 0}, // 5-9
        {16, 2}, {16, 8}, {14, 10}, {18, 10}, {18, 12}, // 10-14
        {20, 18}, {19, 18}, {22, 18}, {8, 12}, {6, 16}, // 15-19
        {7, 18}, {14, 12}, {16, 16}, {15, 18} // 20-23
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {1, 3}, // Pata izq (3)
        {3, 4}, {4, 5}, {5, 12}, {12, 13}, {13, 14}, {14, 21}, {21, 18}, {18, 3}, // Base (8)
        {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}, {11, 12}, // Cupula (7)
        {14, 15}, {16, 15}, {15, 17}, // Pata dcha (3)
        {18, 19}, {19, 20}, {21, 22}, {22, 23}, {19, 22} // Propulsor (5)
    },
    24,
    26
};

const struct DibujableConstante Nave_Propulsion_Maxima = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {6, 16}, // 19
        {16, 16}, // 22
        {11, 48} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

const struct DibujableConstante Nave_Propulsion_Media = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {6, 16}, // 19
        {16, 16}, // 22
        {11, 38} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

const struct DibujableConstante Nave_Propulsion_Minima = {
    CENTRO_CONST_NAVE,
    (struct Punto[]){
        {6, 16}, // 19
        {16, 16}, // 22
        {11, 28} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};