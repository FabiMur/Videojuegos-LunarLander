
#ifndef TRANSFORMACIONES_H
#define TRANSFORMACIONES_H

#define ANGULO_ROTACION 45

#include <stdio.h>
#include <math.h>

#include "dibujable.h"

/**
 * @brief Traslada un dibujable en la dirección indicada
 * 
 * @param dibujable Dibujable a trasladar
 * @param traslacion Punto de traslación
 */
void trasladarDibujable(struct Dibujable* dibujable, struct Punto traslacion);

/**
 * @brief Rota un dibujable en sentido horario o antihorario
 * 
 * @param dibujable Dibujable a rotar
 * @param direccion 0 para izquierda, 1 para derecha
 */
void rotarDibujable(struct Dibujable* dibujable, unsigned char direccion);


#endif // TRANSFORMACIONES_H