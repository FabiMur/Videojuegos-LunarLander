#include "dibujable.h"


struct Dibujable* crearDibujable(const struct DibujableConstante* constante) {
    // Reservar memoria para el Dibujable
    struct Dibujable* dibujable = (struct Dibujable*)malloc(sizeof(struct Dibujable));
    if (!dibujable) return NULL;  // Si falla malloc(), retorna NULL

    dibujable->origen = constante->origen;
    dibujable->num_puntos = constante->num_puntos;
    dibujable->num_aristas = constante->num_aristas;

    // Reservar memoria y copiar los puntos
    dibujable->puntos = (struct Punto*)malloc(constante->num_puntos * sizeof(struct Punto));
    if (!dibujable->puntos) {
        free(dibujable);
        return NULL;
    }
    memcpy(dibujable->puntos, constante->puntos, constante->num_puntos * sizeof(struct Punto));  

    // Reservar memoria y copiar las aristas
    dibujable->aristas = (struct Arista*)malloc(constante->num_aristas * sizeof(struct Arista));
    if (!dibujable->aristas) {
        free(dibujable->puntos);
        free(dibujable);
        return NULL;
    }
    for (uint8_t i = 0; i < constante->num_aristas; i++) {
        dibujable->aristas[i].origen = &dibujable->puntos[constante->unionAristas[i].origen];
        dibujable->aristas[i].destino = &dibujable->puntos[constante->unionAristas[i].destino];
    }
    return dibujable;
}

void destruirDibujable(struct Dibujable* dibujable) {
    if (!dibujable) return;
    free(dibujable->puntos);
    free(dibujable->aristas);
    free(dibujable);
}