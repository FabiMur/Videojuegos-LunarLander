#include "partida.h"
#include "../resources/nave.h"
#include "../resources/superficie_lunar.h"
#include "../resources/asteroides.h"
#include "gestor_plataformas.h"
#include "variables_globales.h"
#include "gestor_colisiones.h"
#include "config.h"
#include "opciones.h"
#include "ai.h"
#include "math.h"
#include <stdlib.h>
#include <string.h>
#include "sonidos.h"
#include "textos_juego.h"

#define fuel_por_moneda 750
#define masa_nave 1000

#define aterrizaje_perfecto_vel 1
#define aterrizaje_brusco_vel 2
#define aterrizaje_perfecto_rot 10
#define aterrizaje_brusco_rot 20

int inicio = 0;

struct objetoFisico* nave = NULL;
struct objetoFisico* trozos_nave[4] = {NULL, NULL, NULL, NULL};

struct Dibujable* motor_debil = NULL;
struct Dibujable* motor_medio = NULL;
struct Dibujable* motor_fuerte = NULL;
struct Dibujable* terreno = NULL;
struct Asteroide* asteroides = NULL;
struct Plataforma* plataformas_partida = NULL;
uint16_t numero_plataformas = 0;
uint8_t numero_asteroides = 0;

uint8_t nave_rota = 0;

int combustible = 0;
uint16_t puntuacion_partida = 0;
uint16_t fisicas = DESACTIVADAS;

int32_t offsetTerrenoDerecha = ANCHURA_TERRENO;
int32_t offsetTerrenoIzquerda = 0;

// Calcula la coordenada Y del terreno para una posicion X determinada
static float obtener_y_terreno(float x) {
    if(!terreno || !terreno->puntos) return (float)BASE_H;

    for(uint16_t i = 0; i < terreno->num_puntos - 1; i++) {
        float x1 = terreno->puntos[i].x;
        float x2 = terreno->puntos[i+1].x;
        if((x >= x1 && x <= x2) || (x >= x2 && x <= x1)) {
            float t = (x2 - x1) != 0 ? (x - x1) / (x2 - x1) : 0;
            return terreno->puntos[i].y + t * (terreno->puntos[i+1].y - terreno->puntos[i].y);
        }
    }

    return (float)BASE_H;
}

void escalar_escena_partida(float factor_x, float factor_y){
	if(inicio == 1) {
		escalar_dibujable_en_escena_dados_ejes(terreno, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_fuerte, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_medio, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(motor_debil, factor_x, factor_y);
		escalar_dibujable_en_escena_dados_ejes(nave->objeto, factor_x, factor_y);
		for(uint8_t i = 0; i < 4; i++) {
			if(trozos_nave[i]) {
				escalar_dibujable_en_escena_dados_ejes(trozos_nave[i]->objeto, factor_x, factor_y);
			}
		}
		
		for(uint16_t i = 0; i < numero_plataformas; i++) {
			escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].linea, factor_x, factor_y);
			for(uint16_t j = 0; j < plataformas_partida[i].texto->num_caracteres; j++){
				escalar_dibujable_en_escena_dados_ejes(plataformas_partida[i].texto->caracteres[j], factor_x, factor_y);
			}
		}
	}
}

uint16_t evaluar_aterrizaje(uint16_t bonificador, uint16_t es_arista_aterrizable,
                            uint8_t* exito){
	uint16_t puntuacion = 0;
	*exito = 0;

	if(es_arista_aterrizable){
		if(nave->velocidad[1] > -aterrizaje_perfecto_vel &&
			(aterrizaje_perfecto_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_perfecto_vel) &&
			(nave->rotacion < aterrizaje_perfecto_rot ||
			nave->rotacion > 360 - aterrizaje_perfecto_rot)) {
			// Aterrizaje perfecto
			Sound_Play(SONIDO_ATERRIZAJE);

			printf("Aterrizaje perfecto\n");
			puntuacion = 50 * bonificador;
			combustible += 50;
			*exito = 1;
		}
		else if(nave->velocidad[1] > -aterrizaje_brusco_vel &&
			(aterrizaje_brusco_vel > nave->velocidad[0] &&
			nave->velocidad[0] > -aterrizaje_brusco_vel) &&
			(nave->rotacion < aterrizaje_brusco_rot ||
			nave->rotacion > 360 - aterrizaje_brusco_rot)) {
			// Aterrizaje brusco
			Sound_Play(SONIDO_ATERRIZAJE);
			printf("Aterrizaje brusco\n");
			puntuacion = 15 * bonificador;
			*exito = 1;
		} else {
			// Colision
			nave_rota = 1;
			Sound_Play(SONIDO_EXPLOSION);
			printf("Colision\n");
			printf("Colision con terreno aterrizable (mal arterrizaje)\n");
			puntuacion = 5 * bonificador;
		}
	} else {
		// Colision
		nave_rota = 1;
		Sound_Play(SONIDO_EXPLOSION);
		printf("Colision\n");
		printf("Colision con terreno no aterrizable\n");
	}
	return puntuacion;
}

