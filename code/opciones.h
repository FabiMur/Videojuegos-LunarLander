#ifndef OPCIONES_H
#define OPCIONES_H

#include <windows.h>

// Enumeracion de flags configurables
typedef enum {
    FLAG_MUSICA,
    FLAG_FPS,
    NUM_FLAGS
} OpcionFlag;

void inicializarOpciones(void);
void destruirOpciones(void);
void dibujarOpcionesEnBuffer(HDC hdc);
LRESULT procesarEventoOpciones(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int obtenerValorFlag(OpcionFlag flag);

#endif // OPCIONES_H
