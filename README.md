# Videojuegos-LunarLander

Este proyecto es una implementación en C del clásico **Lunar Lander** desarrollada para la asignatura de Videojuegos. Utiliza la API de Windows para mostrar gráficos y reproducir sonido.

## Características principales

- **Física 2D**: controla una nave espacial con gravedad, impulso y rotación realistas.
- **Escenarios generados** con plataformas de aterrizaje que otorgan bonificadores de puntuación.
- **Sistema de combustible y puntuación** con tablas de tiempo, velocidad y altitud en el HUD.
- **Obstáculos opcionales**: activa cintas de asteroides para aumentar el desafío.
- **IA opcional**: un piloto automático que aterriza por ti cuando la opción `AI` está habilitada.
- **Efectos de sonido**: explosiones, propulsión y otros efectos (pueden desactivarse en las opciones).
- **Modos de pausa y fin de partida** con menús.

## Controles

Durante el juego las teclas son:

| Tecla                   | Acción                                    |
|-------------------------|-------------------------------------------|
| Flecha **Arriba**       | Activar propulsor (consume combustible)   |
| Flecha **Izquierda**    | Girar la nave a la izquierda              |
| Flecha **Derecha**      | Girar la nave a la derecha                |
| **5**                   | Insertar moneda (añade combustible)       |
| **ESC**                 | Pausar la partida                         |

En los menús se usan las flechas **Arriba/Abajo** para cambiar de opción y **Enter** para confirmar. En el menú de espera, al pulsar **PLAY**, la barra **Espaciadora** comienza una partida si hay crédito.

## Opciones de juego

Desde el menú `OPTIONS` es posible activar o desactivar diferentes características:

- **SOUND**: habilita o silencia todos los efectos de sonido.
- **ASTEROIDS**: activa o desactiva los cinturones de asteroides.
- **AI**: cuando está en `ON`, el piloto automático controla la nave.
- **BACK**: vuelve al menú principal.

Las opciones se manipulan con las **flechas**.

## Compilación

El proyecto incluye un `MakeFile` para compilar con MinGW en Windows. Basta con ejecutar:

```bash
make -f MakeFile
```

El ejecutable generado será `lunar_lander.exe`.

## Ejecución

Una vez compilado, ejecuta el binario para disfrutar de tu propio **Lunar Lander**. ¡Gestiona el combustible, esquiva los asteroides y aterriza con suavidad para conseguir la mayor puntuación!
