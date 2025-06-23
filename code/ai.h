#ifndef AI_H
#define AI_H

#include "partida.h"

// Estados de la maquina de la IA
typedef enum {
    AI_MANTENER_ALTURA,
    AI_MOVER_HORIZONTAL,
    AI_DESCENSO,
    AI_ATERRIZAJE
} EstadoAI;

// Inicializa la IA (debe llamarse tras posicionar la nave)
void ai_iniciar(void);

// Actualiza las acciones de la IA para el frame actual
void ai_actualizar(void);

// Indica si la IA esta activa (segun las opciones)
int ai_activa(void);

#endif // AI_H