#ifndef NAVE_H
#define NAVE_H

#include "../code/dibujable.h"

#define CENTRO_CONST_NAVE {9, 5}
#define ANCHURA_NAVE 18
#define ALTURA_NAVE 18

// nave.h
extern const struct DibujableConstante Nave_Base;
extern const struct DibujableConstante Nave_Propulsion_Maxima;
extern const struct DibujableConstante Nave_Propulsion_Media;
extern const struct DibujableConstante Nave_Propulsion_Minima;

extern const struct DibujableConstante Trozo_Pata_Izquierda;
extern const struct DibujableConstante Trozo_Base_Central;
extern const struct DibujableConstante Trozo_Cupula_Superior;
extern const struct DibujableConstante Trozo_Pata_Derecha;

#endif // NAVE_H
