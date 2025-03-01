#include "code/lunar_lander.h"
#include "code/palabra.h"

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
void pruebasDibujables(HDC hdc){

    // Dibujar nave base
    struct Dibujable* naveMaxRotacion = crearDibujable(&Nave_Base);
    trasladarDibujable(naveMaxRotacion, (struct Punto){280, 10});
    rotarDibujable(naveMaxRotacion, 0);
    struct Dibujable* nave_propulsion_maxRotacion = crearDibujable(&Nave_Propulsion_Maxima);
    rotarDibujable(nave_propulsion_maxRotacion, 0);
    trasladarDibujable(nave_propulsion_maxRotacion, (struct Punto){280, 10});
    escalarDibujable(nave_propulsion_maxRotacion, 2);
    escalarDibujable(naveMaxRotacion, 2);

    // Probar palabra
    struct Palabra* letras = crear_palabra((struct Punto){20, 100});
    agregar_letra(letras, &Letra_A_Base);
    agregar_letra(letras, &Letra_B_Base);
    agregar_letra(letras, &Letra_C_Base);
    agregar_letra(letras, &Letra_D_Base);
    agregar_letra(letras, &Letra_E_Base);
    agregar_letra(letras, &Letra_F_Base);
    agregar_letra(letras, &Letra_G_Base);
    agregar_letra(letras, &Letra_H_Base);
    agregar_letra(letras, &Letra_I_Base);
    agregar_letra(letras, &Letra_J_Base);
    agregar_letra(letras, &Letra_K_Base);
    agregar_letra(letras, &Letra_L_Base);
    agregar_letra(letras, &Letra_M_Base);
    agregar_letra(letras, &Letra_N_Base);
    agregar_letra(letras, &Letra_O_Base);
    agregar_letra(letras, &Letra_P_Base);
    agregar_letra(letras, &Letra_Q_Base);
    agregar_letra(letras, &Letra_R_Base);
    agregar_letra(letras, &Letra_S_Base);
    agregar_letra(letras, &Letra_T_Base);
    agregar_letra(letras, &Letra_U_Base);
    agregar_letra(letras, &Letra_V_Base);
    agregar_letra(letras, &Letra_W_Base);
    agregar_letra(letras, &Letra_X_Base);
    agregar_letra(letras, &Letra_Y_Base);
    agregar_letra(letras, &Letra_Z_Base);

    struct Palabra* numeros = crear_palabra((struct Punto){20, 140});
    agregar_letra(numeros, &Numero_0_Base);
    agregar_letra(numeros, &Numero_1_Base);
    agregar_letra(numeros, &Numero_2_Base);
    agregar_letra(numeros, &Numero_3_Base);
    agregar_letra(numeros, &Numero_4_Base);
    agregar_letra(numeros, &Numero_5_Base);
    agregar_letra(numeros, &Numero_6_Base);
    agregar_letra(numeros, &Numero_7_Base);
    agregar_letra(numeros, &Numero_8_Base);
    agregar_letra(numeros, &Numero_9_Base);

    // Dibujar los dibujables
    dibujarDibujable(hdc, naveMaxRotacion);
    dibujarDibujable(hdc, nave_propulsion_maxRotacion);

    dibujar_palabra(letras, hdc);
    dibujar_palabra(numeros, hdc);
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
