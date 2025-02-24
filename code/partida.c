#include "partida.h"
#include "fisicas.h"

#define fuel_por_moneda 500

void dibujarEscena(HDC hdc){
    dibujarDibujable(hdc, nave);
}

void inicializarPartida(){
    combustible = 0;
}

void anyadirMoneda(){
    combustible += fuel_por_moneda;
}

void comenzarPartida(){
    nave = crearDibujable(&Nave_Base);
    trasladarDibujable(nave, (struct Punto){50, 50});
    fisicas = ACTIVADAS;
}

void finalizarPartida(){

}

void findeJuego(){
    cambiar_estado(PEDIR);
}