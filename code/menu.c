#include "menu.h"
#include "texto.h"
#include "../resources/caracteres.h"
#include "../code/transformaciones.h"
#include "../code/config.h"
#include "../code/sonidos.h"
#include <stdio.h>
#include <windows.h>

// Variable internas para el menu
static OpcionMenu opcionSeleccionada = OPCION_PLAY;

// Número de opciones definido en el header (NUM_OPCIONES)
static struct Texto* opcionesTextuales[NUM_OPCIONES] = {0};

// Esto luego se recalcula al dibujar el menú
static int menuPosX = 0;
static int menuPosY_inicial = 0;
static const int espacioEntreOpciones = 30;

// Cadena de cada opción en mayúsculas.
static const char* cadenasOpciones[NUM_OPCIONES] = {
    "PLAY",
    "OPTIONS",
    "EXIT"
};

// Función auxiliar (definida en este módulo o en otro archivo de utilidades)
// ya está definida arriba: crearTextoDesdeCadena

void inicializarMenu(void) {
    printf("Inicializando menú...\n");
    opcionSeleccionada = OPCION_PLAY;
    // Esto se actualiza al dibujar
    struct Punto origenTemporal = {0, 0};
    // Se crean los objetos Texto para cada opción a partir de sus cadenas.
    for (int i = 0; i < NUM_OPCIONES; i++) {
        printf("Creando cadena de opciones de menu...\n");
        opcionesTextuales[i] = crearTextoDesdeCadena(cadenasOpciones[i], origenTemporal);
    }
    printf("Cadenas de menu inicializadas\n");
}

void destruirMenu(void) {
    // Libera cada objeto Texto creado para las opciones.
    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (opcionesTextuales[i]) {
            destruir_texto(opcionesTextuales[i]);
            opcionesTextuales[i] = NULL;
        }
    }
}

static void actualizarPosicionesMenu(void) {
    // Se usa siempre la resolucon base
    const int anchoCliente = BASE_W;
    const int altoCliente  = BASE_H;

    // Medir ancho maximo de las opciones
    int anchoTotalMenu = 0;
    for (int i = 0; i < NUM_OPCIONES; i++) {
        int anchoOpcion = opcionesTextuales[i]->num_caracteres
                        * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
        if (anchoOpcion > anchoTotalMenu)
            anchoTotalMenu = anchoOpcion;
    }

    // Centrado horizontal en BASE_W
    menuPosX = (anchoCliente - anchoTotalMenu) / 2;

    // Altura total del bloque de opciones
    int altoTotalMenu = NUM_OPCIONES * ALTURA_CARACTER_MAX
                      + (NUM_OPCIONES - 1) * espacioEntreOpciones;

    // Centrado vertical en BASE_H
    menuPosY_inicial = (altoCliente - altoTotalMenu) / 2;

    // Actualizar el origen de cada texto de opción
    for (int i = 0; i < NUM_OPCIONES; i++) {
        struct Punto nuevoOrigen = {
            (float)menuPosX,
            (float)(menuPosY_inicial
                    + i * (ALTURA_CARACTER_MAX + espacioEntreOpciones))
        };
        opcionesTextuales[i]->origen = nuevoOrigen;
        colocar_texto(opcionesTextuales[i], nuevoOrigen);
    }
}

void dibujarMenuEnBuffer(HDC hdc) {
    // Recalcula posiciones del menu basandose en BASE_W y BASE_H
    actualizarPosicionesMenu();

    // Dibujar el titulo
    const int marginTitulo       = 70;
    const int numCaracteresTitulo = 12;  // "LUNAR LANDER"
    int anchoTitulo = numCaracteresTitulo * (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER);
    int xTitulo     = (BASE_W - anchoTitulo) / 2;
    int yTitulo     = menuPosY_inicial - ALTURA_CARACTER_MAX - marginTitulo;
    struct Punto origenTitulo = { (float)xTitulo, (float)yTitulo };

    struct Texto* titulo = crearTextoDesdeCadena("LUNAR LANDER", origenTitulo);
    dibujar_texto(titulo, hdc);
    destruir_texto(titulo);

    // Dibuja cada opción con el indicador ">" a la izquierda de la opción seleccionada
    for (OpcionMenu i = 0; i < NUM_OPCIONES; i++) {
        if (i == obtenerOpcionSeleccionada()) {
            // indicador a la izquierda de la opción
            struct Punto ind = {
                opcionesTextuales[i]->origen.x - 2 * ANCHURA_CARACTER_MAX,
                opcionesTextuales[i]->origen.y
            };
            struct Texto* indicador = crearTextoDesdeCadena(">", ind);
            dibujar_texto(indicador, hdc);
            destruir_texto(indicador);
        }
        // Dibujar la opción de menú
        dibujar_texto(opcionesTextuales[i], hdc);
    }
}


LRESULT procesarEventoMenu(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    (void)lParam;
    if(uMsg == WM_KEYDOWN) {
        switch(wParam) {
            case VK_UP:
                if(opcionSeleccionada > OPCION_PLAY){
                    opcionSeleccionada--;
                }
                InvalidateRect(hwnd, NULL, TRUE);  // repintado
                break;
            case VK_DOWN:
                if(opcionSeleccionada < NUM_OPCIONES - 1){
                    opcionSeleccionada++;
                }
                InvalidateRect(hwnd, NULL, TRUE); // repintado
                break;
            default:
                break;
        }
    }
    return 0;
}

OpcionMenu obtenerOpcionSeleccionada(void) {
    return opcionSeleccionada;
}
