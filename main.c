#include "code/lunar_lander.h"

#include <stdio.h>
#include <windows.h>

#define timer 1

void AttachConsoleToStdout() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirige stdout a la consola
    freopen("CONOUT$", "w", stderr);  // Redirige stderr también
}


/**
 * @brief Funcion para realizar pruebas de dibujo de las naves
 * 
 * @param hdc
 */
void dibujoNaves(HDC hdc){
    
    struct Dibujable* naveMaxRotacion = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMaxRotacion, (struct Punto){280, 10});
    rotarDibujable(naveMaxRotacion, 0);
    struct Dibujable* nave_propulsion_maxRotacion = crearDibujable(&Nave_Propulsion_Maxima);
    rotarDibujable(nave_propulsion_maxRotacion, 0);
    trasladarDibujable(nave_propulsion_maxRotacion, (struct Punto){280, 10});

    dibujarDibujable(hdc, naveMaxRotacion);
    dibujarDibujable(hdc, nave_propulsion_maxRotacion);
}

// Función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            SetTimer(hwnd, timer, intervalo_fisicas_ms, NULL);
            break;

        case WM_TIMER:
            if (wParam == timer) {
                manejar_instante();
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Pruebas de rasterizado 
            dibujoNaves(hdc);

            dibujarEscena(hdc);
            EndPaint(hwnd, &ps);
        } break;

        case WM_KEYUP: {
            if(GetKeyState(VK_UP) & 0x8000) manejar_tecla(ARRIBA);
            if(GetKeyState(VK_LEFT) & 0x8000) manejar_tecla(IZQUIERDA);
            if(GetKeyState(VK_RIGHT) & 0x8000) manejar_tecla(DERECHA);
            if(GetKeyState(VK_SPACE) & 0x8000) manejar_tecla(ESPACIO);
            if(GetKeyState(0x35) & 0x8000 || GetKeyState(VK_NUMPAD5) & 0x8000) manejar_tecla(MONEDA);
        } break;
        
        case WM_DESTROY:
            KillTimer(hwnd, timer);
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
