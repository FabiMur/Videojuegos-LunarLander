#include "fisicas.h"

static uint8_t propulsor = 0;

void destruirObjetoFisico(struct objetoFisico* objeto){
    destruirDibujable(objeto -> objeto);
    free(objeto);
}

void calcularFisicas(struct objetoFisico* elemento){
	if(propulsor){
		elemento -> aceleracion[1] += propulsor_m_ms;
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

/*
void girar_izquierda(){

}

void girar_derecha(){

}*/

uint8_t obtener_propulsor(){
	return propulsor;
}