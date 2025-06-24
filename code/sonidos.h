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
    char* buffer;
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
void Sound_Init();
void Sound_Play(TipoSonido sonido);
void Sound_Loop(TipoSonido sonido);
void Sound_Stop();
TipoSonido Get_Playing_Sound();
void Sound_Cleanup();

#endif /* SONIDOS_H */