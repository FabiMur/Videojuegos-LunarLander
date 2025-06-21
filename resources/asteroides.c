#include "asteroides.h"
#include "../code/transformaciones.h"
#include "../code/dibujable.h"
#include "../code/gestor_colisiones.h"

const struct DibujableConstante Asteroide_pequeno = {
    CENTRO_ASTEROIDE_PEQUENO,
    (struct Punto[]){
        {5, 17}, {0, 11}, {4, 9}, {0, 7}, {7, 0}, {13, 0},
        {ANCHURA_ASTEROIDE_PEQUENO, 6}, {ANCHURA_ASTEROIDE_PEQUENO, 12},
        {14, ALTURA_ASTEROIDE_PEQUENO}, {9, ALTURA_ASTEROIDE_PEQUENO}, {9, 11} 
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 0}
    },
    11,
    11
};

const struct DibujableConstante Asteroide_mediano = {
    CENTRO_ASTEROIDE_MEDIANO,
    (struct Punto[]){
        {9,ALTURA_ASTEROIDE_MEDIANO},{0,20},{0,8},{13,8},{9,0},{20,0},
        {ANCHURA_ASTEROIDE_MEDIANO,7},{ANCHURA_ASTEROIDE_MEDIANO,13},{20,16},
        {ANCHURA_ASTEROIDE_MEDIANO,24},{24,ALTURA_ASTEROIDE_MEDIANO},{20,27}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}, {11, 0}
    },
    12,
    12
};

const struct DibujableConstante Asteroide_grande = {
    CENTRO_ASTEROIDE_MEDIANO,
    (struct Punto[]){
        {14,ALTURA_ASTEROIDE_GRANDE},{0,40},{0,14},{16,0},{26,15},{31,0},
        {ANCHURA_ASTEROIDE_GRANDE,14},{47,25},{ANCHURA_ASTEROIDE_GRANDE,40},
        {34,ALTURA_ASTEROIDE_GRANDE}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 0}
    },
    10,
    10
};

enum TipoAsteroide randomAsteroideType(){
    int random = (int)rand() % 3;
    switch(random){
        case 0:
            return ASTEROIDE_PEQUENO;
        case 1:
            return ASTEROIDE_MEDIANO;
        case 2:
            return ASTEROIDE_GRANDE;
        default:
            return ASTEROIDE_GRANDE; // Por si acaso
    }
}

void dibujarAsteroide(HDC hdc, struct Asteroide* asteroide){
    colocarAsteroide(asteroide);

    switch(asteroide->tipo){
        case ASTEROIDE_PEQUENO:
            dibujarDibujable(hdc, asteroide->objeto);
            break;
        case ASTEROIDE_MEDIANO:
            dibujarDibujable(hdc, asteroide->objeto);
            break;
        case ASTEROIDE_GRANDE:
            dibujarDibujable(hdc, asteroide->objeto);
            break;
    }
}

void dibujarAsteroides(HDC hdc, struct Asteroide* asteroides, uint8_t numAsteroides){
    for(uint8_t i = 0; i < numAsteroides; i++){
        dibujarAsteroide(hdc, &asteroides[i]);
    }
}

void colocarAsteroide(struct Asteroide* asteroide){
    colocar_dibujable(asteroide->objeto, asteroide->objeto->origen);
}

struct Asteroide* generarAsteroides(const struct Dibujable* terreno, uint8_t* numAsteroides){
    struct Asteroide* asteroides = malloc(MAX_ASTEROIDES * sizeof(struct Asteroide));
    if(asteroides == NULL) return NULL;
   
    *numAsteroides = 0;
    for(uint8_t i = 0; i < MAX_ASTEROIDES; i++){
        struct Asteroide* tmp = malloc(sizeof(struct Asteroide));
        struct Punto* punto_random = malloc(sizeof(struct Punto));
        if(i < MAX_ASTEROIDES/2){
            punto_random->y = ALTURA_CINTURON_ASTEROIDES_1;
        } else {
            punto_random->y = ALTURA_CINTURON_ASTEROIDES_2;
        }

        punto_random->x = rand() % ANCHURA_TERRENO;
   
        tmp->tipo = randomAsteroideType();
        switch(tmp->tipo){
            case ASTEROIDE_PEQUENO:
                tmp->objeto = crearDibujable(&Asteroide_pequeno);
                break;
            case ASTEROIDE_MEDIANO:
                tmp->objeto = crearDibujable(&Asteroide_mediano);
                break;
            case ASTEROIDE_GRANDE:
                tmp->objeto = crearDibujable(&Asteroide_grande);
                break;
        }
        colocar_dibujable(tmp->objeto, *punto_random);
        tmp->velocidad = VELOCIDAD_ASTEROIDES;
        if(esPosicionCorrecta(terreno, tmp, asteroides, numAsteroides)){
            asteroides[*numAsteroides] = *tmp;
                (*numAsteroides)++;
        }
      
        free(tmp); 
        free(punto_random);
    }
    return asteroides;
}

struct Punto* randomPoint(uint32_t min_x, uint32_t max_x, uint32_t min_y, uint32_t max_y) {
    struct Punto* point = malloc(sizeof(struct Punto));
    point->x = rand() % (max_x - min_x + 1) + min_x;
    point->y = rand() % (max_y - min_y + 1) + min_y;
    return point;
}

boolean esPosicionCorrecta(const struct Dibujable* terreno, struct Asteroide* asteroide, struct Asteroide* asteroides, uint8_t* numAsteroides){
    struct Arista arista_colision = (struct Arista){0};

    if(!hay_colision(asteroide->objeto, terreno, &arista_colision)){
        for (uint8_t j = 0; j < *numAsteroides; j++){
            if(hay_colision(asteroide->objeto, asteroides[j].objeto, &arista_colision)){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}