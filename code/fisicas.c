#include "fisicas.h"
#include "partida.h"

static uint16_t propulsor_activado = 0;
static uint16_t propulsor = 0;
static uint16_t orden_girar_izquierda = 0;
static uint16_t orden_girar_derecha = 0;

void destruirObjetoFisico(struct objetoFisico* objeto){
    destruirDibujable(objeto -> objeto);
    free(objeto);
}

void calcularFisicas(struct objetoFisico* elemento){
	if(orden_girar_izquierda && !orden_girar_derecha && (elemento -> rotacion <= 90 || elemento -> rotacion >= 270 + ANGULO_ROTACION)){
		elemento -> rotacion = (elemento -> rotacion - ANGULO_ROTACION + 360) % 360;
		rotar_nave(0);
	}
	else if (!orden_girar_izquierda && orden_girar_derecha && (elemento -> rotacion <= 90 - ANGULO_ROTACION || elemento -> rotacion >= 270)){
		elemento -> rotacion = (elemento -> rotacion + ANGULO_ROTACION) % 360;
		rotar_nave(1);
	}
	orden_girar_izquierda = 0;
	orden_girar_derecha = 0;

	if(propulsor && combustible >= combustible_motor){
			elemento -> aceleracion[0] += propulsor_m_ms * SIN_TABLA[elemento -> rotacion];
			elemento -> aceleracion[1] += propulsor_m_ms * COS_TABLA[elemento -> rotacion];
			combustible -= combustible_motor;
	} else if(propulsor && combustible < combustible_motor) {
			propulsor = 0;
			propulsor_activado = 0;
	}


    // Calculo de la aceleracion
    elemento -> aceleracion[1] += gravedad_m_ms;

    // Calculo de la velocidad en cada eje
    elemento -> velocidad[0] += elemento -> aceleracion[0] * intervalo_fisicas_ms;
    elemento -> velocidad[1] += elemento -> aceleracion[1] * intervalo_fisicas_ms;
	
	// Desaceleracion horizontal
	if(elemento -> velocidad[0] > 0){
		elemento -> velocidad[0] -= desaceleracion_horizontal;
		if (elemento->velocidad[0] < 0) elemento->velocidad[0] = 0;
	} else if(elemento -> velocidad[0] < 0){
		elemento -> velocidad[0] += desaceleracion_horizontal;
		if (elemento->velocidad[0] > 0) elemento->velocidad[0] = 0;
	}
	
    // Calculo de la nueva posicion dadas las velocidades
    struct Punto nueva_posicion = {
        elemento -> velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
        -(elemento -> velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)
    };

    // Trasladar el objeto
    trasladarDibujable(elemento -> objeto, nueva_posicion);

	elemento -> aceleracion[0] = 0;
	elemento -> aceleracion[1] = 0;
	
	if(!propulsor_activado){
		switch(propulsor){
			case 0:
				break;
			default:
				propulsor--;
				break;
		}
	}
}

void calcualarFisicasAsteroides(struct Asteroide* asteroides, uint8_t numAsteroides){
	for(uint8_t i = 0; i < numAsteroides; i++){
		struct Punto desplazamiento = {0, 0};
		float avance = asteroides[i].velocidad * intervalo_fisicas_ms / pixels_por_metro;

		if(asteroides[i].objeto->origen.y == ALTURA_CINTURON_ASTEROIDES_1){
				desplazamiento.x = avance;
		} else if(asteroides[i].objeto->origen.y == ALTURA_CINTURON_ASTEROIDES_2){
				desplazamiento.x = -avance;
		}

		trasladarDibujable(asteroides[i].objeto, desplazamiento);

		if(asteroides[i].objeto->origen.x < 0){
				trasladarDibujable(asteroides[i].objeto, (struct Punto){ANCHURA_TERRENO, 0});
		} else if(asteroides[i].objeto->origen.x >= ANCHURA_TERRENO){
				trasladarDibujable(asteroides[i].objeto, (struct Punto){-ANCHURA_TERRENO, 0});
		}
	}
}

void propulsar(){
	switch(propulsor){
		case 3:
			propulsor--;
			break;
		default:
			propulsor++;
			break;
	}
}

void activar_propulsor(){
	propulsor_activado = 1;
}

void desactivar_propulsor(){
	propulsor_activado = 0;
}

void girar_izquierda(){
	orden_girar_izquierda = 1;
}

void girar_derecha(){
	orden_girar_derecha = 1;
}

uint16_t obtener_propulsor(){
	return propulsor;
}

