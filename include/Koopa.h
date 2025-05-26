#pragma once
#include "raylib.h"


class Koopa {
public:
    enum KoopaState {
        CAMINANDO,
        CONCHA_QUIETA,
        CONCHA_MOVIENDOSE
    };

    int CurrentFrame = 0;
    float animationTimer = 0.0f;
    float framespeed = 0.2f;
    bool mirando_derecha;
    bool activo;
    bool hitGround = false;

    Rectangle position;

    // Sub-hitboxes para detección de colisiones con Mario
    Rectangle pies;
    Rectangle cabeza;
    Rectangle derecha;
    Rectangle izquierda;

    float velocidad;
    float velocidadY;

    Rectangle posicion_inicial;

    KoopaState estado;

    Koopa() {}

    Koopa(float x, float y) {
        position = { x, y, 32, 32 };
        posicion_inicial = position;
        velocidad = 30.0f;
        mirando_derecha = false;
        velocidadY = 0;
        activo = true;
        estado = CAMINANDO;
    }

    // Método reset para restaurar estado inicial
    void reset() {
        position = posicion_inicial;
        velocidad = 30.0f;
        mirando_derecha = false;
        activo = true;
        velocidadY = 0;
        CurrentFrame = 0;
        animationTimer = 0.0f;
        estado = CAMINANDO;
    }

    void Update(float delta) {
        if (!activo) return;

        switch (estado) {
        case CAMINANDO:
            position.x += (mirando_derecha ? velocidad : -velocidad) * delta;
            position.y += velocidadY * delta;

            if (position.x < 0) mirando_derecha = true;
            if (position.x > 6000) mirando_derecha = false;

            animationTimer += delta;
            if (animationTimer >= framespeed) {
                CurrentFrame = (CurrentFrame + 1) % 2;
                animationTimer = 0.0f;
            }
            break;

        case CONCHA_QUIETA:
            // No se mueve, pero igual puedes animar si quieres
            break;

        case CONCHA_MOVIENDOSE:
            position.x += (mirando_derecha ? 200 : -200) * delta * 1.5f;  // más rápido
            break;
        }

        pies = { position.x, position.y + position.height - 4, position.width, 4 };
        cabeza = { position.x, position.y, position.width, 4 };
        izquierda = { position.x, position.y + 6, 4, position.height - 12 };
        derecha = { position.x + position.width - 6, position.y + 6, 4, position.height - 12 };
    }

    void Draw(Texture2D enemyTexture) {
        if (!activo) return;

        Rectangle source;
        Rectangle dest;

        if (estado == CAMINANDO) {
            source = { 18.0f * (float)CurrentFrame, 112, 16, 24 };
            dest = { position.x, position.y - 16, 32, 48 };
            if (mirando_derecha) source.width = -16;
        }
        else {
            source = { 72, 120, 16, 14 }; // concha sprite (ajusta coordenadas según tu spritesheet)
            dest = { position.x, position.y+6, 32, 28 };
        }

        DrawTexturePro(enemyTexture, source, dest, { 0, 0 }, 0.0f, WHITE);

        //DrawRectangleLinesEx(cabeza, 1, BLUE);
        //DrawRectangleLinesEx(pies, 1, RED);
        //DrawRectangleLinesEx(izquierda, 1, GREEN);
        //DrawRectangleLinesEx(derecha, 1, YELLOW);
    }
};