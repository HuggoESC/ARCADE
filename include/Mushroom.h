#pragma once
#include "raylib.h"


class Mushroom {
public:
    int CurrentFrame = 0;
    float animationTimer = 0.0f;
    float framespeed = 0.2f;
    bool mirando_derecha;
    bool hitGround = false;

    Rectangle position;
    Rectangle pies;
    Rectangle cabeza;
    Rectangle derecha;
    Rectangle izquierda;

    float velocidadX;
    float velocidadY;
    Rectangle posicion_inicial;

    Mushroom() {}

    Mushroom(float x, float y) {
        position = { x, y, 32, 32 };
        posicion_inicial = position;
        velocidadX = 60.0f;
        velocidadY = 0;
    }

    // Método reset para restaurar estado inicial
    void reset() {
        position = posicion_inicial;
        velocidadX = 60.0f;
        velocidadY = 0;
        CurrentFrame = 0;
        animationTimer = 0.0f;
    }

    void Update(float delta) {

        // Movimiento horizontal
        position.x += (mirando_derecha ? velocidadX : -velocidadX) * delta;
        position.y += velocidadY  * delta;

        if (position.x < 0) mirando_derecha = true;
        if (position.x > 6000) mirando_derecha = false;

        pies = { position.x + 6, position.y + position.height - 6, position.width - 12, 6 };
        cabeza = { position.x + 6, position.y, position.width - 12, 6 };
        izquierda = { position.x, position.y + 6, 6, position.height - 12 };
        derecha = { position.x + position.width - 6, position.y + 6, 6, position.height - 12 };

    }

    void Draw(Texture2D texture) {

        Rectangle source = { 0, 8, 16, 16 };
       

        Rectangle dest = position;
        ///* dest.y -= 32;*/

        DrawTexturePro(texture, source, dest, { 0, 0 }, 0.0f, WHITE);

    }
};