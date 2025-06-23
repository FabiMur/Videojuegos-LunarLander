#include "ai.h"
#include "opciones.h"
#include "fisicas.h"
#include "gestor_plataformas.h"
#include <float.h>
#include <math.h>

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

void ai_iniciar(void) {
    if(!ai_activa()) return;
    estado_ai = AI_MANTENER_ALTURA;
    altura_objetivo = nave->objeto->origen.y;
    spawn_x = nave->objeto->origen.x;

    if(numero_plataformas > 0) {
 float min_dist = FLT_MAX;
        uint16_t candidatos[MAX_PLATAFORMAS];
        uint16_t num_candidatos = 0;

        for(uint16_t i = 0; i < numero_plataformas; i++) {
            struct Dibujable* lin = plataformas_partida[i].linea;
            float centro_x = lin->origen.x + (lin->puntos[0].x + lin->puntos[1].x) / 2.0f;
            float dist = fabsf(centro_x - spawn_x);
            if(dist < min_dist - 0.1f) {
                min_dist = dist;
                candidatos[0] = i;
                num_candidatos = 1;
            } else if(fabsf(dist - min_dist) <= 10.0f && num_candidatos < MAX_PLATAFORMAS) {
                candidatos[num_candidatos++] = i;
            }
        }

        uint16_t elegido = candidatos[rand() % num_candidatos];
        struct Dibujable* linea = plataformas_partida[elegido].linea;
        objetivo_x = linea->origen.x + (linea->puntos[0].x + linea->puntos[1].x) / 2.0f;
        objetivo_y = linea->origen.y + linea->puntos[0].y;
    } else {
        objetivo_x = nave->objeto->origen.x;
        objetivo_y = nave->objeto->origen.y + 200.0f;
    }
}

void ai_actualizar(void) {
    if(!ai_activa() || fisicas != ACTIVADAS) return;

    float dx = objetivo_x - nave->objeto->origen.x;
    float dy_plat = objetivo_y - nave->objeto->origen.y;
    float dy_spawn = altura_objetivo - nave->objeto->origen.y;

    switch(estado_ai) {
    case AI_MANTENER_ALTURA:
        rotar_hacia(0);
        if(nave->velocidad[1] < -0.2f || dy_spawn < -2.0f) {
            activar_propulsor();
            propulsar();
        } else {
            desactivar_propulsor();
        }
        if(fabsf(dy_spawn) <= 2.0f && fabsf(dx) > 3.0f)
            estado_ai = AI_MOVER_HORIZONTAL;
        break;

    case AI_MOVER_HORIZONTAL: {
        int16_t rot_obj;
        if(fabsf(dx) > 3.0f) {
            rot_obj = dx > 0 ? 90 : 270;
        } else {
            rot_obj = nave->velocidad[0] > 0 ? 270 : 90;
        }
        rotar_hacia(rot_obj);
        if(nave->rotacion == rot_obj) {
            activar_propulsor();
            propulsar();
        } else {
            desactivar_propulsor();
        }
        if(fabsf(dy_spawn) > 3.0f) {
            desactivar_propulsor();
            estado_ai = AI_MANTENER_ALTURA;
        } else if(fabsf(dx) < 3.0f && fabsf(nave->velocidad[0]) < 0.3f) {
            desactivar_propulsor();
            estado_ai = AI_DESCENSO;
        }
        break; }

    case AI_DESCENSO: {
        int16_t rot_obj = 0;
        if(dx > 3.0f) rot_obj = 45;
        else if(dx < -3.0f) rot_obj = 315;
        rotar_hacia(rot_obj);
        if(nave->velocidad[1] < -0.4f) {
            rotar_hacia(0);
            activar_propulsor();
            propulsar();
        } else if(dy_plat > 20.0f && nave->velocidad[1] < -0.1f) {
            activar_propulsor();
            propulsar();
        } else {
            desactivar_propulsor();
        }
        if(dy_plat < 20.0f)
            estado_ai = AI_ATERRIZAJE;
        break; }

    case AI_ATERRIZAJE:
        rotar_hacia(0);
        if(nave->velocidad[1] < -0.3f) {
            activar_propulsor();
            propulsar();
        } else {
            desactivar_propulsor();
        }
        break;
    }
}