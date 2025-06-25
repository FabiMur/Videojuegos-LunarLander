#include "code/lunar_lander.h"
#include "code/texto.h"
#include "resources/superficie_lunar.h"
#include "code/gestor_plataformas.h"
#include "code/variables_globales.h"
#include "code/menu.h"
#include "code/opciones.h"
#include "code/config.h"
#include "resources/asteroides.h"
#include "code/partida.h"
#include "code/overlays.h"
#include "code/ai.h"
#include "code/sonidos.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define TIMER_ID 1

// Esquinas de la letterbox
static struct Punto borde_puntos[4] = {
    {0.0f,        1.0f},
    {BASE_W-1.0f, 1.0f},
    {BASE_W-1.0f, BASE_H-1.0f},
    {0.0f,        BASE_H-1.0f}
};

// Aristas de la letterbox
static struct UnionAristas borde_aristas[4] = {{0,1},{1,2},{2,3},{3,0}};

// Constante para el borde de la letterbox
static const struct DibujableConstante borde_const = {{0,0}, borde_puntos, borde_aristas, 4, 4};

// Borde de la letterbox
static struct Dibujable* dib_borde = NULL;

// Margen de la camara para el seguimiento horizontal
#define CAMARA_MARGEN 200

// Desplazamiento actual de la camara en el eje X
static float camaraX = 0.0f;

// Constantes de la letterbox
typedef struct { int internalW, internalH, offsetX, offsetY; } Letterbox;
static Letterbox lb = { BASE_W, BASE_H, 0, 0 };

// Constantes de la aplicación
typedef enum { ESTADO_MENU, ESTADO_JUEGO, ESTADO_OPTIONS, ESTADO_PAUSA, ESTADO_FIN } EstadoAplicacion;
static EstadoAplicacion estadoActual = ESTADO_MENU;

// Calcula la coordenada Y del terreno para una posición X concreta
static float obtener_y_terreno(float x) {
    if(!terreno || !terreno->puntos) return (float)BASE_H;

    for(uint16_t i = 0; i < terreno->num_puntos - 1; i++) {
        float x1 = terreno->puntos[i].x;
        float x2 = terreno->puntos[i+1].x;
        if((x >= x1 && x <= x2) || (x >= x2 && x <= x1)) {
            float t = (x2 - x1) != 0 ? (x - x1) / (x2 - x1) : 0;
            return terreno->puntos[i].y + t * (terreno->puntos[i+1].y - terreno->puntos[i].y);
        }
    }

    return (float)BASE_H;
}

// Variables globales
int tiempo = 0;
int tiempo_ms = 0;
uint16_t fullscreen = 0, esc_presionado = 0;
RECT rectVentanaAnterior;

void informarFinPartida(void) {
    reiniciarOverlayFin();
    estadoActual = ESTADO_FIN;
}

// Inicializa la consola para imprimir mensajes de depuración
void AttachConsoleToStdout() {
    AllocConsole();
    freopen("CONOUT$","w",stdout);
    freopen("CONOUT$","w",stderr);
}


// Fucnion para probar dibujables
void pruebasDibujables(HDC hdc) {
    const char* abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    struct Punto o1 = {10,10};
    struct Texto* t1 = crearTextoDesdeCadena(abc,o1);
    dibujar_texto(t1,hdc); destruir_texto(t1);
    const char* nums = "0123456789 :><";
    struct Punto o2 = {10, 10 + ALTURA_CARACTER_MAX + 5};
    struct Texto* t2 = crearTextoDesdeCadena(nums,o2);
    dibujar_texto(t2,hdc); destruir_texto(t2);
}

// Función para calcular el tamaño y la posición de la letterbox
// en función del tamaño de la ventana y la resolución base
void calcular_letterbox(HWND hwnd) {
    RECT rc; GetClientRect(hwnd,&rc);
    int w = rc.right-rc.left, h = rc.bottom-rc.top;
    float fx = (float)w/BASE_W, fy = (float)h/BASE_H;
    float f = fx<fy?fx:fy;
    lb.internalW = (int)(BASE_W*f);
    lb.internalH = (int)(BASE_H*f);
    lb.offsetX   = (w - lb.internalW)/2;
    lb.offsetY   = (h - lb.internalH)/2;
}

