#ifndef OVERLAYS_H
#define OVERLAYS_H

#include <windows.h>

typedef enum {
    FIN_OPCION_INSERT_COIN,
    FIN_OPCION_MENU,
    FIN_NUM_OPCIONES
} OpcionFin;

typedef enum {
    PAUSE_OPCION_CONTINUAR,
    PAUSE_OPCION_INSERT_COIN,
    PAUSE_OPCION_MENU,
    PAUSE_NUM_OPCIONES
} OpcionPausa;

void procesarEventoOverlayFin(UINT uMsg, WPARAM wParam);
void procesarEventoOverlayPausa(UINT uMsg, WPARAM wParam);

OpcionFin obtenerOpcionFin(void);
OpcionPausa obtenerOpcionPausa(void);

void dibujarOverlayFin(HDC hdc, int tiempo, int puntuacion);
void dibujarOverlayPausa(HDC hdc, int tiempo, int puntuacion);

#endif // OVERLAYS_H