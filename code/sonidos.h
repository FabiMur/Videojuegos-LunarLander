/**
 * @brief Archivo de cabecera para la gestión de efectos de sonido y música de fondo en el juego Lunar Lander.
 * 
 * Este archivo contiene las declaraciones de funciones y variables relacionadas con la 
 * inicialización, carga, reproducción y detención de sonidos utilizados en el juego.
 * Está diseñado para manejar todas las funcionalidades relacionadas con el audio.
 */
#ifndef SONIDOS_H
#define SONIDOS_H

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wtypes.h>
#include <stdio.h>

/**
 * @enum Sonidos
 * @brief Enumeración que representa los diferentes sonidos utilizados en el juego Lunar Lander.
 * 
 * Esta enumeración define los identificadores para los efectos de sonido que se reproducen
 * durante el juego, como el sonido del motor, colisiones, y otros eventos importantes.
 */
typedef enum {
    SONIDO_SELECCIONAR_OPCION_MENU,
    SONIDO_CAMBIAR_OPCION_MENU,
    SONIDO_MONEDA,
    SONIDO_ATERRIZAJE,
    SONIDO_EXPLOSION,
    SONIDO_PROPULSION,
    NO_SOUND
} TipoSonido;

/**
 * Filenames de los sonidos utilizados en el juego.
 */
static const char* sonido_seleccionar_opcion_menu = "./resources/sounds/sonido_seleccionar_opcion_menu.wav";
static const char* sonido_cambiar_opcion_menu = "./resources/sounds/sonido_cambiar_opcion_menu.wav";
static const char* sonido_moneda = "./resources/sounds/sonido_moneda.wav";
static const char* sonido_aterrizaje = "./resources/sounds/sonido_aterrizaje.wav";
static const char* sonido_explosion = "./resources/sounds/sonido_explosion.wav";
static const char* sonido_propulsion = "./resources/sounds/sonido_propulsion.wav";

static TipoSonido playing = NO_SOUND;

/**
 * @brief Inicializa el sistema de sonido para reproducir archivos de audio.
 */
void Sound_Init();

/**
 * @brief Reproduce un efecto de sonido específico una vez.
 * 
 * @param sonido El identificador del sonido a reproducir, basado en la enumeración TipoSonido.
 */
void Sound_Play(TipoSonido sonido);

/**
 * @brief Reproduce un efecto de sonido en bucle continuo.
 * 
 * @param sonido El identificador del sonido a reproducir en bucle, basado en la enumeración TipoSonido.
 */
void Sound_Loop(TipoSonido sonido);

/**
 * @brief Obtiene el nombre del archivo de sonido correspondiente a un tipo de sonido.
 * 
 * Esta función devuelve la ruta del archivo de sonido asociada con el identificador
 * proporcionado en la enumeración TipoSonido.
 * 
 * @param tipo El identificador del tipo de sonido, basado en la enumeración TipoSonido.
 * @return char* La ruta del archivo de sonido correspondiente.
 */
const char* obtener_nombre_sonido(TipoSonido tipo);

/**
 * @brief Detiene la reproducción de cualquier sonido en curso.
 * 
 * Esta función detiene cualquier efecto de sonido o música de fondo que esté
 * reproduciéndose en el momento de su invocación.
 */
void Sound_Stop();

/**
 * @brief Devuelve el tipo de sonido que se está reproduciendo actualmente.
 * 
 * @return TipoSonido - el tipo de sonido que se está reproduciendo actualmente.
 */
TipoSonido Get_Playing_Sound();

#endif /* SONIDOS_H */