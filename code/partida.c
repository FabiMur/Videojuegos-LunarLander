#include "partida.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"
#include "gestor_plataformas.h"
#include "variables_globales.h"
#include "gestor_colisiones.h"

#define fuel_por_moneda 500
#define masa_nave 1000

#define aterrizaje_perfecto 0.5
#define aterrizaje_brusco 1

int inicio = 1;

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

int combustible = 0;
uint16_t puntuacion_partida = 0;
static uint8_t fisicas = DESACTIVADAS;


void escalar_escena(){
	escalarDibujableDadosEjes(terreno, factor_resized_X, factor_resized_Y);
	escalarDibujableDadosEjes(motor_fuerte, factor_resized_X, factor_resized_Y);
	escalarDibujableDadosEjes(motor_medio, factor_resized_X, factor_resized_Y);
	escalarDibujableDadosEjes(motor_debil, factor_resized_X, factor_resized_Y);
	escalarDibujableDadosEjes(nave->objeto, factor_resized_X, factor_resized_Y);
	for(uint8_t i = 0; i < numero_plataformas; i++) {
		escalar_plataforma_dados_ejes(&plataformas_partida[i], factor_resized_X, factor_resized_Y);
	}
	factor_resized_X = 1.0;
	factor_resized_Y = 1.0;
}

uint16_t evaluar_aterrizaje(uint8_t bonificador){
	uint16_t puntuacion = 0;
	printf("velocidad en x: %f\n", nave->velocidad[0]);
	printf("velocidad en y: %f\n\n", nave->velocidad[1]);

	if(nave->velocidad[1] > -aterrizaje_perfecto && (aterrizaje_perfecto > nave->velocidad[0] && nave->velocidad[0] > -aterrizaje_perfecto)) {
		// Aterrizaje perfecto
		printf("Aterrizaje perfecto\n");
		puntuacion = 50 * bonificador;
		combustible += 50;
	}
	else if(nave->velocidad[1] > -aterrizaje_brusco && (aterrizaje_brusco > nave->velocidad[0] && nave->velocidad[0] > -aterrizaje_brusco)) {
		// Aterrizaje brusco
		printf("Aterrizaje brusco\n");
		puntuacion = 15 * bonificador;
	}
	else{
		// Colision
		printf("Colision\n");
		puntuacion = 5 * bonificador;
	}
	return puntuacion;
}

void se_ha_aterrizado(){
	nave->velocidad[0] = 0;
	nave->velocidad[1] = 0;
	nave->aceleracion[0] = 0;
	nave->aceleracion[1] = 0;
	fisicas = DESACTIVADAS;
	printf("Combustible restante: %d\n", combustible);
}

void gestionar_colisiones() {
	if(hay_colision(nave->objeto, terreno)){ // Comprobar colision con el terreno
		uint8_t bonificador = 1;
		// Si hay colision con el terreno -> evaluar si ha sido colision con plataforma
		for(uint8_t i = 0; i < numero_plataformas; i++) {
			if(hay_colision(nave->objeto, plataformas_partida[i].linea)) {
				// La colision ha sido con una plataforma
				bonificador = plataformas_partida[i].bonificador;
				break;
			}
		}
		// Determinar tipo de aterrizaje y puntos conseguidos
		uint16_t puntos_conseguidos = evaluar_aterrizaje(bonificador);
		puntuacion_partida += puntos_conseguidos;
		printf("Has conseguido %d puntos en este aterrizaje\n", puntos_conseguidos);
		se_ha_aterrizado();
	}
}


void dibujar_escena(HDC hdc){
	escalar_escena();
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
    if(fisicas == ACTIVADAS) {
		calcularFisicas(nave);
		gestionar_colisiones();
	}
}

void inicializarPartida(){
    combustible = 0;
	terreno = crearDibujable(&Terreno);
	plataformas_partida = generar_plataformas(&Terreno, &numero_plataformas);
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){0, 350});
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
	printf("Combustible inicial: %d\n", combustible);
}

void finalizarPartida(){
    destruirObjetoFisico(nave);
    nave -> objeto = NULL;
    fisicas = DESACTIVADAS;
}

void findeJuego(){
    cambiar_estado(PEDIR);
}