#include "partida.h"
#include "../resources/nave.h"

#define fuel_por_moneda 500
#define masa_nave 1000

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
struct Dibujable* motor_debil = NULL;
struct Dibujable* motor_medio = NULL;
struct Dibujable* motor_fuerte = NULL;

static int combustible = 0;
static uint8_t fisicas = DESACTIVADAS;

void dibujar_escena(HDC hdc){
    dibujarDibujable(hdc, nave -> objeto);

	switch(obtener_propulsor()){
		case 1:
			colocarDibujable(motor_debil, nave -> objeto -> origen);
			dibujarDibujable(hdc, motor_debil);
			break;
		case 2:
			colocarDibujable(motor_medio, nave -> objeto -> origen);
			dibujarDibujable(hdc, motor_medio);
			break;
		case 3:
			colocarDibujable(motor_fuerte, nave -> objeto -> origen);
			dibujarDibujable(hdc, motor_fuerte);
			break;
		default:
			break;
	}
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

	motor_debil = crearDibujable(&Nave_Propulsion_Minima);
	motor_medio = crearDibujable(&Nave_Propulsion_Media);
	motor_fuerte = crearDibujable(&Nave_Propulsion_Maxima);

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