#include "partida.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"
#include "gestor_plataformas.h"
#include "variables_globales.h"
#include "gestor_colisiones.h"
#include "fisicas.h"
#include <math.h>
#include <time.h>
#include <float.h>

#define fuel_por_moneda 500
#define masa_nave 1000

#define aterrizaje_perfecto_vel 0.5
#define aterrizaje_brusco_vel 1
#define aterrizaje_perfecto_rot 5
#define aterrizaje_brusco_rot 10

int inicio = 0;

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
static uint8_t zoom_escalado_aplicado = 0;
int es_dibujando_nave = 0; 
float pos_x_deseada_pantalla = 1024 / 2;
float pos_y_deseada_pantalla = 768 * 0.25;

static uint8_t fisicas = DESACTIVADAS;


void escalar_escena_partida(float factor_x, float factor_y){
	if(inicio == 1) {
		escalar_dibujable_en_escena_dados_ejes(terreno, factor_x, factor_x);
		escalar_dibujable_en_escena_dados_ejes(motor_fuerte, factor_y, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_medio, factor_y, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_debil, factor_y, factor_y);
		escalar_dibujable_en_escena_dados_ejes(nave->objeto, factor_y, factor_y);
		for(uint8_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].linea, factor_x, factor_x);
			for(uint8_t j = 0; j < plataformas_partida[i].palabra->num_letras; j++){
				escalar_dibujable_en_escena_dados_ejes(&plataformas_partida[i].palabra->letras[j], factor_x, factor_x);
			}
		}
		/*struct Punto nave_despues = nave->objeto->origen;

		float destino_x = 1024 / 2.0;
		float destino_y = 768 * 0.2;

		float dx = destino_x - nave_despues.x;
		float dy = destino_y - nave_despues.y;

		trasladarDibujable(terreno, (struct Punto){dx, dy});
		trasladarDibujable(motor_fuerte, (struct Punto){dx, dy});
		trasladarDibujable(motor_medio, (struct Punto){dx, dy});
		trasladarDibujable(motor_debil, (struct Punto){dx, dy});
		trasladarDibujable(nave->objeto, (struct Punto){dx, dy});

		for (uint8_t i = 0; i < numero_plataformas; i++) {
			trasladarDibujable(plataformas_partida[i].linea, (struct Punto){dx, dy});
			for (uint8_t j = 0; j < plataformas_partida[i].palabra->num_letras; j++) {
				trasladarDibujable(&plataformas_partida[i].palabra->letras[j], (struct Punto){dx, dy});
			}
		}*/
	}
}

uint16_t evaluar_aterrizaje(uint8_t bonificador, uint8_t es_arista_aterrizable){
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
	uint8_t bonificador = 1;
	uint8_t es_arista_aterrizable = 0;
	uint8_t se_produce_colision = 0;

	// Comprobar colision con el terreno
	if(hay_colision(nave->objeto, terreno, &arista_colision)){
		se_produce_colision = 1;
		es_arista_aterrizable = es_horizontal(arista_colision);
		if(es_arista_aterrizable == 1){

			// Si hay colision con el terreno -> evaluar si ha sido colision con plataforma
			for(uint8_t i = 0; i < numero_plataformas; i++) {
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
			
	}
}


void dibujar_escena(HDC hdc){
	es_dibujando_nave = 0;
	dibujarDibujable(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
        dibujar_plataforma(hdc, plataformas_partida[i]);
	}
	es_dibujando_nave = 1;
    dibujarDibujable(hdc, nave->objeto);
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
	es_dibujando_nave =0;
}


void actualizar_camara() {

    const float UMBRAL_DISTANCIA = 200.0f;
    const float MARGEN_HISTERESIS = 400.0f;

    float x_nave = nave->objeto->origen.x;
    float y_nave = nave->objeto->origen.y;
    float altura_terreno = obtener_altura_terreno_cercano(x_nave);
    float distancia_al_terreno = altura_terreno - y_nave;

    if (distancia_al_terreno < UMBRAL_DISTANCIA && !zoom_activado) {
        // Activar zoom
        zoom_activado = 1;
        camara_zoom = FACTOR_ZOOM;

        if (!zoom_escalado_aplicado) {
            zoom_escalado_aplicado = 1;
            escalar_escena_partida(FACTOR_ZOOM, ESCALA_NAVE_ZOOM);
        }

    } else if (distancia_al_terreno > (UMBRAL_DISTANCIA + MARGEN_HISTERESIS) && zoom_activado) {
        // Desactivar zoom
        camara_zoom = 1.0f;
        zoom_activado = 0;
        if (zoom_escalado_aplicado) {
            zoom_escalado_aplicado = 0;
            escalar_escena_partida(1.0f / FACTOR_ZOOM, 1.0f / ESCALA_NAVE_ZOOM);
        }
    }
}

// Función para obtener la altura del terreno más cercano debajo de la nave
float obtener_altura_terreno_cercano(float x_nave) {
    float altura_max = 0.0f; // Buscamos el punto más "bajo" (Y mayor) debajo de la nave
    
    if(!terreno || !terreno->aristas) return 764.0f; // Si no hay terreno, devolver fondo de pantalla

    for(uint16_t i = 0; i < terreno->num_aristas; i++) {
        struct Punto p1 = *terreno->aristas[i].origen;
        struct Punto p2 = *terreno->aristas[i].destino;
        
        // Considerar cualquier arista (no solo horizontales)
        float x_min = fmin(p1.x, p2.x);
        float x_max = fmax(p1.x, p2.x);
        
        if(x_nave >= x_min && x_nave <= x_max) {
            // Calcular Y en este punto X (interpolación lineal)
            float pendiente = (p2.y - p1.y) / (p2.x - p1.x);
            float y = p1.y + pendiente * (x_nave - p1.x);
            
            if(y > altura_max) {
                altura_max = y;
            }
        }
    }
    
    return altura_max;
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
		actualizar_camara();
		gestionar_colisiones();
	}
}

void inicializarPartida(){
    combustible = 0;
    camara_zoom = 1.0f;
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
	inicio = 1;
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