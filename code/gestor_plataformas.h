#ifndef GESTOR_PLATAFORMAS_H
#define GESTOR_PLATAFORMAS_H

#include "../resources/superficie_lunar.h"

// Numero maximo de plataformas por terreno
#define MAX_PLATAFORMAS 25


/**
 * @brief inicializar el rand para la aleatoriedad de generacion de plataformas
 */
void inicializar_aleatoriedad();

/**
 * @brief Genera un conjunto de plataformas basadas en las aristas del terreno.
 *
 * @param terreno Puntero a la estructura `DibujableConstante` que representa el
 *                terreno base.
 * @param num_plataformas Puntero a una variable donde se almacenará la cantidad
 *                        de plataformas generadas.
 * 
 * @return Un puntero a un arreglo de estructuras `Plataforma`, o `NULL` en caso
 *         de error.
 *
 * @note La memoria asignada para las plataformas debe ser liberada por el
 *       llamador cuando ya no se necesite.
 * @note La función asume que `srand()` ha sido inicializado previamente para
 *       garantizar aleatoriedad.
 *
 * @warning Si no se encuentran aristas válidas, la función retorna `NULL`.
 * @warning Si ocurre un error de asignación de memoria, la función libera lo
 *          previamente asignado y retorna `NULL`.
 */
struct Plataforma* generar_plataformas(const struct DibujableConstante* terreno,
    uint16_t* num_plataformas);

/**
 * @brief Dibuja la plataforma pasada como parametro
 * 
 * @param hdc
 * @param plataforma plataforma a dibujar
 */
void dibujar_plataforma(HDC hdc, struct Plataforma plataforma);


#endif // GESTOR_PLATAFORMAS_H