#include "transformaciones.h"

struct Punto trasladarPunto(struct Punto origen, struct Punto traslacion){
    return (struct Punto){origen.x + traslacion.x, origen.y + traslacion.y};
}

void trasladarDibujable(struct Dibujable* dibujable, struct Punto traslacion){
    dibujable->origen = trasladarPunto(dibujable->origen, traslacion);
    if (!dibujable->puntos) return;
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        dibujable->puntos[i] = trasladarPunto(dibujable->puntos[i], traslacion);
    }
}
