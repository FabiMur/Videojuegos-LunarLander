#include "palabra.h"

#include "../resources/caracteres.h"
#include "transformaciones.h"

struct Palabra* crear_palabra(struct Punto origen){
    struct Palabra* palabra = (struct Palabra*)malloc(sizeof(struct Palabra));
    palabra->origen = origen;
    palabra->num_letras = 0;
    palabra->letras = NULL;
    return palabra;
}

void agregar_letra(struct Palabra* palabra, struct DibujableConstante* letra) {
    // Aumentar el tamaño del arreglo de letras
    palabra->num_letras++;
    palabra->letras = (struct Dibujable*)realloc(palabra->letras,
                       palabra->num_letras * sizeof(struct Dibujable));
    
    // Definir como dibujable y ajustar posicion trasladando el origen
    struct Dibujable* letraDibujable = crearDibujable(letra);
    struct Punto nuevoOrigen = {palabra->origen.x + (palabra->num_letras - 1) *
                               (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER),
                               palabra->origen.y};
    trasladarDibujable(letraDibujable, nuevoOrigen);

    // Añadir la letra a la palabra
    palabra->letras[palabra->num_letras - 1] = *letraDibujable;
}

void dibujar_palabra(struct Palabra* palabra, HDC hdc) {
    for (uint8_t i = 0; i < palabra->num_letras; i++) {
        dibujarDibujable(hdc, &palabra->letras[i]);
    }
}

void destruir_palabra(struct Palabra* palabra) {
    if(palabra == NULL) return;
    if(palabra -> letras != NULL){
        for(uint8_t i = 0; i < palabra -> num_letras; i++){
            destruirDibujable(&palabra -> letras[i]);
        }
        free(palabra -> letras);
    }
    free(palabra);
}