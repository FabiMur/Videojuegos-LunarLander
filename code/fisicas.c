#include "fisicas.h"

void destruirObjetoFisico(struct objetoFisico* objeto){
    destruirDibujable(objeto -> objeto);
    free(objeto);
}

void calcularFisicas(struct objetoFisico* elemento){
    elemento -> aceleracion[1] += gravedad_m_ms;
    elemento -> velocidad[0] += elemento -> aceleracion[0] * intervalo_fisicas_ms;
    elemento -> velocidad[1] += elemento -> aceleracion[1] * intervalo_fisicas_ms;
    elemento -> objeto -> origen.x += elemento -> velocidad[0] * intervalo_fisicas_ms;
    elemento -> objeto -> origen.y -= elemento -> velocidad[1] * intervalo_fisicas_ms;
    trasladarDibujable(elemento -> objeto, elemento -> objeto -> origen);
}

void acelerar(){

}

void girar_izquierda(){

}

void girar_derecha(){

}