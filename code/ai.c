#include "ai.h"
#include "opciones.h"
#include "fisicas.h"
#include "gestor_plataformas.h"
#include "../resources/superficie_lunar.h"
#include "../resources/nave.h"
#include "../resources/asteroides.h"
#include "sonidos.h"
#include <float.h>
#include <math.h>

struct PlataformaCercana {
    float dist;
    uint16_t idx;
};

static int cmp_dist(const void* a, const void* b) {
    float da = ((const struct PlataformaCercana*)a)->dist;
    float db = ((const struct PlataformaCercana*)b)->dist;
    return (da > db) - (da < db);
}

static EstadoAI estado_ai = AI_MANTENER_ALTURA;
static float altura_objetivo = 0.0f; // Y de spawneo
static float objetivo_x = 0.0f;      // Centro de la plataforma objetivo
static float objetivo_y = 0.0f;      // Y de la plataforma objetivo
static uint16_t plataforma_objetivo_idx = 0; // Indice de la plataforma objetivo
static float spawn_x = 0.0f;         // X de spawneo
static float spawn_y = 0.0f;         // Y de spawneo

int ai_activa(void) {
    return obtenerValorFlag(FLAG_AI);
}

static void rotar_hacia(int16_t rot_deseada) {
    if(nave->rotacion == rot_deseada) return;
    int diff = (rot_deseada - nave->rotacion + 360) % 360;
    if(diff > 180) {
        girar_izquierda();
    } else {
        girar_derecha();
    }
}

static void propulsar_hacia(int16_t rot_deseada) {
    rotar_hacia(rot_deseada);

    if(nave->rotacion == rot_deseada) {
        activar_propulsor();
        propulsar();
        if(Get_Playing_Sound()!=SONIDO_PROPULSION)
            Sound_Loop(SONIDO_PROPULSION);
    } else {
        desactivar_propulsor();
        if(Get_Playing_Sound()==SONIDO_PROPULSION)
            Sound_Stop();
    }
}

static void desactivar_propulsor_wrapper(void) {
    desactivar_propulsor();
    if(Get_Playing_Sound()==SONIDO_PROPULSION)
        Sound_Stop();
}

void ai_iniciar(void) {
    if(!ai_activa()) return;
    
    estado_ai = AI_MANTENER_ALTURA;
    altura_objetivo = ALTURA_SPAWN_NAVE;
    spawn_x = nave->objeto->origen.x;
    spawn_y = nave->objeto->origen.y;
    struct PlataformaCercana dists[MAX_PLATAFORMAS];

    // Buscar la plataforma mas cercana al spawn
    for(uint16_t i = 0; i < numero_plataformas; i++) {
        struct Dibujable* lin = plataformas_partida[i].linea;
        float centro_x = (lin->puntos[0].x + lin->puntos[1].x) / 2.0f;
        float dx = centro_x - spawn_x;
        if(dx > ANCHURA_TERRENO/2) dx -= ANCHURA_TERRENO;
        else if(dx < -ANCHURA_TERRENO/2) dx += ANCHURA_TERRENO;
        dists[i].dist = fabsf(dx); // distancia horizontal absoluta
        dists[i].idx = i;
    }
    
    // Ordenar las plataformas por distancia al spawn
    qsort(dists, numero_plataformas, sizeof(struct PlataformaCercana), cmp_dist);

    // Seleccionar la plataforma mas cercana.
    uint16_t elegido = dists[0].idx;
    plataforma_objetivo_idx = elegido;
    struct Dibujable* linea = plataformas_partida[plataforma_objetivo_idx].linea;
    objetivo_x = (linea->puntos[0].x + linea->puntos[1].x) / 2.0f;
    objetivo_y = linea->puntos[0].y;
    printf("IA: plataforma objetivo en (%.2f, %.2f)\n", objetivo_x, objetivo_y);
}

// La AI primero va alternando entre MANTENER_ALTURA y MOVER_HORIZONTAL, y luego Aterrizaje.
// En MANTENER_ALTURA, la nave se mantiene a una altura fija y ajusta la propulsión para no descender demasiado.
// En MOVER_HORIZONTAL, la nave se mueve hacia la plataforma objetivo, ajustando su rotación y propulsión para llegar a la posición horizontal.
// Finalmente, en Aterrizaje, la nave se asegura de que su velocidad vertical sea adecuada

