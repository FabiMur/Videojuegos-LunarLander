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

int tamano_pantalla_X = 1024;
int tamano_pantalla_Y = 768;

int posicion_x = 0;
int posicion_y = 0;
int tamx = 1024;
int tamy = 768;

float ratio_escena_x = 1024;
float ratio_escena_y = 768;

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

// Función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            SetTimer(hwnd, timer, intervalo_fisicas_ms, NULL);

            break;

        case WM_TIMER:
            if (wParam == timer) {
                manejar_instante();
                manejar_teclas();
                InvalidateRect(hwnd, NULL, FALSE); // Fuerza un repintado 
            }
        break;

        case WM_SYSCOMMAND: // Gestion maximizar y restaurar
            if((wParam & 0xFFF0) == SC_MAXIMIZE) {
                RECT rectangulo;
                GetWindowRect(hwnd, &rectangulo);
                posicion_x = rectangulo.left;
                posicion_y = rectangulo.top;
                tamx = rectangulo.right - rectangulo.left;
                tamy = rectangulo.bottom - rectangulo.top;

                SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE); // WS_POPUP: estilo de ventana sin bordes
                SetWindowPos(hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                            SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
            }
            else if((wParam & 0xFFF0) == SC_RESTORE && IsZoomed(hwnd)) {
                SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE); // WS_OVERLAPPEDWINDOW: estilo de la ventana estandar
                SetWindowPos(hwnd, NULL, posicion_x, posicion_y, tamx, tamy,
                            SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
            }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
        break;

        case WM_GETMINMAXINFO:{ // Gestion tamaño de la ventana
            if (IsZoomed(hwnd)) {  // Si está maximizado, ajustar tamaño
                MINMAXINFO* mmi = (MINMAXINFO*)lParam;
                mmi->ptMaxSize.x = GetSystemMetrics(SM_CXSCREEN);
                mmi->ptMaxSize.y = GetSystemMetrics(SM_CYSCREEN);
                mmi->ptMaxPosition.x = 0;
                mmi->ptMaxPosition.y = 0;
            }

            // Limitar tamaño de ventana minima (sin tener en cuenta bordes ni cabecera)
            MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;
            RECT rc = { 0, 0, anchura_minima_ventana, altura_minima_ventana };
            AdjustWindowRectEx(&rc, GetWindowLong(hwnd, GWL_STYLE), FALSE, GetWindowLong(hwnd, GWL_EXSTYLE));
        
            pMinMax->ptMinTrackSize.x = rc.right - rc.left;
            pMinMax->ptMinTrackSize.y = rc.bottom - rc.top;
            
        }
        break;
    
        case WM_SIZE:{
            // Cambio de tamaño de la pantalla
            int width = LOWORD(lParam);  // Nuevo ancho de la ventana
            int height = HIWORD(lParam); // Nueva altura de la ventana
            if ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)) {
                // Calcular los factores de escala para X e Y
                float factor_resized_X = (float)width / tamano_pantalla_X;
                float factor_resized_Y = (float)height / tamano_pantalla_Y;
        
                // Mantener la proporción correcta de la escena
                float factor_resized = 1.0f;
                
                // Decidir qué factor de escala utilizar según las dimensiones de la ventana
                /*
                if (width < ratio_escena_x && height < ratio_escena_y) {
                    // Elige el menor de los dos factores de escala para evitar que se corte la escena
                    factor_resized = factor_resized_X < factor_resized_Y ? factor_resized_X : factor_resized_Y;
                } else if (width < ratio_escena_x) {
                    factor_resized = factor_resized_X;
                    ratio_escena_x = width / factor_resized; 
                } else if (height < ratio_escena_y) {
                    factor_resized = factor_resized_Y;
                    ratio_escena_y = height / factor_resized; 
                } else if(width > ratio_escena_x && width > tamano_pantalla_X && ratio_escena_y < height){
                    printf("width > ratio_x\n");
                    factor_resized = factor_resized_X;
                }
                */

                if (width < ratio_escena_x) {
                    factor_resized = factor_resized_X;
                    //ratio_escena_x = width / factor_resized; 
                } else if (height < ratio_escena_y) {
                    factor_resized = factor_resized_Y;
                    //ratio_escena_y = height / factor_resized; 
                } else if(width > ratio_escena_x && width > tamano_pantalla_X && ratio_escena_y < height){
                    factor_resized = factor_resized_X;
                }
                else if(height > ratio_escena_y && height > tamano_pantalla_Y && ratio_escena_x < width){
                    factor_resized = factor_resized_Y;
                }

                // Solo aplicar el escalado si el factor ha cambiado
                if (factor_resized != 1.0f) {
                    // Escalar la escena según el factor de escala determinado
                    escalar_escena(factor_resized, factor_resized);
                    ratio_escena_x = ratio_escena_x * factor_resized;
                    ratio_escena_y =  ratio_escena_y * factor_resized;
                    printf("ratio_x = %d, width = %d, tam_pantalla_x = %d\n", ratio_escena_x, width, tamano_pantalla_X);
                }
            }
            tamano_pantalla_X = width;
            tamano_pantalla_Y = height;
            
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
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
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

    HWND hwnd = CreateWindowEx(0, "RasterWindow", "Lunar Lander",
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
