#include "overlays.h"
#include "texto.h"
#include "config.h"
#include "../resources/caracteres.h"
#include <stdio.h>

static OpcionFin opcionFin = FIN_OPCION_INSERT_COIN;
static OpcionPausa opcionPausa = PAUSE_OPCION_CONTINUAR;

static const char* opcionesFin[FIN_NUM_OPCIONES] = { "INSERT COIN", "MENU" };
static const char* opcionesPausa[PAUSE_NUM_OPCIONES] = { "CONTINUE", "INSERT COIN", "MENU" };

void procesarEventoOverlayFin(UINT uMsg, WPARAM wParam) {
    if(uMsg==WM_KEYDOWN) {
        if(wParam==VK_UP && opcionFin>0) opcionFin--;
        else if(wParam==VK_DOWN && opcionFin<FIN_NUM_OPCIONES-1) opcionFin++;
    }
}

void procesarEventoOverlayPausa(UINT uMsg, WPARAM wParam) {
    if(uMsg==WM_KEYDOWN) {
        if(wParam==VK_UP && opcionPausa>0) opcionPausa--;
        else if(wParam==VK_DOWN && opcionPausa<PAUSE_NUM_OPCIONES-1) opcionPausa++;
    }
}

OpcionFin obtenerOpcionFin(void) { return opcionFin; }
OpcionPausa obtenerOpcionPausa(void) { return opcionPausa; }

void reiniciarOverlayFin(void) { opcionFin = FIN_OPCION_INSERT_COIN; }
void reiniciarOverlayPausa(void) { opcionPausa = PAUSE_OPCION_CONTINUAR; }

static void dibujarRecuadro(HDC hdc, int ancho, int alto, int* outX, int* outY) {
    int x = (BASE_W - ancho)/2;
    int y = (BASE_H - alto)/2;

    HBRUSH oldBrush = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
    HPEN oldPen = SelectObject(hdc, hPen);

    Rectangle(hdc, x, y, x+ancho, y+alto);

    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
    SelectObject(hdc, oldBrush);

    if(outX) *outX = x;
    if(outY) *outY = y;
}

void dibujarOverlayFin(HDC hdc, int tiempo, int puntuacion) {
    int x,y; const int w=400,h=220; dibujarRecuadro(hdc,w,h,&x,&y);
    int salto = ALTURA_CARACTER_MAX + 5;
    struct Texto* t; struct Punto o; char buf[64];

    o = (struct Punto){ x+20, y+20 };
    t = crearTextoDesdeCadena("GAME OVER", o); dibujar_texto(t,hdc); destruir_texto(t);

    sprintf(buf,"TIME %02d:%02d",tiempo/60,tiempo%60);
    o = (struct Punto){ x+20, y+20+salto };
    t = crearTextoDesdeCadena(buf,o); dibujar_texto(t,hdc); destruir_texto(t);

    sprintf(buf,"SCORE %04d",puntuacion);
    o = (struct Punto){ x+20, y+20+salto*2 };
    t = crearTextoDesdeCadena(buf,o); dibujar_texto(t,hdc); destruir_texto(t);

    for(int i=0;i<FIN_NUM_OPCIONES;i++) {
        o = (struct Punto){ x+60, y+20+salto*(4+i) };
        t = crearTextoDesdeCadena(opcionesFin[i], o);
        if(i==opcionFin) {
            struct Punto ind={ o.x-2*ANCHURA_CARACTER_MAX, o.y };
            struct Texto* ti=crearTextoDesdeCadena(">",ind); dibujar_texto(ti,hdc); destruir_texto(ti);
        }
        dibujar_texto(t,hdc); destruir_texto(t);
    }
}

void dibujarOverlayPausa(HDC hdc, int tiempo, int puntuacion) {
    int x,y; const int w=400,h=220; dibujarRecuadro(hdc,w,h,&x,&y);
    int salto = ALTURA_CARACTER_MAX + 5;
    struct Texto* t; struct Punto o; char buf[64];

    o = (struct Punto){ x+20, y+20 };
    t = crearTextoDesdeCadena("PAUSE", o); dibujar_texto(t,hdc); destruir_texto(t);

    sprintf(buf,"TIME %02d:%02d",tiempo/60,tiempo%60);
    o = (struct Punto){ x+20, y+20+salto };
    t = crearTextoDesdeCadena(buf,o); dibujar_texto(t,hdc); destruir_texto(t);

    sprintf(buf,"SCORE %04d",puntuacion);
    o = (struct Punto){ x+20, y+20+salto*2 };
    t = crearTextoDesdeCadena(buf,o); dibujar_texto(t,hdc); destruir_texto(t);

    for(int i=0;i<PAUSE_NUM_OPCIONES;i++) {
        o = (struct Punto){ x+60, y+20+salto*(4+i) };
        t = crearTextoDesdeCadena(opcionesPausa[i], o);
        if(i==opcionPausa) {
            struct Punto ind={ o.x-2*ANCHURA_CARACTER_MAX, o.y };
            struct Texto* ti=crearTextoDesdeCadena(">",ind); dibujar_texto(ti,hdc); destruir_texto(ti);
        }
        dibujar_texto(t,hdc); destruir_texto(t);
    }
}