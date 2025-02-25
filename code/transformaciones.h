
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

/**
 * @brief Escala un dibujable en el eje x e y
 * 
 * @param dibujable Dibujable a escalar
 * @param factor Factor de escala
 */
void escalarDibujable(struct Dibujable* dibujable, double factor);

/**
 * @brief Escala un dibujable en el eje x y el eje y dados los factores de
 *        escalado de cada eje
 * 
 * @param dibujable Dibujable a escalar
 * @param factorX Factor de escala en el eje x
 * @param factorY Factor de escala en el eje y
 */
void escalarDibujableDadosEjes(struct Dibujable* dibujable, double factorX, double factorY);

#endif // TRANSFORMACIONES_H