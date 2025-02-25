#include "fisicas.h"

void destruirObjetoFisico(struct objetoFisico* objeto){
    destruirDibujable(objeto -> objeto);
    free(objeto);
}

void calcularFisicas(struct objetoFisico* elemento){
    // Calculo de la aceleracion
    elemento -> aceleracion[1] += gravedad_m_ms;
    // Calculo de la velocidad en cada eje
    elemento -> velocidad[0] += elemento -> aceleracion[0] * intervalo_fisicas_ms;
    elemento -> velocidad[1] += elemento -> aceleracion[1] * intervalo_fisicas_ms;
    // Calculo de la nueva posicion dadas las velocidades
    struct Punto nueva_posicion = {
        elemento -> velocidad[0] * intervalo_fisicas_ms,
        -(elemento -> velocidad[1] * intervalo_fisicas_ms)
    };
    // Trasladar el objeto
    trasladarDibujable(elemento -> objeto, nueva_posicion);
}

/*
void propulsar(){ //Mejor propulsarse ?

}

void girar_izquierda(){

}

void girar_derecha(){

}*/