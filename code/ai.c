#include "ai.h"
#include "opciones.h"
#include "fisicas.h"
#include <math.h>

static EstadoAI estado_ai = AI_MANTENER_ALTURA;
static float altura_objetivo = 0.0f;
static float objetivo_x = 0.0f;
static float objetivo_y = 0.0f;

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
    if(numero_plataformas > 0) {
        struct Dibujable* linea = plataformas_partida[0].linea;
        objetivo_x = linea->origen.x +
                     (linea->puntos[0].x + linea->puntos[1].x) / 2.0f;
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
        if(fabsf(dx) > 5.0f) estado_ai = AI_MOVER_HORIZONTAL;
        break;

    case AI_MOVER_HORIZONTAL: {
        int16_t rot_obj = dx > 0 ? 90 : 270;
        rotar_hacia(rot_obj);
        if(nave->rotacion == rot_obj) {
            activar_propulsor();
            propulsar();
        } else {
            desactivar_propulsor();
        }
        if(fabsf(dx) < 5.0f && fabsf(nave->velocidad[0]) < 0.3f)
            estado_ai = AI_DESCENSO;
        if(dy_plat < 60.0f)
            estado_ai = AI_DESCENSO;
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