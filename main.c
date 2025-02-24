#include "code/dibujable.h"
#include "resources/letras.h"
#include "resources/nave.h"
#include "code/transformaciones.h"

#include <stdio.h>
#include <windows.h>

void AttachConsoleToStdout() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirige stdout a la consola
    freopen("CONOUT$", "w", stderr);  // Redirige stderr también
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

/**
 * @brief Funcion para realizar pruebas de dibujo de las letras
 * 
 * @param hdc
 */
void dibujoLetras(HDC hdc){
    struct Dibujable* letraB = crearDibujable(&Letra_B_Base);
    trasladarDibujable(letraB, (struct Punto){50, 50});
    struct Dibujable* letraA = crearDibujable(&Letra_A_Base);
    trasladarDibujable(letraA, (struct Punto){50+ANCHURA_MAX+5, 50});
    struct Dibujable* letraI = crearDibujable(&Letra_I_Base);
    trasladarDibujable(letraI, (struct Punto){50+2*(ANCHURA_MAX+5), 50});
    rotarDibujable(letraI, 1);
    struct Dibujable* palabraBA[] = {letraB, letraA, letraI};

    for(int i = 0; i < (sizeof(palabraBA) / sizeof(palabraBA[0])); i++){
        for(int j = 0; j < palabraBA[i]->num_aristas; j++){
            DrawLine(hdc, palabraBA[i]->aristas[j].origen->x,
                     palabraBA[i]->aristas[j].origen->y,
                     palabraBA[i]->aristas[j].destino->x,
                     palabraBA[i]->aristas[j].destino->y,
                     RGB(255, 255, 255));
        }
    }
}

/**
 * @brief Funcion para realizar pruebas de dibujo de las naves
 * 
 * @param hdc
 */
void dibujoNaves(HDC hdc){
    struct Dibujable* naveMax = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMax, (struct Punto){150, 10});
    struct Dibujable* nave_propulsion_max = crearDibujable(&Nave_Propulsion_Maxima);
    trasladarDibujable(nave_propulsion_max, (struct Punto){150, 10});

    struct Dibujable* naveMedia = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMedia, (struct Punto){180, 10});
    struct Dibujable* nave_propulsion_media = crearDibujable(&Nave_Propulsion_Media);
    trasladarDibujable(nave_propulsion_media, (struct Punto){180, 10});

    struct Dibujable* naveMin = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMin, (struct Punto){210, 10});
    struct Dibujable* nave_propulsion_min = crearDibujable(&Nave_Propulsion_Minima);
    trasladarDibujable(nave_propulsion_min, (struct Punto){210, 10});

    struct Dibujable* naveMaxRotacion = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMaxRotacion, (struct Punto){280, 10});
    rotarDibujable(naveMaxRotacion, 0);
    struct Dibujable* nave_propulsion_maxRotacion = crearDibujable(&Nave_Propulsion_Maxima);
    rotarDibujable(nave_propulsion_maxRotacion, 0);
    trasladarDibujable(nave_propulsion_maxRotacion, (struct Punto){280, 10});

    struct Dibujable* naveMaxRotacion2 = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMaxRotacion2, (struct Punto){340, 30});
    rotarDibujable(naveMaxRotacion2, 1);
    struct Dibujable* nave_propulsion_maxRotacion2 = crearDibujable(&Nave_Propulsion_Maxima);
    rotarDibujable(nave_propulsion_maxRotacion2, 1);
    trasladarDibujable(nave_propulsion_maxRotacion2, (struct Punto){340, 30});

    struct Dibujable* naves[] = {naveMax, nave_propulsion_max, naveMedia,
                                 nave_propulsion_media, naveMin,
                                 nave_propulsion_min,
                                 naveMaxRotacion, nave_propulsion_maxRotacion,
                                 naveMaxRotacion2, nave_propulsion_maxRotacion2};

    for(int i = 0; i < (sizeof(naves) / sizeof(naves[0])); i++){
        for(int j = 0; j < naves[i]->num_aristas; j++){
            DrawLine(hdc, naves[i]->aristas[j].origen->x,
                     naves[i]->aristas[j].origen->y,
                     naves[i]->aristas[j].destino->x,
                     naves[i]->aristas[j].destino->y,
                     RGB(255, 255, 255));
        }
    }    
}

// Función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Pruebas de rasterizado
            dibujoLetras(hdc);  
            dibujoNaves(hdc);

            EndPaint(hwnd, &ps);
        } break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Función principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AttachConsoleToStdout(); // Activa la consola para ver printf
    printf("Estamos aqui 1\n");
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RasterWindow";

    // Fondo negro
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "RasterWindow", "VentanaPruebas",
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                               500, 500, NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
