#include "partida.h"

#define fuel_por_moneda 500
#define masa_nave 1000

#include "../resources/nave.h"

/**
 * @brief Enumeración para el estado de las fisicas
 * 
 * DESACTIVADAS: 0
 * ACTIVADAS: 1
 */
enum fisicas {
    DESACTIVADAS,
    ACTIVADAS
};

struct objetoFisico* nave = NULL;
static uint8_t fisicas = DESACTIVADAS;

void dibujar_escena(HDC hdc){
    dibujarDibujable(hdc, nave -> objeto);
}

void manejar_instante_partida(){
    if(fisicas == ACTIVADAS) calcularFisicas(nave);
}

void inicializarPartida(){
    combustible = 0;
}

void anyadirMoneda(){
    combustible += fuel_por_moneda;
}

void comenzarPartida(){
    nave = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
    nave -> objeto = crearDibujable(&Nave_Base);
    nave -> velocidad[0] = 0;
    nave -> velocidad[1] = 0;
    nave -> aceleracion[0] = 0;
    nave -> aceleracion[1] = 0;
    nave -> masa = masa_nave;
    trasladarDibujable(nave -> objeto, (struct Punto){50, 50});
    fisicas = ACTIVADAS;
}

void finalizarPartida(){
    destruirObjetoFisico(nave);
    nave -> objeto = NULL;
    fisicas = DESACTIVADAS;
}

void findeJuego(){
    cambiar_estado(PEDIR);
}