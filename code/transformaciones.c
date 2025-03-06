#include "transformaciones.h"


struct Rotacion {
    const uint8_t grados;
    const double radianes;
    const double seno;
    const double coseno;
};

// Valores precomputados de sin(15°) y cos(15°)
const struct Rotacion Rotacion_15 = {
    15,
    0.261799,  // 15 * (π / 180)
    0.258819,  // sin(15°)
    0.965926   // cos(15°)
};

const struct Rotacion Rotacion_30 = {
    30,
    0.523599,  // 30 * (π / 180)
    0.5,       // sin(30°)
    0.866025   // cos(30°)
};

const struct Rotacion Rotacion_45 = {
    45,
    0.785398,  // 45 * (π / 180)
    0.707107,  // sin(45°)
    0.707107   // cos(45°)
};

/**
 * @brief Traslada un punto en la dirección indicada
 * 
 * @param origen Punto a trasladar
 * @param traslacion Punto de traslación
 */
void trasladarPunto(struct Punto* origen, struct Punto traslacion){
    origen -> x = origen -> x + traslacion.x;
    origen -> y = origen -> y + traslacion.y;
}

void trasladarDibujable(struct Dibujable* dibujable, struct Punto traslacion){
    trasladarPunto(&dibujable -> origen, traslacion);
    if (!dibujable -> puntos) return;
    for(uint8_t i = 0; i < dibujable -> num_puntos; i++){
        trasladarPunto(&dibujable -> puntos[i], traslacion);
    }
}

void colocarDibujable(struct Dibujable* dibujable, struct Punto destino){
	struct Punto traslacion = {
		destino.x - dibujable -> origen.x,
		destino.y - dibujable -> origen.y
	};
    if (!dibujable -> puntos) return;
    for(uint8_t i = 0; i < dibujable -> num_puntos; i++){
        trasladarPunto(&dibujable -> puntos[i], traslacion);
    }
}

/**
 * @brief Rota un punto dado un centro y un sentido
 * 
 * @param punto Punto a rotar
 * @param centro Centro de rotación
 * @param sentido 1 para horario, -1 para antihorario
 */
void rotarPuntoDadoCentro(struct Punto* punto, struct Punto centro, int8_t sentido){
    int16_t x = punto->x - centro.x;
    int16_t y = punto->y - centro.y;
    int16_t nuevaX, nuevaY;

    if(ANGULO_ROTACION == 15){
        nuevaX = x * Rotacion_15.coseno - y * sentido * Rotacion_15.seno;
        nuevaY = x * sentido * Rotacion_15.seno + y * Rotacion_15.coseno;
    }
    else if(ANGULO_ROTACION == 30){
        nuevaX = x * Rotacion_30.coseno - y * sentido * Rotacion_30.seno;
        nuevaY = x * sentido * Rotacion_30.seno + y * Rotacion_30.coseno;
    }
    else if(ANGULO_ROTACION == 45){
        nuevaX = x * Rotacion_45.coseno - y * sentido * Rotacion_45.seno;
        nuevaY = x * sentido * Rotacion_45.seno + y * Rotacion_45.coseno;
    }
    else{
        nuevaX = x * cos(ANGULO_ROTACION) - y * sentido * sin(ANGULO_ROTACION);
        nuevaY = x * sentido * sin(ANGULO_ROTACION) + y * cos(ANGULO_ROTACION);
    }
    punto->x = (nuevaX + centro.x);
    punto->y = (nuevaY + centro.y);
}

void rotarDibujable(struct Dibujable* dibujable, unsigned char direccion){
    if (!dibujable->puntos) return;
    int8_t sentido = (direccion == 0) ? 1 : -1;
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        rotarPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, sentido);
    }
}


void escalarXPuntoDadoCentro(struct Punto* punto, struct Punto centro, double factor){
    int16_t x = punto->x - centro.x;
    punto->x = (int16_t)(x * factor + centro.x);
}

void escalarYPuntoDadoCentro(struct Punto* punto, struct Punto centro, double factor){
    int16_t y = punto->y - centro.y;
    punto->y = (int16_t)(y * factor + centro.y);
}

void escalarDibujable(struct Dibujable* dibujable, double factor){
    escalarDibujableDadosEjes(dibujable, factor, factor);
}

void escalarDibujableDadosEjes(struct Dibujable* dibujable, double factorX, double factorY){
    if (!dibujable->puntos) return;
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        escalarXPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, factorX);
        escalarYPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, factorY);
    }
}
        
