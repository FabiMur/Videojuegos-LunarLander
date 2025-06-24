#ifndef SONIDOS_H
#define SONIDOS_H

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wtypes.h>
#include <stdio.h>

// Tipos de sonidos
typedef enum {
    SONIDO_SELECCIONAR_OPCION_MENU,
    SONIDO_CAMBIAR_OPCION_MENU,
    SONIDO_MONEDA,
    SONIDO_ATERRIZAJE,
    SONIDO_EXPLOSION,
    SONIDO_PROPULSION,
    NO_SOUND
} TipoSonido;

// Estructura para almacenar el sonido en memoria
typedef struct {
    uint8_t* buffer;
    uint64_t size;
} Sonido;

// Variables
extern Sonido sonidos[6];  // Declaración global para los buffers
extern TipoSonido playing;

// Rutas de archivos
static const char* rutas_sonidos[] = {
    "./resources/sounds/sonido_seleccionar_opcion_menu.wav",
    "./resources/sounds/sonido_cambiar_opcion_menu.wav",
    "./resources/sounds/sonido_moneda.wav",
    "./resources/sounds/sonido_aterrizaje.wav",
    "./resources/sounds/sonido_explosion.wav",
    "./resources/sounds/sonido_propulsion.wav"
};

// Prototipos
/**
 * @brief Inicializa los sonidos del juego.
 * 
 * Carga los sonidos desde los archivos y los prepara para su reproducción.
 */
void Sound_Init();

/**
 * @brief Reproduce un sonido específico.
 * 
 * @param sonido Tipo de sonido a reproducir.
 */
void Sound_Play(TipoSonido sonido);

/**
 * @brief Reproduce un sonido en bucle.
 * 
 * @param sonido Tipo de sonido a reproducir en bucle.
 */
void Sound_Loop(TipoSonido sonido);

/**
 * @brief Detiene el sonido actual.
 */
void Sound_Stop();

/**
 * @brief Obtiene el sonido que se está reproduciendo actualmente.
 * 
 * @return TipoSonido El tipo de sonido que se está reproduciendo.
 */
TipoSonido Get_Playing_Sound();

/**
 * @brief Libera los recursos utilizados por los sonidos.
 */
void Sound_Cleanup();

#endif /* SONIDOS_H */