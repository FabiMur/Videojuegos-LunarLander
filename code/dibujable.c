#include "dibujable.h"
#include <stdio.h>


struct Dibujable* crearDibujable(const struct DibujableConstante* constante) {
    // Reservar memoria para el Dibujable
    struct Dibujable* dibujable = (struct Dibujable*)malloc(sizeof(struct Dibujable));
    if (!dibujable) return NULL;  // Si falla malloc(), retorna NULL

    dibujable->origen = constante->origen;
    dibujable->num_puntos = constante->num_puntos;
    dibujable->num_aristas = constante->num_aristas;
    dibujable->num_puntos_originales = constante->num_puntos;
    dibujable->num_aristas_originales = constante->num_aristas;
    dibujable->capacidad_puntos = constante->num_puntos + 8;
    dibujable->capacidad_aristas = constante->num_aristas + 8;

    // Reservar memoria y copiar los puntos
    dibujable->puntos = (struct Punto*)malloc(dibujable->capacidad_puntos * sizeof(struct Punto));
    if (!dibujable->puntos) {
        free(dibujable);
        return NULL;
    }
    memcpy(dibujable->puntos, constante->puntos, constante->num_puntos * sizeof(struct Punto));  

    // Reservar memoria y copiar las aristas
    dibujable->aristas = (struct Arista*)malloc(dibujable->capacidad_aristas * sizeof(struct Arista));
    if (!dibujable->aristas) {
        free(dibujable->puntos);
        free(dibujable);
        return NULL;
    }
    for (uint8_t i = 0; i < constante->num_aristas; i++) {
        dibujable->aristas[i].origen = &dibujable->puntos[constante->unionAristas[i].origen];
        dibujable->aristas[i].destino = &dibujable->puntos[constante->unionAristas[i].destino];
        dibujable->aristas[i].activa = 1;
    }
    return dibujable;
}

void destruirDibujable(struct Dibujable* dibujable) {
    if(dibujable == NULL) return;
    if(dibujable -> puntos != NULL){
        free(dibujable -> puntos);
    }
    if(dibujable -> aristas != NULL) {
        free(dibujable -> aristas);
    }
    free(dibujable);
}

void destruirArista(struct Arista* arista){
    if(arista == NULL) return;
    if(arista -> origen != NULL){
        free(arista -> origen);
    }
    if(arista -> destino != NULL){
        free(arista -> destino);
    }
    free(arista);
}


// Algoritmo de Bresenham para rasterizar una línea
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        SetPixel(hdc, x1, y1, color); // Dibuja el pixel actual

        if (x1 == x2 && y1 == y2) break; // Si llegamos al final, salimos

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}


uint8_t es_horizontal(struct Arista arista_colision){
    if(arista_colision.origen->y == arista_colision.destino->y) {
        return 1;
    }
    return 0;
}

void dibujarDibujable(HDC hdc, const struct Dibujable* dibujable){
    for(uint8_t i = 0; i < dibujable->num_aristas; i++) {
        if (dibujable->aristas[i].activa) {
            DrawLine(hdc, dibujable->aristas[i].origen->x,
                dibujable->aristas[i].origen->y,
                dibujable->aristas[i].destino->x,
                dibujable->aristas[i].destino->y,
                RGB(255, 255, 255));
        }
    }
}

struct Punto* crear_punto_temporal(struct Dibujable* dibujable, float x, float y) {
    if (dibujable->num_puntos >= dibujable->capacidad_puntos) {
        dibujable->capacidad_puntos += 2;
        dibujable->puntos = realloc(dibujable->puntos, dibujable->capacidad_puntos * sizeof(struct Punto));
    }
    dibujable->puntos[dibujable->num_puntos++] = (struct Punto){x, y};
    return &dibujable->puntos[dibujable->num_puntos - 1];
}


void crear_arista_temporal(struct Dibujable* dibujable, struct Punto* p1, struct Punto* p2) {
    if (dibujable->num_aristas >= dibujable->capacidad_aristas) {
        dibujable->capacidad_aristas += 2;
        dibujable->aristas = realloc(dibujable->aristas, dibujable->capacidad_aristas * sizeof(struct Arista));
    }
    dibujable->aristas[dibujable->num_aristas++] = (struct Arista){p1, p2, 1};
}

void desplazarDibujableLoop(struct Dibujable* dibujable, int dx, int dy) {
    // Recorrer puntos originales, desplazandolos
    dibujable->num_puntos = dibujable->num_puntos_originales;
    for (uint8_t i = 0; i < dibujable->num_puntos_originales; i++) {
        dibujable->puntos[i].x += dx;
		dibujable->puntos[i].y += dy;

        // Aplicar efecto de envoltura en X
        if (dibujable->puntos[i].x <= -1) {
			dibujable->puntos[i].x += 1024; // Tamaño de pantalla en X
        } else if (dibujable->puntos[i].x >= 1024) {
			dibujable->puntos[i].x -= 1024;
        }
    }
    // Recorrer aristas, activando por defecto, desactivando y creando nuevas (con nuevos puntos)
    dibujable->num_aristas = dibujable->num_aristas_originales;
    for (uint8_t i = 0; i < dibujable->num_aristas_originales; i++) {
        if (dibujable->aristas[i].origen->x > dibujable->aristas[i].destino->x) {
            struct Punto* p1 = dibujable->aristas[i].origen;
            struct Punto* p2 = dibujable->aristas[i].destino;

            float m = (p2->y - p1->y)/(p2->x + 1024 - p1->x);
            float n = p1->y - m*p1->x;
            float y = m*1023 + n;

            struct Punto* p1_temp = crear_punto_temporal(dibujable, 1023, y);
            struct Punto* p2_temp = crear_punto_temporal(dibujable, 0, y);

            crear_arista_temporal(dibujable, p1, p1_temp);
            crear_arista_temporal(dibujable, p2_temp, p2);

            dibujable->aristas[i].activa = 0;

        } else {
            dibujable->aristas[i].activa = 1;
        }
    }
}
