#include "opciones.h"
#include "texto.h"
#include "../resources/caracteres.h"
#include "../code/transformaciones.h"
#include "../code/config.h"
#include <stdio.h>
#include <windows.h>

static int flags[NUM_FLAGS] = {1, 0}; // valores por defecto
static struct Texto* opcionesTextuales[NUM_FLAGS] = {0};
static int opcionSeleccionadaOpc = 0;
static int opcionesPosX = 0;
static int opcionesPosY_inicial = 0;
static const int espacioEntreOpciones = 30;
// Separación entre el texto de la opción y el valor <SI>/<NO>
static const int paddingValor = 100;
// Ancho total de cada fila (texto + padding + valor)
static int anchoFilaOpciones = 0;

static const char* cadenasOpcionesFlags[NUM_FLAGS] = {
    "SOUND",
    "ASTEROIDS",
    "AI"
};

void inicializarOpciones(void) {
    struct Punto origenTemp = {0,0};
    for(int i=0;i<NUM_FLAGS;i++) {
        opcionesTextuales[i] = crearTextoDesdeCadena(cadenasOpcionesFlags[i], origenTemp);
    }
}

void destruirOpciones(void) {
    for(int i=0;i<NUM_FLAGS;i++) {
        if(opcionesTextuales[i]) {
            destruir_texto(opcionesTextuales[i]);
            opcionesTextuales[i] = NULL;
        }
    }
}

static void actualizarPosicionesOpciones(void) {
    const int anchoCliente = BASE_W;
    const int altoCliente  = BASE_H;

    int anchoTextoMax = 0;

    for(int i=0;i<NUM_FLAGS;i++) {
        int anchoOpcion = opcionesTextuales[i]->num_caracteres * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
        if(anchoOpcion > anchoTextoMax) anchoTextoMax = anchoOpcion;
    }

    int anchoValor = 4 * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
    anchoFilaOpciones = anchoTextoMax + paddingValor + anchoValor;

    opcionesPosX = (anchoCliente - anchoFilaOpciones) / 2;
    
    int altoTotal = NUM_FLAGS * ALTURA_CARACTER_MAX + (NUM_FLAGS - 1)*espacioEntreOpciones;
    opcionesPosY_inicial = (altoCliente - altoTotal) / 2;

    for(int i=0;i<NUM_FLAGS;i++) {
        struct Punto o = { (float)opcionesPosX, (float)(opcionesPosY_inicial + i * (ALTURA_CARACTER_MAX + espacioEntreOpciones)) };
        opcionesTextuales[i]->origen = o;
        colocar_texto(opcionesTextuales[i], o);
    }
}

void dibujarOpcionesEnBuffer(HDC hdc) {
    actualizarPosicionesOpciones();

    const int marginTitulo = 70;
    const char* tituloCad = "OPTIONS";
    int numCarTitulo = 7; // OPTIONS
    int anchoTitulo = numCarTitulo * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
    int xTitulo = (BASE_W - anchoTitulo) / 2;
    int yTitulo = opcionesPosY_inicial - ALTURA_CARACTER_MAX - marginTitulo;
    struct Punto oT = { (float)xTitulo, (float)yTitulo };

    struct Texto* titulo = crearTextoDesdeCadena(tituloCad, oT);
    dibujar_texto(titulo, hdc);
    destruir_texto(titulo);

    for(int i=0;i<NUM_FLAGS;i++) {
        if(i == opcionSeleccionadaOpc) {
            struct Punto ind = { opcionesTextuales[i]->origen.x - 2*ANCHURA_CARACTER_MAX, opcionesTextuales[i]->origen.y };
            struct Texto* indicador = crearTextoDesdeCadena(">", ind);
            dibujar_texto(indicador, hdc);
            destruir_texto(indicador);
        }
        dibujar_texto(opcionesTextuales[i], hdc);

        char valor[8];
        const char* txt = flags[i] ? "ON " : "OFF";
        snprintf(valor, sizeof(valor), "<%s>", txt);
        int anchoValor = 4 * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
        int xValor = opcionesPosX + anchoFilaOpciones - anchoValor;
        struct Punto o = { (float)xValor, opcionesTextuales[i]->origen.y };
        struct Texto* valorTxt = crearTextoDesdeCadena(valor, o);
        dibujar_texto(valorTxt, hdc);
        destruir_texto(valorTxt);
    }
}

LRESULT procesarEventoOpciones(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    (void)lParam;
    if(uMsg == WM_KEYDOWN) {
        switch(wParam) {
            case VK_UP:
                if(opcionSeleccionadaOpc > 0) opcionSeleccionadaOpc--;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            case VK_DOWN:
                if(opcionSeleccionadaOpc < NUM_FLAGS - 1) opcionSeleccionadaOpc++;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            case VK_LEFT:
            case VK_RIGHT:
                flags[opcionSeleccionadaOpc] = !flags[opcionSeleccionadaOpc];
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            default:
                break;
        }
    }
    return 0;
}

int obtenerValorFlag(OpcionFlag flag) {
    if(flag < 0 || flag >= NUM_FLAGS) return 0;
    return flags[flag];
}
