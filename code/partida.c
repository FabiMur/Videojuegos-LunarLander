#include "partida.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"
#include "../resources/asteroides.h"
#include "gestor_plataformas.h"
#include "variables_globales.h"
#include "gestor_colisiones.h"
#include "config.h"
#include "math.h"

#define fuel_por_moneda 500
#define masa_nave 1000

#define aterrizaje_perfecto_vel 0.5
#define aterrizaje_brusco_vel 1
#define aterrizaje_perfecto_rot 5
#define aterrizaje_brusco_rot 10

int inicio = 0;

struct objetoFisico* nave = NULL;
struct Dibujable* motor_debil = NULL;
struct Dibujable* motor_medio = NULL;
struct Dibujable* motor_fuerte = NULL;
struct Dibujable* terreno = NULL;
struct Asteroide* asteroides = NULL;
struct Plataforma* plataformas_partida = NULL;
uint16_t numero_plataformas = 0;
uint8_t numero_asteroides = 0;


int combustible = 0;
uint16_t puntuacion_partida = 0;
uint16_t fisicas = DESACTIVADAS;

int32_t offsetTerrenoDerecha = ANCHURA_TERRENO;
int32_t offsetTerrenoIzquerda = 0;

void escalar_escena_partida(float factor_x, float factor_y){
	if(inicio == 1) {
		escalar_dibujable_en_escena_dados_ejes(terreno, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_fuerte, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_medio, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_debil, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(nave->objeto, factor_x, factor_y);
		for(uint16_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].linea, factor_x, factor_y);
			for(uint16_t j = 0; j < plataformas_partida[i].texto->num_caracteres; j++){
				escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].texto->caracteres[j], factor_x, factor_y);
			}
		}
	}
}

uint16_t evaluar_aterrizaje(uint16_t bonificador, uint16_t es_arista_aterrizable){
	uint16_t puntuacion = 0;

	if(es_arista_aterrizable == 1){
		if(nave->velocidad[1] > -aterrizaje_perfecto_vel &&
			(aterrizaje_perfecto_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_perfecto_vel) &&
			(nave->rotacion < aterrizaje_perfecto_rot ||
			nave->rotacion > 360 - aterrizaje_perfecto_rot)) {
			// Aterrizaje perfecto
			printf("Aterrizaje perfecto\n");
			puntuacion = 50 * bonificador;
			combustible += 50;
		}
		else if(nave->velocidad[1] > -aterrizaje_brusco_vel &&
			(aterrizaje_brusco_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_brusco_vel) &&
			(nave->rotacion < aterrizaje_brusco_rot ||
			nave->rotacion > 360 - aterrizaje_brusco_rot)) {
			// Aterrizaje brusco
			printf("Aterrizaje brusco\n");
			puntuacion = 15 * bonificador;
		}
		else{
			// Colision
			printf("Colision\n");
			puntuacion = 5 * bonificador;
		}
	}
	else {
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
	struct Arista arista_colision = (struct Arista){0};
	uint16_t bonificador = 1;
	uint16_t es_arista_aterrizable = 0;
	uint16_t se_produce_colision = 0;

	// Comprobar colision con el terreno
	if(hay_colision(nave->objeto, terreno, &arista_colision)) {
		se_produce_colision = 1;
		es_arista_aterrizable = es_horizontal(arista_colision);
		if(es_arista_aterrizable == 1){

			// Si hay colision con el terreno -> evaluar si ha sido colision con plataforma
			for(uint16_t i = 0; i < numero_plataformas; i++) {
				if(hay_colision(nave->objeto, plataformas_partida[i].linea, &arista_colision)) {
					// La colision ha sido con una plataforma
					bonificador = plataformas_partida[i].bonificador;
					break;
				}
			}
		}
	}
	if(se_produce_colision == 1){
		// Determinar tipo de aterrizaje y puntos conseguidos
		uint16_t puntos_conseguidos = evaluar_aterrizaje(bonificador, es_arista_aterrizable);
		puntuacion_partida += puntos_conseguidos;
		printf("Has conseguido %d puntos en este aterrizaje\n", puntos_conseguidos);
		se_ha_aterrizado();
		finalizarPartida();		
	}
}

void dibujar_terreno(HDC hdc){
	dibujarDibujable(hdc, terreno);
	
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);	
	}
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){-ANCHURA_TERRENO,0});
	dibujarDibujable(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);	
	}
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){2*ANCHURA_TERRENO,0});
	dibujarDibujable(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);	
	}
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){-ANCHURA_TERRENO,0});
}

