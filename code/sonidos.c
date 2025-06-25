#include "sonidos.h"
#include "opciones.h"

Sonido sonidos[6];
TipoSonido playing = NO_SOUND;

void Sound_Init() {
    playing = NO_SOUND;

    for (int i = 0; i < 6; i++) {
        FILE* file = fopen(rutas_sonidos[i], "rb");
        if (!file) {
            printf("Error: No se pudo abrir el archivo %s\n", rutas_sonidos[i]);
            continue;
        }

        fseek(file, 0, SEEK_END);
        uint64_t size = ftell(file);
        rewind(file);

        sonidos[i].buffer = (uint8_t*)malloc(size);
        if (!sonidos[i].buffer) {
            printf("Error: No se pudo asignar memoria para el sonido %d\n", i);
            fclose(file);
            continue;
        }

        fread(sonidos[i].buffer, 1, size, file);
        sonidos[i].size = size;

        fclose(file);
    }
}

void Sound_Play(TipoSonido sonido) {
    if (!obtenerValorFlag(FLAG_SOUND)) {
        return;
    }
    
    if (sonido < 0 || sonido >= 6 || sonidos[sonido].buffer == NULL) {
        printf("Error: Tipo de sonido no válido\n");
        return;
    }

    PlaySound((LPCSTR)sonidos[sonido].buffer, NULL, SND_MEMORY | SND_ASYNC);
    playing = sonido;
}

void Sound_Loop(TipoSonido sonido) {
    if (!obtenerValorFlag(FLAG_SOUND)) {
        return;
    }
    if (playing == sonido) {
        return; // Ya está sonando
    }
    if (sonido < 0 || sonido >= 6 || sonidos[sonido].buffer == NULL) {
        printf("Error: Tipo de sonido no válido\n");
        return;
    }

    PlaySound((LPCSTR)sonidos[sonido].buffer, NULL, SND_MEMORY | SND_ASYNC | SND_LOOP);
    playing = sonido;
}

void Sound_Stop() {
    PlaySound(NULL, NULL, SND_PURGE);
    playing = NO_SOUND;
}

TipoSonido Get_Playing_Sound() {
    return playing;
}

void Sound_Cleanup() {
    for (int i = 0; i < 6; i++) {
        if (sonidos[i].buffer != NULL) {
            free(sonidos[i].buffer);
            sonidos[i].buffer = NULL;
            sonidos[i].size = 0;
        }
    }
}