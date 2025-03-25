#include "fisicas.h"
#include "partida.h"
#include <stdio.h>

static uint8_t propulsor = 0;
static uint8_t orden_girar_izquierda = 0;
static uint8_t orden_girar_derecha = 0;

const float SIN_TABLA[24] = {	
    0.0000, 0.2588, 0.5000, 0.7071, 0.8660, 0.9659, 1.0000, 0.9659,
    0.8660, 0.7071, 0.5000, 0.2588, 0.0000, -0.2588, -0.5000, -0.7071,
    -0.8660, -0.9659, -1.0000, -0.9659, -0.8660, -0.7071, -0.5000, -0.2588
};

const float COS_TABLA[24] = {	
    1.0000, 0.9659, 0.8660, 0.7071, 0.5000, 0.2588, 0.0000, -0.2588,
    -0.5000, -0.7071, -0.8660, -0.9659, -1.0000, -0.9659, -0.8660, -0.7071,
    -0.5000, -0.2588, 0.0000, 0.2588, 0.5000, 0.7071, 0.8660, 0.9659
};

void destruirObjetoFisico(struct objetoFisico* objeto){
    destruirDibujable(objeto -> objeto);
    free(objeto);
}

void calcularFisicas(struct objetoFisico* elemento){
	if(orden_girar_izquierda && !orden_girar_derecha){
		elemento -> rotacion = (elemento -> rotacion - ANGULO_ROTACION + 360) % 360;
		rotar_nave(0);
	}
	else if (!orden_girar_izquierda && orden_girar_derecha){
		elemento -> rotacion = (elemento -> rotacion + ANGULO_ROTACION) % 360;
		rotar_nave(1);
	}
	orden_girar_izquierda = 0;
	orden_girar_derecha = 0;

	if(propulsor){
		int indice_rotacion = elemento -> rotacion / ANGULO_ROTACION;
		fprintf(stderr, "Rotacion: %d\n", elemento -> rotacion);
		fprintf(stderr, "Indice: %d\n", indice_rotacion);
		fprintf(stderr, "Seno: %f\n", SIN_TABLA[indice_rotacion]);
		fprintf(stderr, "Coseno: %f\n", COS_TABLA[indice_rotacion]);
		elemento -> aceleracion[0] += propulsor_m_ms * SIN_TABLA[indice_rotacion];
		elemento -> aceleracion[1] += propulsor_m_ms * COS_TABLA[indice_rotacion];
	}

    // Calculo de la aceleracion
    elemento -> aceleracion[1] += gravedad_m_ms;

    // Calculo de la velocidad en cada eje
    elemento -> velocidad[0] += elemento -> aceleracion[0] * intervalo_fisicas_ms;
    elemento -> velocidad[1] += elemento -> aceleracion[1] * intervalo_fisicas_ms;

    // Calculo de la nueva posicion dadas las velocidades
    struct Punto nueva_posicion = {
        elemento -> velocidad[0] * intervalo_fisicas_ms / pixels_por_metro,
        -(elemento -> velocidad[1] * intervalo_fisicas_ms / pixels_por_metro)
    };

    // Trasladar el objeto
    trasladarDibujable(elemento -> objeto, nueva_posicion);

	elemento -> aceleracion[0] = 0;
	elemento -> aceleracion[1] = 0;
	
	switch(propulsor){
		case 0:
			break;
		default:
			propulsor--;
			break;
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


void girar_izquierda(){
	orden_girar_izquierda = 1;
}

void girar_derecha(){
	orden_girar_derecha = 1;
}

uint8_t obtener_propulsor(){
	return propulsor;
}