void gestionar_colisiones_asteroides() {
	struct Arista arista_colision = (struct Arista){0};
	uint8_t se_produce_colision = 0;

	// Comprobar colision con el terreno
	for(uint8_t i = 0; i < numero_asteroides; i++){
		if(hay_colision(nave->objeto, asteroides[i].objeto, &arista_colision)){
			se_produce_colision = 1;
			break;
		}
	}
	if(se_produce_colision == 1){
		printf("Has colisionado con un asteoride\n");
		se_ha_aterrizado();	
	}
}

void dibujar_escena(HDC hdc){
    dibujarDibujable(hdc, nave -> objeto);

	dibujar_terreno(hdc);

	for(uint8_t i = 0; i < numero_asteroides; i++){
		dibujarAsteroide(hdc, &asteroides[i]);
	}

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

void dibujarHUD(HDC hdc) {
    char buf[64];
    struct Punto origen;
    struct Texto* txt;
    int salto = ALTURA_CARACTER_MAX + 5;

    // SCORE
    sprintf(buf, "SCORE %04d", puntuacion_partida);
    origen = (struct Punto){ 10, 10 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // TIME
    extern int tiempo;  
    sprintf(buf, "TIME  %02d:%02d", tiempo/60, tiempo%60);
    origen = (struct Punto){ 10, 10 + salto * 1 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // FUEL
    sprintf(buf, "FUEL  %04d", combustible);
    origen = (struct Punto){ 10, 10 + salto * 2 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // ALTITUDE
    int alt = (int)(BASE_H - nave->objeto->origen.y);
    sprintf(buf, "ALTITUDE           %4d", alt);
    origen = (struct Punto){ 300, 10 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // HORIZONTAL SPEED
    sprintf(buf, "HORIZONTAL SPEEED  %4d>", (int)roundf(nave->velocidad[0]));
    origen = (struct Punto){ 300, 10 + salto * 1 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // VERTICAL SPEED
    sprintf(buf, "VERTICAL SPEED     %4d<", (int)roundf(-nave->velocidad[1]));
    origen = (struct Punto){ 300, 10 + salto * 2 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);
}

void rotar_nave(uint16_t direccion){
	rotarDibujable(nave -> objeto, direccion);
	rotarDibujable(motor_debil, direccion);
	rotarDibujable(motor_medio, direccion);
	rotarDibujable(motor_fuerte, direccion);
}

void manejar_instante_partida(){
    if(fisicas == ACTIVADAS) {
		calcularFisicas(nave);
		if(nave->objeto->origen.x > offsetTerrenoDerecha) {
			trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){ANCHURA_TERRENO,0});
			offsetTerrenoDerecha += ANCHURA_TERRENO;
			offsetTerrenoIzquerda += ANCHURA_TERRENO;
		}
		else if(nave->objeto->origen.x < offsetTerrenoIzquerda) {
			trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){-ANCHURA_TERRENO,0});
			offsetTerrenoDerecha -= ANCHURA_TERRENO;
			offsetTerrenoIzquerda -= ANCHURA_TERRENO;
		}
		calcualarFisicasAsteroides(asteroides, numero_asteroides);
		gestionar_colisiones_asteroides();
		gestionar_colisiones();
	}
}

void inicializarPartida(){
    combustible = 0;
	terreno = crearDibujable(&Terreno);
	
	plataformas_partida = generar_plataformas(&Terreno, &numero_plataformas);

	offsetTerrenoDerecha = ANCHURA_TERRENO;
	offsetTerrenoIzquerda = 0;

	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){0, 350});

	asteroides = generarAsteroides(terreno, &numero_asteroides);
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
	inicio = 1;
	printf("Combustible inicial: %d\n", combustible);
}

void finalizarPartida(){
    //destruirObjetoFisico(nave);
    fisicas = DESACTIVADAS;
	cambiar_estado(PEDIR);
}

void findeJuego(){
    cambiar_estado(PEDIR);
}