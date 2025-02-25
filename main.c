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
    escalarDibujable(nave_propulsion_maxRotacion, 2);
    escalarDibujable(naveMaxRotacion, 2);

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
                InvalidateRect(hwnd, NULL, FALSE); // Fuerza un repintado 
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Limpiar ventana
            RECT rect;
            GetClientRect(hwnd, &rect); // Obtener el tamaño de la ventana
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); // Pintar negro
            FillRect(hdc, &rect, brush); // Rellenar la ventana de negro
            DeleteObject(brush);

            // Pintar ventana
            pintar_pantalla(hdc);
            EndPaint(hwnd, &ps);
        } break;

        case WM_KEYDOWN: {
            if(wParam == VK_UP) manejar_tecla(ARRIBA);
            if(wParam == VK_LEFT) manejar_tecla(IZQUIERDA);
            if(wParam == VK_RIGHT) manejar_tecla(DERECHA);
            if(wParam == VK_SPACE) manejar_tecla(ESPACIO);
            if(wParam == 0x35 || wParam == VK_NUMPAD5) manejar_tecla(MONEDA);
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
