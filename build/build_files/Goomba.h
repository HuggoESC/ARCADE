#pragma once
#include "raylib.h"


class Goomba {
public:
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

    Goomba() {}

    Goomba(float x, float y) {
        position = { x, y, 32, 32 };
        posicion_inicial = position;
        velocidad = 30.0f;
        velocidadY = 0;
        mirando_derecha = false;
        activo = true;
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
    }

    void Update(float delta) {
        if (!activo) return;

        // Movimiento horizontal
        position.x += (mirando_derecha ? velocidad : -velocidad) * delta;
        position.y += velocidadY * delta;

        if (position.x < 0) mirando_derecha = true;
        if (position.x > 6000) mirando_derecha = false;

        // Animación
        animationTimer += delta;
        if (animationTimer >= framespeed) {
            CurrentFrame = (CurrentFrame + 1) % 2;
            animationTimer = 0.0f;
        }

        pies = { position.x + 6, position.y + position.height - 6, position.width - 12, 6 };
        cabeza = { position.x + 6, position.y, position.width - 12, 6 };
        izquierda = { position.x, position.y + 6, 6, position.height - 12 };
        derecha = { position.x + position.width - 6, position.y + 6, 6, position.height - 12 };
    }

    void Draw(Texture2D enemyTexture) {
        if (!activo) return;

        Rectangle source = { 18.0f * CurrentFrame, 16, 16, 16 };
        if (!mirando_derecha) source.width = -16;

        Rectangle dest = position;
        ///* dest.y -= 32;*/

        DrawTexturePro(enemyTexture, source, dest, { 0, 0 }, 0.0f, WHITE);

        // 🔧 OPCIONAL: Dibujar sub-hitboxes para depuración visual
        // (coméntalo si no lo necesitas)
        DrawRectangleLinesEx(cabeza, 1, BLUE);
        DrawRectangleLinesEx(pies, 1, RED);
        DrawRectangleLinesEx(izquierda, 1, GREEN);
        DrawRectangleLinesEx(derecha, 1, YELLOW);
    }
};