#include "partida.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"
#include "gestor_plataformas.h"

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
struct Dibujable* terreno = NULL;
struct Plataforma* plataformas_partida = NULL;
uint8_t numero_plataformas = 0;

static int combustible = 0;
static uint8_t fisicas = DESACTIVADAS;

void dibujar_escena(HDC hdc){
    dibujarDibujable(hdc, nave -> objeto);
	dibujarDibujable(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);
	}

	switch(obtener_propulsor()){
		case 1:
			colocar_dibujable(motor_debil, nave -> objeto -> origen);
			dibujarDibujable(hdc, motor_debil);
			break;
		case 2:
			colocar_dibujable(motor_medio, nave -> objeto -> origen);
			dibujarDibujable(hdc, motor_medio);
			break;
		case 3:
			colocar_dibujable(motor_fuerte, nave -> objeto -> origen);
			dibujarDibujable(hdc, motor_fuerte);
			break;
		default:
			break;
	}
}

void rotar_nave(uint8_t direccion){
	rotarDibujable(nave -> objeto, direccion);
	rotarDibujable(motor_debil, direccion);
	rotarDibujable(motor_medio, direccion);
	rotarDibujable(motor_fuerte, direccion);
}

void manejar_instante_partida(){
    if(fisicas == ACTIVADAS) calcularFisicas(nave);
}

void inicializarPartida(){
    combustible = 0;
	terreno = crearDibujable(&Terreno);
	plataformas_partida = generar_plataformas(&Terreno, &numero_plataformas);
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
	nave -> rotacion = 0;
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