// Función para dibujar el frame completo
static void DibujaFrame(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdcWin = BeginPaint(hwnd,&ps);

    // Full client buffer
    RECT rcC; GetClientRect(hwnd,&rcC);
    int Wc = rcC.right-rcC.left, Hc = rcC.bottom-rcC.top;
    HDC  hdcAll = CreateCompatibleDC(hdcWin);
    HBITMAP hbmAll = CreateCompatibleBitmap(hdcWin,Wc,Hc);
    HGDIOBJ oldAll = SelectObject(hdcAll,hbmAll);
    
    // Clear full buffer
    HBRUSH brA = CreateSolidBrush(RGB(0,0,0));
    FillRect(hdcAll,&rcC,brA);
    DeleteObject(brA);

    // Internal backbuffer BASE_W x BASE_H
    HDC  hdcBase = CreateCompatibleDC(hdcAll);
    HBITMAP hbmBase = CreateCompatibleBitmap(hdcAll,BASE_W,BASE_H);
    HGDIOBJ oldBase = SelectObject(hdcBase,hbmBase);
    
    // Clear internal
    HBRUSH brB = CreateSolidBrush(RGB(0,0,0));
    RECT rB = {0,0,BASE_W,BASE_H};
    FillRect(hdcBase,&rB,brB);
    DeleteObject(brB);

    // Draw scene
    if (estadoActual == ESTADO_MENU) {
        dibujarMenuEnBuffer(hdcBase);
    } else if (estadoActual == ESTADO_JUEGO || estadoActual == ESTADO_PAUSA || estadoActual == ESTADO_FIN) {
        float cx = BASE_W/2.0f, cy = BASE_H/2.0f;
        float naveX_en_pantalla = nave->objeto->origen.x + camaraX;
        if(naveX_en_pantalla < CAMARA_MARGEN)
            camaraX = CAMARA_MARGEN - nave->objeto->origen.x;
        else if(naveX_en_pantalla > BASE_W - CAMARA_MARGEN)
            camaraX = BASE_W - CAMARA_MARGEN - nave->objeto->origen.x;

        float alt = obtener_y_terreno(nave->objeto->origen.x) - nave->objeto->origen.y;
        const float ALT_ZOOM_INI = 200.0f;
        const float ALT_ZOOM_MIN = 50.0f;
        const float ZOOM_DEFAULT = 0.8f;
        float zoom = ZOOM_DEFAULT;
        if (alt < ALT_ZOOM_INI) {
            float a = alt < ALT_ZOOM_MIN ? ALT_ZOOM_MIN : alt;
            zoom = ZOOM_DEFAULT + (ALT_ZOOM_INI - a) / (ALT_ZOOM_INI - ALT_ZOOM_MIN);
        }

        XFORM xf = { zoom, 0, 0, zoom, (1.0f - zoom) * cx, (1.0f - zoom) * cy };
        SetGraphicsMode(hdcBase, GM_ADVANCED);
        SetWorldTransform(hdcBase, &xf);

        float camX_zoom = camaraX + (1.0f - zoom) * (nave->objeto->origen.x - cx);
        float camY_zoom = (cy - nave->objeto->origen.y) * zoom;
        SetViewportOrgEx(hdcBase,(int)camX_zoom,(int)camY_zoom,NULL);

        dibujar_escena(hdcBase);

        SetViewportOrgEx(hdcBase,0,0,NULL);

        {
            XFORM id = {1,0,0,1,0,0};
            SetWorldTransform(hdcBase,&id);
            SetGraphicsMode(hdcBase, GM_COMPATIBLE);
        }

        if (estadoActual == ESTADO_PAUSA) {
            dibujarOverlayPausa(hdcBase, tiempo, puntuacion_partida);
        } else if (estadoActual == ESTADO_FIN) {
            dibujarOverlayFin(hdcBase, tiempo, puntuacion_partida);
        }

        dibujarHUD(hdcBase);
    } else if (estadoActual == ESTADO_OPTIONS) {
        dibujarOpcionesEnBuffer(hdcBase);
    }

    // Draw border inside internal
    if (dib_borde) dibujarDibujable(hdcBase,dib_borde);

    // Blit internal into full with letterbox
    StretchBlt(hdcAll,
               lb.offsetX, lb.offsetY,
               lb.internalW, lb.internalH,
               hdcBase,0,0,BASE_W,BASE_H,
               SRCCOPY);

    // Release internal
    SelectObject(hdcBase,oldBase);
    DeleteObject(hbmBase);
    DeleteDC(hdcBase);

    // Single blit to screen
    BitBlt(hdcWin,0,0,Wc,Hc,hdcAll,0,0,SRCCOPY);

    // Release full
    SelectObject(hdcAll,oldAll);
    DeleteObject(hbmAll);
    DeleteDC(hdcAll);

    EndPaint(hwnd,&ps);
}

