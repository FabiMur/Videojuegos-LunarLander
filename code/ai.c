#include "ai.h"
#include "opciones.h"
#include "fisicas.h"
#include "gestor_plataformas.h"
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
static float spawn_x = 0.0f;         // X de spawneo

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
    } else {
        desactivar_propulsor();
    }
}

void ai_iniciar(void) {
    if(!ai_activa()) return;
    
    estado_ai = AI_MANTENER_ALTURA;
    altura_objetivo = 50.0f;
    spawn_x = nave->objeto->origen.x;
    struct PlataformaCercana dists[MAX_PLATAFORMAS];

    // Buscar la plataforma mas cercana al spawn
    for(uint16_t i = 0; i < numero_plataformas; i++) {
        struct Dibujable* lin = plataformas_partida[i].linea;
        float centro_x = lin->origen.x + (lin->puntos[0].x + lin->puntos[1].x) / 2.0f;
        dists[i].dist = fabsf(centro_x - spawn_x);
        dists[i].idx = i;
    }
    
    // Ordenar las plataformas por distancia al spawn
    qsort(dists, numero_plataformas, sizeof(struct PlataformaCercana), cmp_dist);
    uint16_t elegido = dists[0].idx; // Seleccionar la plataforma mas cercana
    struct Dibujable* linea = plataformas_partida[elegido].linea;
    objetivo_x = linea->origen.x + (linea->puntos[0].x + linea->puntos[1].x) / 2.0f;
    objetivo_y = linea->origen.y + linea->puntos[0].y;
    printf("IA: plataforma objetivo en (%.2f, %.2f)\n", objetivo_x, objetivo_y);
}

// La AI primero va alternando entre MANTENER_ALTURA y MOVER_HORIZONTAL, y luego DESCENSO y ATERRIZAJE.
// En MANTENER_ALTURA, la nave se mantiene a una altura fija y ajusta la propulsión para no descender demasiado.
// En MOVER_HORIZONTAL, la nave se mueve hacia la plataforma objetivo, ajustando su rotación y propulsión para llegar a la posición horizontal.
// En DESCENSO, la nave se prepara para aterrizar, ajustando su rotación y propulsión para descender suavemente.
// Finalmente, en ATERRIZAJE, la nave se asegura de que su velocidad vertical sea adecuada

void ai_actualizar(void) {
    if(!ai_activa() || fisicas != ACTIVADAS) return;


    float vel_x = nave->velocidad[0];
    float vel_y = nave->velocidad[1];
    float acc_x = nave->aceleracion[0];
    float acc_y = nave->aceleracion[1];

    float pos_x = nave->objeto->origen.x;
    float pos_y = nave->objeto->origen.y;
    
    float rotacion = nave->rotacion;

    float dx = objetivo_x - pos_x;
    float dy_plat = objetivo_y - pos_y;
    float dy_hover = altura_objetivo - pos_y;

    printf("IA DEBUG -> Estado:%d DX:%.2f DY:%.2f DY_HOVER:%.2f POS_X:%.2f POS_Y:%.2f VEL_X:%.2f VEL_Y:%.2f\n",
        estado_ai, dx, dy_plat, dy_hover, pos_x, pos_y, vel_x, vel_y);

    switch(estado_ai) {
        case AI_MANTENER_ALTURA:
            rotar_hacia(0);


            if(dy_hover <= 1.0f || vel_y < -0.4f && nave->rotacion == 0) {

                propulsar_hacia(0);
            } else {
                desactivar_propulsor();
            }

            if(vel_y < 0.3f) {
                estado_ai = AI_MOVER_HORIZONTAL;
                printf("IA: cambio a estado MOVER_HORIZONTAL\n");
            }
            break;

        case AI_MOVER_HORIZONTAL: {
            int16_t rot_obj = 0;
            if(dx > 10.0f && vel_x < 0.05f) {
                rot_obj = 90;
            } else if (dx < -3.0f && vel_x > -0.05f) {
                rot_obj = 270;
            }

            propulsar_hacia(rot_obj);

            // Si estamos en la posición correcta, cambiamos a DESCENSO
            if(fabsf(dx) < 3.0f && abs(vel_x) < 0.3f) {
                desactivar_propulsor();
                estado_ai = AI_DESCENSO;
                printf("IA: cambio a estado DESCENSO\n");
            
            // Si perdemos altura, volvemos a MANTENER_ALTURA
            } else if(dy_hover >= 6.0f) {
                desactivar_propulsor();
                estado_ai = AI_MANTENER_ALTURA;
                printf("IA: regreso a estado MANTENER_ALTURA\n");
            }
            break; }

        case AI_DESCENSO: {
            int16_t rot_obj = 0;
            if(dx > 1.0f && vel_x < 0.1f) rot_obj = 45; printf("IA: rotando a 45\n");
            if(dx < -1.0f && vel_x > -0.1f) rot_obj = 315; printf("IA: rotando a 315\n");

            propulsar_hacia(rot_obj);
            
            // Si estamos en la posición correcta y a una altura adecuada, cambiamos a ATERRIZAJE
            if(fabsf(dx) <= 1.0f && abs(vel_x) < 0.1f && abs(dy_plat) < 5.0f) {
                printf("IA: cambio a estado ATERRIZAJE\n");
                estado_ai = AI_ATERRIZAJE;
            break; }

        case AI_ATERRIZAJE:
            if(vel_y < -0.3f) {
                propulsar_hacia(0);
            } else {
                desactivar_propulsor();
            }
            break;
        }
    }
}