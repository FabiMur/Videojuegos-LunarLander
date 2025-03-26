#include "code/lunar_lander.h"
#include "code/palabra.h"
#include "resources/superficie_lunar.h"
#include "code/gestor_plataformas.h"
#include "code/variables_globales.h"

#include <stdio.h>
#include <windows.h>

#define timer 1
#define tamano_inicial_pantalla_X 1024
#define tamano_inicial_pantalla_Y 768
#define anchura_minima_ventana 500
#define altura_minima_ventana 375

uint32_t tamano_pantalla_X = 1024;
uint32_t tamano_pantalla_Y = 768;

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

        case WM_GETMINMAXINFO:
            // Limitar tamaño de ventana minima
            MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;

            pMinMax->ptMinTrackSize.x = anchura_minima_ventana;
            pMinMax->ptMinTrackSize.y = altura_minima_ventana;

            return 0;
    
        case WM_SIZE:
            // Cambio de tamaño de la pantalla
            int width = LOWORD(lParam);  // Nuevo ancho de la ventana
            int height = HIWORD(lParam); // Nueva altura de la ventana

            if ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)){  
                factor_resized_X = (float)width / tamano_pantalla_X;
                factor_resized_Y = (float)height / tamano_pantalla_Y;
            }
            tamano_pantalla_X = width;
            tamano_pantalla_Y = height;
            escalar_escena(factor_resized_X, factor_resized_Y);
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
            if (GetAsyncKeyState(VK_UP) & 0x8000) manejar_tecla(ARRIBA);
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) manejar_tecla(IZQUIERDA);
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) manejar_tecla(DERECHA);
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) manejar_tecla(ESPACIO);
            if (GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) manejar_tecla(MONEDA);
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

    // Creacion del rectangulo que será la pantalla con los valores iniciales
    RECT rc = {0, 0, tamano_inicial_pantalla_X, tamano_inicial_pantalla_Y};
    // Extender el rectangulo para que asuma el tamaño de bordes y cabecera
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    int anchoVentana = rc.right - rc.left;
    int altoVentana = rc.bottom - rc.top;

    HWND hwnd = CreateWindowEx(0, "RasterWindow", "VentanaPruebas",
                            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                            (rc.right - rc.left), (rc.bottom - rc.top), NULL,
                            NULL, hInstance, NULL);

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
