#include "sonidos.h"

void Sound_Init(){
    // pass
}

void Sound_Play(TipoSonido sonido){
    const char* nombre_sonido = obtener_nombre_sonido(sonido);
    if (nombre_sonido != NULL) {
        PlaySound(nombre_sonido, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOWAIT);
        playing = sonido;
    } else {
        printf("Error: Tipo de sonido no valido\n");
    }
    
}

void Sound_Loop(TipoSonido sonido){
    if (playing == sonido) {
        return; // No hacer nada si el sonido ya está reproduciéndose
    }   
    const char* nombre_sonido = obtener_nombre_sonido(sonido);

    if (nombre_sonido != NULL) {
        PlaySound(nombre_sonido, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP );
        playing = sonido;
    } else {
        printf("Error: Tipo de sonido no valido\n");
    }
}

const char* obtener_nombre_sonido(TipoSonido tipo){
    switch(tipo){
        case SONIDO_SELECCIONAR_OPCION_MENU:
            return sonido_seleccionar_opcion_menu;
        case SONIDO_CAMBIAR_OPCION_MENU:
            return sonido_cambiar_opcion_menu;
        case SONIDO_MONEDA:
            return sonido_moneda;
        case SONIDO_ATERRIZAJE:
            return sonido_aterrizaje;
        case SONIDO_EXPLOSION:
            return sonido_explosion;
        case SONIDO_PROPULSION:
            return sonido_propulsion;
        default:
            printf("Error: No existe archivo para el tipo de sonido especificado\n");
            return NULL; 
    }
}

void Sound_Stop(){
    PlaySound(NULL, NULL, SND_PURGE);
    playing = NO_SOUND;
}

TipoSonido Get_Playing_Sound(){
    return playing;
}