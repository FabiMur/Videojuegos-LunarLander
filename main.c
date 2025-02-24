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

// Función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Dibuja una línea de ejemplo
            struct Dibujable* letraB = crearDibujable(&Letra_B_Base);
            trasladarDibujable(letraB, (struct Punto){50, 50});
            struct Dibujable* letraA = crearDibujable(&Letra_A_Base);
            trasladarDibujable(letraA, (struct Punto){50+ANCHURA_MAX+5, 50});
            struct Dibujable* letraI = crearDibujable(&Letra_I_Base);
            struct Dibujable* palabraBA[] = {letraB, letraA};

            struct Dibujable* nave = crearDibujable(&Nave_Base);
            trasladarDibujable(nave, (struct Punto){10, 10});
            struct Dibujable* nave_propulsion = crearDibujable(&Nave_Propulsion);
            trasladarDibujable(nave_propulsion, (struct Punto){10, 10});
            

            for(int i = 0; i < (sizeof(palabraBA) / sizeof(palabraBA[0])); i++){
                for(int j = 0; j < palabraBA[i]->num_aristas; j++){
                    DrawLine(hdc, palabraBA[i]->aristas[j].origen->x, palabraBA[i]->aristas[j].origen->y, palabraBA[i]->aristas[j].destino->x, palabraBA[i]->aristas[j].destino->y, RGB(255, 255, 255));
                }
            }

            for(int i = 0; i < nave->num_aristas; i++){
                DrawLine(hdc, nave->aristas[i].origen->x, nave->aristas[i].origen->y, nave->aristas[i].destino->x, nave->aristas[i].destino->y, RGB(255, 255, 255));
            }
            for(int i = 0; i < nave_propulsion->num_aristas; i++){
                DrawLine(hdc, nave_propulsion->aristas[i].origen->x, nave_propulsion->aristas[i].origen->y, nave_propulsion->aristas[i].destino->x, nave_propulsion->aristas[i].destino->y, RGB(255, 255, 255));
            }

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
