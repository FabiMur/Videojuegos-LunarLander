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
#define anchura_minima_ventana 512
#define altura_minima_ventana 384

// Factor por el que escalar la escena
float factor_escalado = 1.0f; 

// 1 si la ventana esta en fullcreen, 0 si no (sin bordes ni cabecera)
uint8_t fullscreen = 0;
uint8_t esc_presionado = 0;

// Rectangulo que contiene la ventana anterior al resize de pantalla completa
RECT rectVentanaAnterior;

struct tamano_escena{
    int ancho;
    int alto;
};

// Anchura y altura de la escena 
struct tamano_escena tam_escena = {1024, 768};


float minimo(float a, float b) {
    return (a < b) ? a : b;
}

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
    struct Dibujable* dibu = crearDibujable(&Letra_A_Base);
    colocar_dibujable(dibu, (struct Punto){0, 384});
    dibujarDibujable(hdc, dibu);
}


void get_tamano_ventana(HWND hwnd, int* ancho, int* alto) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    *ancho = rect.right - rect.left;
    *alto = rect.bottom - rect.top;
}

void get_tamano_area_cliente(HWND hwnd, int* ancho, int* alto){
    RECT rect;
    GetClientRect(hwnd, &rect);
    *ancho = rect.right - rect.left;
    *alto = rect.bottom - rect.top;
}


// Se llamará a esta funcion cuando la ventana pase de fullscreen a restaurada
void calcular_escalado_restore(HWND hwnd) {
    int ancho_cliente = 0;
    int alto_cliente = 0;
    get_tamano_area_cliente(hwnd, &ancho_cliente, &alto_cliente);
    float factor_resized_x = (float)ancho_cliente / tamano_inicial_pantalla_X;
    float factor_resized_y = (float)alto_cliente / tamano_inicial_pantalla_Y;

    factor_escalado = minimo(factor_resized_x, factor_resized_y);

    // Como no se ha actualizado el tamano escena, su valor es el de la fullscreen
    tam_escena.ancho = (int)(tamano_inicial_pantalla_X * factor_escalado);
    tam_escena.alto = (int)(tamano_inicial_pantalla_Y * factor_escalado);

    escalar_escena(factor_escalado, factor_escalado);
}

// Función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:{
            SetTimer(hwnd, timer, intervalo_fisicas_ms, NULL);
        }
        break;

        case WM_SYSCOMMAND: {
            // Una ventana recibe este mensaje cuando el usuario elige un comando
            // en el menú Ventana (anteriormente conocido como el menú del sistema
            // o control) o cuando el usuario elige el botón maximizar, minimizar
            // el botón, restaurar botón o cerrar.
            // printf("El usuario ha elegido un comando del menu ventana\n");
            switch(wParam & 0xFFF0) {
                case SC_SIZE: {
                    // printf("WS_COMMAND - size\n");
                }
                break;
                case SC_RESTORE: {
                    // printf("WS_COMMAND - restore\n");
                    printf("WS_COMMAND - RESTORE: fullscreen = %d, esc_pres = %d\n", fullscreen, esc_presionado);
                    if(fullscreen == 1 && esc_presionado == 1) {
                        // Si se ha pulsado ESC y se estaba en fullscreen -> se restaura la ventana
                        fullscreen = 0;
                        // Restaurar estilo de ventana
                        SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

                        // Aplicar nuevo tamaño y posición (rectVentanaAnterior)
                        SetWindowPos(
                            hwnd, NULL,
                            rectVentanaAnterior.left,
                            rectVentanaAnterior.top,
                            rectVentanaAnterior.right - rectVentanaAnterior.left,
                            rectVentanaAnterior.bottom - rectVentanaAnterior.top,
                            SWP_NOZORDER | SWP_FRAMECHANGED // Necesario para aplicar los bordes
                        );

                    } else if (esc_presionado == 1){
                        // Si se ha pulsado ESC pero no se estaba en fullscreen -> no se propaga el restore
                        esc_presionado = 0;
                        return 0;
                    }
                    esc_presionado = 0;
                }
                break;
                case SC_MOVE: {
                    // printf("WS_COMMAND - move\n");
                }
                break;
                case SC_MINIMIZE: {
                    // printf("WS_COMMAND - minimize\n");
                }
                break;
                case SC_MAXIMIZE: {
                    printf("WS_COMMAND - maximize\n");
                    fullscreen = 1;
                    GetWindowRect(hwnd, &rectVentanaAnterior);
                    // Escalar y quitar bordes y cabecera
                    SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                }
                break;
                case SC_CLOSE: {
                    // printf("WS_COMMAND - close\n");
                }
                break;
            }
        }
        break;
        case WM_GETMINMAXINFO: {
            // El tamaño o la posicion de la ventana estan a punto de cambiar
            // printf("El tamano o la posicion de la ventana estan a punto de cambiar\n");

            // TAMAÑO VENTANA MINIMO -------------------------------------------
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            RECT rc = {0, 0, anchura_minima_ventana, altura_minima_ventana};
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
            mmi->ptMinTrackSize.x = rc.right - rc.left;
            mmi->ptMinTrackSize.y = rc.bottom - rc.top;
        }
        break;
        case WM_SIZING: {
            // El usuario esta redimensionando la ventana
            // printf("El usuario esta redimensionando la ventana\n");
        }
        break;

        case WM_SIZE: {
            // La ventana ha cambiado de tamaño
            // printf("Modificado el tamano de la ventana\n");
        }
        break;

        case WM_TIMER:{
            if (wParam == timer) {
                manejar_instante();
                manejar_teclas();
                InvalidateRect(hwnd, NULL, FALSE); // Fuerza un repintado 
            }
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
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                esc_presionado = 1;
                SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            }
            if (GetAsyncKeyState(VK_UP) & 0x8000) pulsar_tecla(ARRIBA);
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) pulsar_tecla(IZQUIERDA);
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pulsar_tecla(DERECHA);
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) pulsar_tecla(ESPACIO);
            if (GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) pulsar_tecla(MONEDA);
        }
		break;

        case WM_KEYUP: {
            if (!(GetAsyncKeyState(VK_UP) & 0x8000)) levantar_tecla(ARRIBA);
            if (!(GetAsyncKeyState(VK_LEFT) & 0x8000)) levantar_tecla(IZQUIERDA);
            if (!(GetAsyncKeyState(VK_RIGHT) & 0x8000)) levantar_tecla(DERECHA);
            if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) levantar_tecla(ESPACIO);
            if (!(GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000)) levantar_tecla(MONEDA);
        }
		break;
        
        case WM_DESTROY:{
            KillTimer(hwnd, timer);
            PostQuitMessage(0);
        }
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

    // Creacion de la ventana
    RECT rc = {0, 0, tamano_inicial_pantalla_X, tamano_inicial_pantalla_Y};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hwnd = CreateWindowEx(0, "RasterWindow", "Lunar Lander",
                            WS_OVERLAPPEDWINDOW, 0, 0,
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
