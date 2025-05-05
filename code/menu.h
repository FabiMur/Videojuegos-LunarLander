#ifndef MENU_H
#define MENU_H

#include <windows.h>

typedef enum {
    OPCION_PLAY,
    OPCION_TEST_DIBUJABLES,
    OPCION_OPTIONS,
    OPCION_EXIT,
    NUM_OPCIONES 
} OpcionMenu;

void inicializarMenu(void);

// Dibuja el menu en el buffer de la ventana.
void dibujarMenuEnBuffer(HDC hdc);


LRESULT procesarEventoMenu(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

OpcionMenu obtenerOpcionSeleccionada(void);

#endif // MENU_H
