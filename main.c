#include "code/lunar_lander.h"
#include "code/palabra.h"
#include "resources/superficie_lunar.h"
#include "code/gestor_plataformas.h"

#include <stdio.h>
#include <windows.h>

#define timer 1

uint8_t primera_vez = 1;
uint8_t num_plataformas = 0;
struct Plataforma* plataformas;

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
void pruebasDibujables(HDC hdc){
    
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
		
			// Crear un buffer en memoria
			HDC hdcMem = CreateCompatibleDC(hdc);
			RECT rect;
			GetClientRect(hwnd, &rect);
			HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			HGDIOBJ hOld = SelectObject(hdcMem, hbmMem);
		
			// Limpiar el buffer (pintarlo de negro)
			HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
			FillRect(hdcMem, &rect, brush);
			DeleteObject(brush);
		
			// Dibujar en el buffer en memoria
			pruebasDibujables(hdcMem);
			pintar_pantalla(hdcMem);
		
			// Copiar el buffer en la ventana
			BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
		
			// Liberar recursos
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);
		
			EndPaint(hwnd, &ps);
		}
		break;

        case WM_KEYDOWN: {
            if(wParam == VK_UP) manejar_tecla(ARRIBA);
            if(wParam == VK_LEFT) manejar_tecla(IZQUIERDA);
            if(wParam == VK_RIGHT) manejar_tecla(DERECHA);
            if(wParam == VK_SPACE) manejar_tecla(ESPACIO);
            if(wParam == 0x35 || wParam == VK_NUMPAD5) manejar_tecla(MONEDA);
        }
		break;
        
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
                               1024, 768, NULL, NULL, hInstance, NULL);

    inicializar_aleatoriedad(); // Inicializar rand
    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
