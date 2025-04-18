#include "transformaciones.h"

#include "../resources/caracteres.h"


/**
 * @brief Traslada un punto en la dirección indicada
 * 
 * @param origen Punto a trasladar
 * @param traslacion Punto de traslación
 */
void trasladarPunto(struct Punto* origen, struct Punto traslacion){
    origen -> x += traslacion.x;
    origen -> y += traslacion.y;
}

void trasladarDibujable(struct Dibujable* dibujable, struct Punto traslacion){
    trasladarPunto(&dibujable -> origen, traslacion);
    if (!dibujable -> puntos) return;
    for(uint8_t i = 0; i < dibujable -> num_puntos; i++){
        trasladarPunto(&dibujable -> puntos[i], traslacion);
    }
}

void colocar_dibujable(struct Dibujable* dibujable, struct Punto destino){
    if (!dibujable->puntos) return;
    struct Punto traslacion = (struct Punto){ destino.x - dibujable -> origen.x, destino.y - dibujable -> origen.y};
    trasladarDibujable(dibujable, traslacion);
}

void colocar_texto(struct Texto* texto, struct Punto destino) {
    if (!texto || !texto->caracteres) return;

    // Define el nuevo origen "base" para el Texto
    texto->origen = destino;
    
    // La separación entre centros, usando la escala actual
    float separacion_centros = (ANCHURA_CARACTER_MAX + SEPARACION_CARACTER) * texto->factor_escalado_x;
    if (separacion_centros < 1)
        separacion_centros = 1;
    
    // Para cada letra, calcular la posición absoluta deseada
    for (uint8_t i = 0; i < texto->num_caracteres; i++) {
        struct Punto posicionAbsoluta = {
            destino.x + separacion_centros * i,
            destino.y
        };
        // Coloca la letra en la posición absoluta calculada
        colocar_dibujable(texto->caracteres[i], posicionAbsoluta);
    }
}


void colocarDibujable(struct Dibujable* dibujable, struct Punto destino){
	struct Punto traslacion = (struct Punto){
		destino.x - dibujable -> origen.x,
		destino.y - dibujable -> origen.y
	};
    trasladarDibujable(dibujable, traslacion);
}

/**
 * @brief Rota un punto dado un centro y un sentido
 * 
 * @param punto Punto a rotar
 * @param centro Centro de rotación
 * @param sentido 1 para horario, -1 para antihorario
 */
void rotarPuntoDadoCentro(struct Punto* punto, struct Punto centro, int8_t sentido){
    float x = punto->x - centro.x;
    float y = punto->y - centro.y;
    float nuevaX, nuevaY;

    nuevaX = x * COS_TABLA[ANGULO_ROTACION] - y * sentido * SIN_TABLA[ANGULO_ROTACION];
    nuevaY = x * sentido * SIN_TABLA[ANGULO_ROTACION] + y * COS_TABLA[ANGULO_ROTACION];
    
    punto->x = (nuevaX + centro.x);
    punto->y = (nuevaY + centro.y);
}

void rotarDibujable(struct Dibujable* dibujable, unsigned char direccion){
    if (!dibujable->puntos) return;
    int8_t sentido = (direccion == 0) ? -1 : 1;
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        rotarPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, sentido);
    }
}


void escalarXPuntoDadoCentro(struct Punto* punto, struct Punto centro, float factor){
    float x = punto->x - centro.x;
    punto->x = (float)(x * factor + centro.x);
}

void escalarYPuntoDadoCentro(struct Punto* punto, struct Punto centro, float factor){
    float y = punto->y - centro.y;
    punto->y = (float)(y * factor + centro.y);
}

void escalarDibujable(struct Dibujable* dibujable, float factor){
    escalarDibujableDadosEjes(dibujable, factor, factor);
}