static void respawn_nave(){
	Sleep(1000);
    struct Punto destino = {offsetTerrenoIzquerda + rand()%ANCHURA_TERRENO, ALTURA_SPAWN_NAVE};
	colocarDibujable(nave->objeto, destino);
	while(nave->rotacion != 0){
		if(nave->rotacion > 180){
			rotar_nave(1);
			nave->rotacion = (nave->rotacion + ANGULO_ROTACION) % 360;
		} else {
			rotar_nave(0);
			nave->rotacion = (nave->rotacion - ANGULO_ROTACION + 360) % 360;
		}
	}

    fisicas = ACTIVADAS;
    ai_iniciar();
}

void se_ha_aterrizado(){
	nave->velocidad[0] = 0;
	nave->velocidad[1] = 0;
	nave->aceleracion[0] = 0;
	nave->aceleracion[1] = 0;
	//fisicas = DESACTIVADAS;
	printf("Combustible restante: %d\n", combustible);
	if(combustible >= combustible_motor){
		respawn_nave();
	} else {
		informarFinPartida();
	}
}

void gestionar_colisiones() {
	struct Arista arista_colision = (struct Arista){0};
	uint16_t bonificador = 1;
	uint16_t es_arista_aterrizable = 0;
	uint16_t se_produce_colision = 0;

	// Comprobar colision con el terreno
	if(hay_colision(nave->objeto, terreno, &arista_colision)) {
		se_produce_colision = 1;
		es_arista_aterrizable = es_horizontal(&arista_colision);
		if(es_arista_aterrizable){

			// Si hay colision con el terreno -> evaluar si ha sido colision con plataforma
			for(uint16_t i = 0; i < numero_plataformas; i++) {
				if(hay_colision(nave->objeto, plataformas_partida[i].linea, &arista_colision)) {
					// La colision ha sido con una plataforma
					bonificador = plataformas_partida[i].bonificador;
					break;
				}
			}
		}
	}
	if(se_produce_colision == 1){
		uint8_t exito = 0;
		// Determinar tipo de aterrizaje y puntos conseguidos
		uint16_t puntos_conseguidos = evaluar_aterrizaje(bonificador, es_arista_aterrizable, &exito);
		puntuacion_partida += puntos_conseguidos;
		printf("Has conseguido %d puntos en este aterrizaje\n", puntos_conseguidos);
		if(exito){
			se_ha_aterrizado();
		} else {
			nave_rota = 1;
			centrarTrozosEnNave();
			informarFinPartida();
		}
	}
}

void dibujar_terreno(HDC hdc){
	dibujarDibujable(hdc, terreno);
	
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);	
	}
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){-ANCHURA_TERRENO,0});
	dibujarDibujable(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);	
	}
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){2*ANCHURA_TERRENO,0});
	dibujarDibujable(hdc, terreno);
	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);	
	}
	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){-ANCHURA_TERRENO,0});
}

static void dibujar_copia_asteroides(HDC hdc){
	for(uint8_t i = 0; i < numero_asteroides; i++){
			dibujarAsteroide(hdc, &asteroides[i]);
	}
	trasladar_asteroides(asteroides, numero_asteroides, (struct Punto){-ANCHURA_TERRENO, 0});
	for(uint8_t i = 0; i < numero_asteroides; i++){
			dibujarAsteroide(hdc, &asteroides[i]);
	}
	trasladar_asteroides(asteroides, numero_asteroides, (struct Punto){2*ANCHURA_TERRENO, 0});
	for(uint8_t i = 0; i < numero_asteroides; i++){
			dibujarAsteroide(hdc, &asteroides[i]);
	}
	trasladar_asteroides(asteroides, numero_asteroides, (struct Punto){-ANCHURA_TERRENO, 0});
}

void gestionar_colisiones_asteroides() {
	struct Arista arista_colision = (struct Arista){0};

	for(uint8_t i = 0; i < numero_asteroides; i++){
		struct Punto despl[3] = { {0,0}, {ANCHURA_TERRENO,0}, {-ANCHURA_TERRENO,0} };
		for(uint8_t j = 0; j < 3; j++){
			trasladarDibujable(asteroides[i].objeto, despl[j]);
			if(hay_colision(nave->objeto, asteroides[i].objeto, &arista_colision)){
				Sound_Play(SONIDO_EXPLOSION);
				nave_rota = 1;
				centrarTrozosEnNave();
				informarFinPartida();
				trasladarDibujable(asteroides[i].objeto, (struct Punto){-despl[j].x, -despl[j].y});
				return;
			}
			trasladarDibujable(asteroides[i].objeto, (struct Punto){-despl[j].x, -despl[j].y});
		}
	}
}

