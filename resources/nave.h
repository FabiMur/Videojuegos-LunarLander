#ifndef NAVE_H
#define NAVE_H


#include "../code/dibujable.h"

const struct DibujableConstante Nave_Base = {
    {11, 5},
    (struct Punto[]){
        {0, 18}, //0
        {2, 18},
        {3, 18},
        {4, 12},
        {4, 10},
        {8, 10}, // 5
        {6, 8},
        {6, 2},
        {8, 0},
        {14, 0},
        {16, 2}, //10
        {16, 8},
        {14, 10},
        {18, 10},
        {18, 12},
        {20, 18}, //15
        {19, 18},
        {22, 18},
        {8, 12},
        {6, 16},
        {7, 18}, //20
        {14, 12},
        {16, 16},
        {15, 18}
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



// Habrá que ver como hacerlo porque igual el punto de esquina se desplaza
const struct DibujableConstante Nave_Propulsion = {
    {11, 5},
    (struct Punto[]){
        {6, 16}, // 19
        {16, 16}, // 22
        {11, 35} // Centro
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 0}
    },
    3,
    3
};

#endif // NAVE_H   