void ai_actualizar(void) {
    if(!ai_activa() || fisicas != ACTIVADAS) return;


    float vel_x = nave->velocidad[0];
    float vel_y = nave->velocidad[1];

    float acc_x = nave->aceleracion[0];
    float acc_y = nave->aceleracion[1];

    float pos_x = nave->objeto->origen.x;
    float pos_y = nave->objeto->origen.y;


    float dx = objetivo_x - pos_x;
    float base_nave_y = pos_y + (ALTURA_NAVE - 5);
    float dy_plat = objetivo_y - base_nave_y;
    float dy_hover = altura_objetivo - pos_y;

printf("\r\33"
       "IA DEBUG -> Estado:%d DX:%.2f DY:%.2f DY_HOVER:%.2f "
       "POS_X:%.2f POS_Y:%.2f VEL_X:%.2f VEL_Y:%.2f",
       estado_ai, dx, dy_plat, dy_hover,
       pos_x, pos_y, vel_x, vel_y);

    switch(estado_ai) {
        case AI_MANTENER_ALTURA:
            if(dy_hover <= 1.0f || vel_y < -0.4f) {
                propulsar_hacia(0);
            } else if(vel_y < 0.3f) {
                desactivar_propulsor_wrapper();
                estado_ai = AI_MOVER_HORIZONTAL;
                printf("\nIA: cambio a estado MOVER_HORIZONTAL\n");
            } else if(fabsf(dx) < 3.0f && abs(vel_x) < 0.3f) {
                estado_ai = AI_ATERRIZAJE;
                nave->velocidad[0] = 0.0f;
                printf("\n3IA: cambio a estado DESCENSO\n");
            } else {
                desactivar_propulsor_wrapper();
            }
            break;

        case AI_MOVER_HORIZONTAL: {
            int16_t rot_obj = 0;
            if(dx > 3.0f && vel_x < 0.5f) {
                rot_obj = 90;
                propulsar_hacia(rot_obj);
            } else if (dx < -3.0f && vel_x > -0.5f) {
                rot_obj = 270;
                propulsar_hacia(rot_obj);
            } else if(fabsf(dx) < 1.0f && abs(vel_x) < 0.3f) {  // Si estamos en la posición correcta, cambiamos a DESCENSO
                desactivar_propulsor_wrapper();
                estado_ai = AI_ATERRIZAJE;
                nave->velocidad[0] = 0.0f;
                printf("\n3IA: cambio a estado ATERRIZAJE\n");
            } else if(dy_hover <= 6.0f) { // Si perdemos altura, volvemos a MANTENER_ALTURA
                desactivar_propulsor_wrapper();
                estado_ai = AI_MANTENER_ALTURA;
                printf("\nIA: regreso a estado MANTENER_ALTURA\n");
            } else {
                desactivar_propulsor_wrapper();
            }
            break; }

        case AI_DESCENSO: {
/*             int16_t rot_obj = 0;
            desactivar_propulsor_wrapper();

            // Si estamos en la posición correcta y a una altura adecuada, cambiamos a ATERRIZAJE
            if(fabsf(dx) <= 2.0f && abs(vel_x) < 0.3f && abs(dy_plat) <= 700.0f) {
                printf("\r\33IA: cambio a estado ATERRIZAJE\n");
                estado_ai = AI_ATERRIZAJE;
            }

            if(dx > 3.0f &&  acc_x <= 0) {
                rot_obj = 70;
                propulsar_hacia(rot_obj);
                printf("\r\33IA: rotando a 70\n");
            } else if(dx < -3.0f && acc_x >= 0) {
                rot_obj = 290;
                propulsar_hacia(rot_obj);
                printf("\r\33IA: rotando a 290\n");
            }

            break; */
        }
        case AI_ATERRIZAJE:{
            rotar_hacia(0);
            if(vel_y < - 0.2f && dy_plat < 130.0f)  {
                propulsar_hacia(0);
            } else {
                desactivar_propulsor_wrapper();
            }
            break;
        }
    }
}