void dibujar_escena(HDC hdc){
	if(!nave_rota){
		dibujarDibujable(hdc, nave -> objeto);
	}else{
		for(int i = 0; i < 4; i++){
			dibujarDibujable(hdc, trozos_nave[i]->objeto);	
		}
	}

	dibujar_terreno(hdc);

 	dibujar_copia_asteroides(hdc);

	for(uint8_t i = 0; i < numero_plataformas; i++){
		dibujar_plataforma(hdc, plataformas_partida[i]);
	}

    if(obtener_estado() == JUGANDO) {
        switch(obtener_propulsor()){
			case 1:
					colocar_dibujable(motor_debil, nave -> objeto -> origen);
					dibujarDibujable(hdc, motor_debil);
					break;
			case 2:
					colocar_dibujable(motor_medio, nave -> objeto -> origen);
					dibujarDibujable(hdc, motor_medio);
					break;
			case 3:
					colocar_dibujable(motor_fuerte, nave -> objeto -> origen);
					dibujarDibujable(hdc, motor_fuerte);
					break;
			default:
					break;
        }
    }
}

void dibujarHUD(HDC hdc) {
    char buf[64];
    struct Punto origen;
    struct Texto* txt;
    int salto = ALTURA_CARACTER_MAX + 5;

    // SCORE
    sprintf(buf, "SCORE %04d", puntuacion_partida);
    origen = (struct Punto){ 10, 10 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // TIME
    extern int tiempo;  
    sprintf(buf, "TIME  %02d:%02d", tiempo/60, tiempo%60);
    origen = (struct Punto){ 10, 10 + salto * 1 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // FUEL
    sprintf(buf, "FUEL  %04d", combustible);
    origen = (struct Punto){ 10, 10 + salto * 2 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // ALTITUDE
    float alt_f = obtener_y_terreno(nave->objeto->origen.x) - nave->objeto->origen.y;
    int alt = (int)roundf(alt_f);
    sprintf(buf, "ALTITUDE           %4d", alt);
    origen = (struct Punto){ 300, 10 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // HORIZONTAL SPEED
    int vel_h = (int)roundf(nave->velocidad[0]);
    char flecha_h = vel_h >= 0 ? '>' : '<';
    sprintf(buf, "HORIZONTAL SPEED  %4d%c", abs(vel_h), flecha_h);
    origen = (struct Punto){ 300, 10 + salto * 1 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);

    // VERTICAL SPEED
    int vel_v = (int)roundf(-nave->velocidad[1]);
    char flecha_v = vel_v >= 0 ? 'v' : '^';
    sprintf(buf, "VERTICAL SPEED    %4d%c", abs(vel_v), flecha_v);
    origen = (struct Punto){ 300, 10 + salto * 2 };
    txt = crearTextoDesdeCadena(buf, origen);
    dibujar_texto(txt, hdc);
    destruir_texto(txt);
}

// Dibuja las indicaciones mientras la partida no ha comenzado
void dibujarIndicacionesInicio(HDC hdc) {
    const char* lineas[4];
    lineas[0] = combustible == 0 ? "INSERT COINS: PRESS 5" : "";
	lineas[1] = " ";
    lineas[2] = "SIDE ARROWS TO STEER";
    lineas[3] = "UP ARROW TO THRUST AND START";

    int lineCount = 0;
    for(int i=0;i<4;i++) if(strlen(lineas[i])>0) lineCount++;
    if(lineCount==0) return;

    int salto = ALTURA_CARACTER_MAX + 5;
    int altoTotal = lineCount*ALTURA_CARACTER_MAX + (lineCount-1)*5;
    int yInicio = (BASE_H - altoTotal)/2;
    int indice=0;
    for(int i=0;i<4;i++) {
        if(strlen(lineas[i])==0) continue;
        int ancho = strlen(lineas[i])*ANCHURA_CARACTER_MAX +
                    (strlen(lineas[i])-1)*SEPARACION_CARACTER;
        struct Punto o = { (BASE_W - ancho)/2, yInicio + indice*salto };
        struct Texto* t = crearTextoDesdeCadena(lineas[i], o);
        dibujar_texto(t, hdc); destruir_texto(t);
        indice++;
    }
}

void rotar_nave(uint16_t direccion){
	rotarDibujable(nave -> objeto, direccion);
	rotarDibujable(motor_debil, direccion);
	rotarDibujable(motor_medio, direccion);
	rotarDibujable(motor_fuerte, direccion);
}

void manejar_instante_partida(){
    if(fisicas == ACTIVADAS) {
		if (!nave_rota) {
			calcularFisicas(nave);
			if(nave->objeto->origen.x > offsetTerrenoDerecha) {
				struct Punto traslacion = {ANCHURA_TERRENO, 0};
				trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, traslacion);
				trasladar_asteroides(asteroides, numero_asteroides, traslacion);
				offsetTerrenoDerecha += ANCHURA_TERRENO;
				offsetTerrenoIzquerda += ANCHURA_TERRENO;
			}
			else if(nave->objeto->origen.x < offsetTerrenoIzquerda) {
				struct Punto traslacion = {-ANCHURA_TERRENO, 0};
				trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, traslacion);
				trasladar_asteroides(asteroides, numero_asteroides, traslacion);
				offsetTerrenoDerecha -= ANCHURA_TERRENO;
				offsetTerrenoIzquerda -= ANCHURA_TERRENO;
			}
		}
		calcualarFisicasAsteroides(asteroides, numero_asteroides);
		gestionar_colisiones_asteroides();
		gestionar_colisiones();
	}
}

void manejar_instante_pausa(){
	if(nave_rota){
		calcularFisicasTrozosNave(trozos_nave, 4);
		calcualarFisicasAsteroides(asteroides, numero_asteroides);
	}
}

void inicializarPartida(){
	nave_rota = 0;
    combustible = 0;
	terreno = crearDibujable(&Terreno);
	
	plataformas_partida = generar_plataformas(&Terreno, &numero_plataformas);

	offsetTerrenoDerecha = ANCHURA_TERRENO;
	offsetTerrenoIzquerda = 0;

	trasladar_superficie_lunar(terreno, plataformas_partida, numero_plataformas, (struct Punto){0, 350});

	if(obtenerValorFlag(FLAG_ASTEROIDS)){
        asteroides = generarAsteroides(terreno, &numero_asteroides);
    } else {
        asteroides = NULL;
        numero_asteroides = 0;
    }
}

void anyadirMoneda(){
	Sound_Play(SONIDO_MONEDA);
    combustible += fuel_por_moneda;
}

static void iniciarPartidaComun(){
	nave_rota = 0;
    nave = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
    nave -> objeto = crearDibujable(&Nave_Base);
    nave -> velocidad[0] = 2.5;
    nave -> velocidad[1] = 0;
    nave -> aceleracion[0] = 0;
    nave -> aceleracion[1] = 0;
    nave -> masa = masa_nave;
	nave -> rotacion = 270;

    struct Punto spawn = {offsetTerrenoIzquerda + rand()%ANCHURA_TERRENO, ALTURA_SPAWN_NAVE};
    colocarDibujable(nave->objeto, spawn);

	motor_debil = crearDibujable(&Nave_Propulsion_Minima);
	motor_medio = crearDibujable(&Nave_Propulsion_Media);
	motor_fuerte = crearDibujable(&Nave_Propulsion_Maxima);

    for(int i = 0; i < 270 / ANGULO_ROTACION; i++) {
        rotar_nave(1);
    }

	iniciarTrozosNave();

    fisicas = ACTIVADAS;
    inicio = 1;
    ai_iniciar();
    printf("Combustible inicial: %d\n", combustible);
}

void iniciarTrozosNave(){
	for(uint8_t i = 0; i < 4; i++){
		trozos_nave[i] = (struct objetoFisico*)malloc(sizeof(struct objetoFisico));
		trozos_nave[i]->velocidad[0] = (rand() % 10) - 5;
		trozos_nave[i]->velocidad[1] = (rand() % 10) - 5;
		trozos_nave[i]->aceleracion[0] = 0;
		trozos_nave[i]->aceleracion[1] = 0;
		trozos_nave[i]->masa = masa_nave / 4;
		trozos_nave[i]->rotacion = 0;
	}

	trozos_nave[0]->objeto = crearDibujable(&Trozo_Pata_Izquierda);
	trozos_nave[1]->objeto = crearDibujable(&Trozo_Pata_Derecha);
	trozos_nave[2]->objeto = crearDibujable(&Trozo_Cupula_Superior);
	trozos_nave[3]->objeto = crearDibujable(&Trozo_Base_Central);
}

void centrarTrozosEnNave(){
	for(uint8_t i = 0; i < 4; i++){
		trasladarDibujable(trozos_nave[i]->objeto, nave->objeto->origen);
	}
}

void comenzarPartida(){
	extern int tiempo;
    extern int tiempo_ms;
    tiempo = 0;
    tiempo_ms = 0;
    iniciarPartidaComun();
}
void continuarPartida(){
    iniciarPartidaComun();
}

void finalizarPartida(){
    //destruirObjetoFisico(nave);
    fisicas = DESACTIVADAS;
	cambiar_estado(PEDIR);
}

void findeJuego(){
    cambiar_estado(PEDIR);
}