void escalar_dibujable_en_escena_dados_ejes(struct Dibujable* dibujable, float factorX, float factorY) {
    if (!dibujable->puntos) return;
    dibujable->origen.x *= factorX;
    dibujable->origen.y *= factorY;
    
    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        dibujable->puntos[i].x *= factorX;
        dibujable->puntos[i].y *= factorY;      
    }
}

void escalar_texto_en_escena_dados_ejes(struct Texto* texto, float factorX, float factorY) {
    if (!texto->caracteres) return;
    for(uint8_t i = 0; i < texto->num_caracteres; i++){
        escalar_dibujable_en_escena_dados_ejes(texto->caracteres[i], factorX, factorY);
    }

}

void escalarDibujableDadosEjes(struct Dibujable* dibujable, float factorX, float factorY){
    if (!dibujable->puntos) return;

    for(uint8_t i = 0; i < dibujable->num_puntos; i++){
        escalarXPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, factorX);
        escalarYPuntoDadoCentro(&dibujable->puntos[i], dibujable->origen, factorY);
    }
}
        
void escalar_texto_centrado(struct Texto* texto, float factor){
    escalar_texto_centrado_dados_ejes(texto, factor, factor);
}


/**
 * @brief Escala una palabra dados los centros de cada letra en los ejes X e Y
 * 
 * @param palabra Palabra a escalar
 * @param factorX factor de escalado en el eje X
 * @param factorY factor de escalado en el eje Y
 */
void escalar_texto(struct Texto* texto, float factorX, float factorY) {
    for(uint8_t i = 0; i < texto->num_caracteres; i++){
        if(!texto->caracteres[i]->puntos) return;
        for(uint8_t j = 0; j < texto->caracteres[i]->num_puntos; j++){
            escalarXPuntoDadoCentro(&texto->caracteres[i]->puntos[j], texto->caracteres[i]->origen, factorX);
            escalarYPuntoDadoCentro(&texto->caracteres[i]->puntos[j], texto->caracteres[i]->origen, factorY);
        }
    }
}


void escalar_texto_centrado_dados_ejes(struct Texto* texto, float factorX, float factorY){
    if(!texto->caracteres) return;

    // Establecer los factores de escalado
    texto->factor_escalado_x = factorX;
    texto->factor_escalado_y = factorY;

    float distancia_origen_a_centro = ((texto->num_caracteres * ANCHURA_CARACTER_MAX) + ((texto->num_caracteres-1)* SEPARACION_CARACTER) - ANCHURA_CARACTER_MAX) / 2;
    float nuevo_origen_texto_x = (texto->origen.x + distancia_origen_a_centro) - (distancia_origen_a_centro * factorX);
    
    // Generar el escalado de los caracteres de la texto
    escalar_texto(texto, factorX, factorY);

    // Establecer las posiciones de los caracteres dado el nuevo origen
    colocar_texto(texto, (struct Punto){nuevo_origen_texto_x, texto->caracteres[0]->origen.y});
}


void escalar_plataforma_dados_ejes(struct Plataforma* plataforma, float factorX, float factorY){
    if(!plataforma->linea) return;
    escalarDibujableDadosEjes(plataforma->linea, factorX, factorY);
    escalar_texto_centrado_dados_ejes(plataforma->texto, factorX, factorY);
}



void trasladar_texto(struct Texto* texto, struct Punto traslacion){
    if(!texto || !texto->caracteres) return;
    trasladarPunto(&texto->origen, traslacion);
    for(uint8_t i = 0; i < texto->num_caracteres; i++) {
        trasladarDibujable(texto->caracteres[i], traslacion);
    }    
}

void trasladar_superficie_lunar(struct Dibujable* terreno, struct Plataforma* plataformas, uint8_t num_plataformas, struct Punto traslacion) {
    trasladarDibujable(terreno, traslacion);
    for(uint8_t i = 0; i < num_plataformas; i++) {
        trasladarDibujable(plataformas[i].linea, traslacion);
        trasladar_texto(plataformas[i].texto, traslacion);
    }
}