// Función de ventana principal
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch(uMsg) {
    // Evento de creación de la ventana
    case WM_CREATE:
        AttachConsoleToStdout();
        Sound_Init();
        SetTimer(hwnd,TIMER_ID,intervalo_fisicas_ms,NULL);
        inicializarMenu();
        inicializarOpciones();
        inicializar_aleatoriedad();
        calcular_letterbox(hwnd);
        dib_borde = crearDibujable(&borde_const);
        break;

    // Evento de redimensionado de la ventana
    case WM_SIZE:
        // En caso de que la ventana cambie de tamaño, recalcular el letterbox
        calcular_letterbox(hwnd);
        break;

    // Evento de comando del sistema
    case WM_SYSCOMMAND:
        // Si se presiona ESC y la ventana está en modo pantalla completa, volver a modo ventana
        if ((wParam&0xFFF0)==SC_RESTORE && fullscreen && esc_presionado) {
            fullscreen=0;
            SetWindowLong(hwnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd,NULL,
                         rectVentanaAnterior.left,rectVentanaAnterior.top,
                         rectVentanaAnterior.right-rectVentanaAnterior.left,
                         rectVentanaAnterior.bottom-rectVentanaAnterior.top,
                         SWP_NOZORDER|SWP_FRAMECHANGED);
            calcular_letterbox(hwnd);
            InvalidateRect(hwnd,NULL,TRUE);
            esc_presionado=0;

        // Si se presiona el botón de maximizar, cambiar a modo pantalla completa
        } else if ((wParam&0xFFF0)==SC_MAXIMIZE) {
            fullscreen=1;
            GetWindowRect(hwnd,&rectVentanaAnterior);
            SetWindowLong(hwnd,GWL_STYLE,WS_POPUP|WS_VISIBLE);
            SetWindowPos(hwnd,NULL,0,0,
                         GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),
                         SWP_NOZORDER|SWP_FRAMECHANGED);
            calcular_letterbox(hwnd);
        }
        break;
    
    case WM_KEYDOWN: {
        if(estadoActual == ESTADO_MENU) {
            if(wParam == VK_UP || wParam == VK_DOWN) {
                // Cambia la opción seleccionada
                Sound_Play(SONIDO_CAMBIAR_OPCION_MENU);
            }
            procesarEventoMenu(hwnd, uMsg, wParam, lParam);
            if(wParam == VK_RETURN) {
                Sound_Play(SONIDO_SELECCIONAR_OPCION_MENU);
                OpcionMenu op = obtenerOpcionSeleccionada();
                if (op==OPCION_PLAY) {
                    estadoActual = ESTADO_JUEGO;
                    inicializarPartida();
                    comenzarPartida();
                    if(obtenerValorFlag(FLAG_AI)) {
                        anyadirMoneda();
                        anyadirMoneda();
                        anyadirMoneda();
                        cambiar_estado(JUGANDO);
                    } else {
                        cambiar_estado(PEDIR);
                    }
                    } else if (op==OPCION_OPTIONS) {
                    estadoActual = ESTADO_OPTIONS;
                } else if (op==OPCION_EXIT) {
                    PostQuitMessage(0);
                }
            }
        } else if (estadoActual==ESTADO_JUEGO) {
            if (wParam==VK_ESCAPE) {
                reiniciarOverlayPausa();
                estadoActual = ESTADO_PAUSA;
                fisicas = DESACTIVADAS;
                break;
            }
            if (GetAsyncKeyState(VK_UP) & 0x8000) pulsar_tecla(ARRIBA);
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) pulsar_tecla(IZQUIERDA);
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pulsar_tecla(DERECHA);
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) pulsar_tecla(ESPACIO);
            if (GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) pulsar_tecla(MONEDA);
        } else if (estadoActual==ESTADO_PAUSA) {
            procesarEventoOverlayPausa(uMsg,wParam);
            if (wParam==VK_RETURN) {
                switch(obtenerOpcionPausa()) {
                    case PAUSE_OPCION_CONTINUAR:
                        estadoActual = ESTADO_JUEGO;
                        fisicas = ACTIVADAS;
                        break;
                    case PAUSE_OPCION_INSERT_COIN:
                        anyadirMoneda();
                        break;
                    case PAUSE_OPCION_MENU:
                        estadoActual = ESTADO_MENU;
                        findeJuego();
                        break;
                    default:
                        break;
                }
            }
        } else if (estadoActual==ESTADO_FIN) {
            procesarEventoOverlayFin(uMsg,wParam);
            if (wParam==VK_RETURN) {
                switch(obtenerOpcionFin()) {
                    case FIN_OPCION_INSERT_COIN:
                        inicializarPartida();
                        anyadirMoneda();
                        continuarPartida();
                        estadoActual = ESTADO_JUEGO;
                        break;
                    case FIN_OPCION_MENU:
                        estadoActual = ESTADO_MENU;
                        findeJuego();
                        break;
                    default:
                        break;
                }
            }
        } else if (estadoActual==ESTADO_OPTIONS) {
            procesarEventoOpciones(hwnd,uMsg,wParam,lParam);
            if (wParam==VK_RETURN && obtenerOpcionSeleccionadaOpc() == NUM_FLAGS) {
                estadoActual = ESTADO_MENU;
            } else if(wParam==VK_ESCAPE) {
                estadoActual = ESTADO_MENU;
            }
        }
        break;
    }
    // Evento de tecla levantada
    case WM_KEYUP: {
        if (estadoActual == ESTADO_JUEGO) {
            if (!(GetAsyncKeyState(VK_UP) & 0x8000)) levantar_tecla(ARRIBA);
            if (!(GetAsyncKeyState(VK_LEFT) & 0x8000)) levantar_tecla(IZQUIERDA);
            if (!(GetAsyncKeyState(VK_RIGHT) & 0x8000)) levantar_tecla(DERECHA);
            if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) levantar_tecla(ESPACIO);
            if (!(GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000)) levantar_tecla(MONEDA);
        }
        break;
    }

    // Evento de temporizador
    case WM_TIMER:
        if (wParam==TIMER_ID) {
            if (estadoActual == ESTADO_JUEGO || estadoActual == ESTADO_PAUSA) {
                manejar_instante();
                ai_actualizar();
                manejar_teclas();
            }
            InvalidateRect(hwnd,NULL,FALSE);
        }
        break;

    // Evento de repintado
    case WM_PAINT:
        DibujaFrame(hwnd);
        break;
    
    // Evento de cierre de ventana
    case WM_DESTROY:
        if (dib_borde) destruirDibujable(dib_borde);
        Sound_Cleanup();
        destruirOpciones();
        KillTimer(hwnd,TIMER_ID);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// Función principal de la aplicación
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)lpCmdLine;
    (void)hPrevInstance;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RasterWindow";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);

    RECT rc = {0, 0, BASE_W, BASE_H};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hwnd = CreateWindow("RasterWindow", "Lunar Lander",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             rc.right - rc.left, rc.bottom - rc.top,
                             NULL, NULL, hInstance, NULL);
    if(!hwnd) return 0;
    ShowWindow(hwnd,nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}