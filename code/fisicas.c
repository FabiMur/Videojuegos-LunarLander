#include "fisicas.h"

#include "partida.h"

static uint8_t propulsor_activado = 0;
static uint8_t propulsor = 0;
static uint8_t orden_girar_izquierda = 0;
static uint8_t orden_girar_derecha = 0;

void destruirObjetoFisico(struct objetoFisico* objeto){
    destruirDibujable(objeto -> objeto);
    free(objeto);
}

struct Punto calcularFisicas(struct objetoFisico* elemento){
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

	if(propulsor && combustible >= combustible_motor){
		elemento -> aceleracion[0] += propulsor_m_ms * SIN_TABLA[elemento -> rotacion];
		elemento -> aceleracion[1] += propulsor_m_ms * COS_TABLA[elemento -> rotacion];
		combustible -= combustible_motor;
	}

    // Calculo de la aceleracion
    elemento -> aceleracion[1] += gravedad_m_ms;

    // Calculo de la velocidad en cada eje
    elemento -> velocidad[0] += elemento -> aceleracion[0] * intervalo_fisicas_ms;
    elemento -> velocidad[1] += elemento -> aceleracion[1] * intervalo_fisicas_ms;

    // Calculo del desplazamiento dadas las velocidades
	float desp_x = elemento -> velocidad[0] * intervalo_fisicas_ms / pixels_por_metro;
	float desp_y = -(elemento -> velocidad[1] * intervalo_fisicas_ms / pixels_por_metro);
	float pos_x = elemento -> objeto -> origen.x;
	float pos_y = elemento -> objeto -> origen.y;
	float desp_terreno_x = 0;
	float desp_terreno_y = 0;

	// Límite de posición de la nave, no desplazar nave en x y desplazar terreno
	if (pos_x + desp_x > 920) {
		desp_terreno_x = pos_x + desp_x - 920;
		desp_x -= desp_terreno_x;
	} else if (pos_x + desp_x < 106) {
		desp_terreno_x = pos_x - desp_x - 106;
		desp_x -= desp_terreno_x;
	}

	if (pos_y + desp_y < 168) {
		desp_terreno_y = -pos_y - desp_y + 168;
		desp_y += desp_terreno_y;
	} else if (pos_y + desp_y > 550) {
		desp_terreno_y = -pos_y + desp_y + 550;
		desp_y += desp_terreno_y;
	}


    struct Punto desplazamiento = {
        desp_x, desp_y
    };

    // Trasladar el objeto
    trasladarDibujable(elemento -> objeto, desplazamiento);

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
	return (struct Punto) {desp_terreno_x, desp_terreno_y};
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

uint8_t obtener_propulsor(){
	return propulsor;
}
