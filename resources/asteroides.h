#ifndef ASTEROIDES_H
#define ASTEROIDES_H

#include "../code/dibujable.h"

#define CENTRO_ASTEROIDE_PEQUENO {ALTURA_ASTEROIDE_PEQUENO/2, ANCHURA_ASTEROIDE_PEQUENO/2}
#define CENTRO_ASTEROIDE_MEDIANO {ALTURA_ASTEROIDE_MEDIANO/2, ANCHURA_ASTEROIDE_MEDIANO/2}
#define CENTRO_ASTEROIDE_GRANDE {ALTURA_ASTEROIDE_GRANDE/2, ANCHURA_ASTEROIDE_GRANDE/2}


#define ANCHURA_ASTEROIDE_GRANDE 54
#define ALTURA_ASTEROIDE_GRANDE 54
#define ANCHURA_ASTEROIDE_MEDIANO 31
#define ALTURA_ASTEROIDE_MEDIANO 31
#define ANCHURA_ASTEROIDE_PEQUENO 18
#define ALTURA_ASTEROIDE_PEQUENO 18

#define ALTURA_CINTURON_ASTEROIDES_1 100
#define ALTURA_CINTURON_ASTEROIDES_2 250

#define VELOCIDAD_ASTEROIDES 2

#define MAX_ASTEROIDES 10

/**
 * @brief Constantes que representan los asteroides
 * 
 */
extern const struct DibujableConstante Asteroide_grande;
extern const struct DibujableConstante Asteroide_mediano;
extern const struct DibujableConstante Asteroide_pequeno;

/**
 * @brief Enumeración de los tipos de asteroides
 * 
 */
enum TipoAsteroide {
    ASTEROIDE_PEQUENO,
    ASTEROIDE_MEDIANO,
    ASTEROIDE_GRANDE
};

/**
 * @brief Estructura que representa un asteroide
 * 
 */
struct Asteroide {
    struct Dibujable* objeto;
    enum TipoAsteroide tipo;
    float velocidad;
};

/**
 * @brief Genera un tipo de asteroide aleatorio
 * 
 * @return enum TipoAsteroide 
 */
enum TipoAsteroide randomAsteroideType();

/**
 * @brief Dibuja un asteroide en la pantalla
 * 
 * @param hdc ventana de dibujo
 * @param asteroide asteroide a dibujar
 */
void dibujarAsteroide(HDC hdc, struct Asteroide* asteroide);

/**
 * @brief Dibuja un conjunto de asteroides en la pantalla
 * 
 * @param hdc ventana de dibujo
 * @param asteroides asterides a dibujar
 * @param numAsteroides numero de asteroides a dibujar
 */
void dibujarAsteroides(HDC hdc, struct Asteroide* asteroides, uint8_t numAsteroides);

/**
 * @brief Coloca un asteroide en la pantalla
 * 
 * @param asteroide asteroide a colocar
 */
void colocarAsteroide(struct Asteroide* asteroide);

/**
 * @brief Genera un conjunto de asteroides aleatorios en el terreno dado
 * 
 * @param terreno terreno donde se generaran los asteroides
 * @param numAsteroides numero de asteroides a generar
 * @return puntero a los asteroides generados 
 */
struct Asteroide* generarAsteroides(const struct Dibujable* terreno, uint8_t* numAsteroides);

/**
 * @brief Genera un punto aleatorio dentro de un rango dado
 * 
 * @param min_x valor minimo de x
 * @param max_x valor maximo de x
 * @param min_y valor minimo de y
 * @param max_y valor maximo de y
 * @return punto aleatorio generado
 */
struct Punto* randomPoint(uint32_t min_x, uint32_t max_x, uint32_t min_y, uint32_t max_y);

/**
 * @brief Comprueba si la posicion de un asteroide es correcta, es decir, si no colisiona con el terreno o con otros asteroides
 * 
 * @param terreno  terreno donde se generaran los asteroides
 * @param asteroide asteroide a comprobar
 * @param asteroides asteroides generados
 * @param numAsteroides numero de asteroides generados
 * @return 1 si la posicion es correcta, 0 si no lo es 
 */
boolean esPosicionCorrecta(const struct Dibujable* terreno, struct Asteroide* asteroide, struct Asteroide* asteroides, uint8_t* numAsteroides);

#endif // ASTEROIDES_H