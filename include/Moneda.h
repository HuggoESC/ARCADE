#pragma once
#include "raylib.h"


class Moneda {
public:
    int CurrentFrame = 0;
    float animationTimer = 0.0f;
    float framespeed = 0.2f;
    int transform_status = 0;
    int transform_timer = 0;
    int transform_secuence[4] = { 180, 190, 200, 210};
    Rectangle position;
    

 
    Rectangle posicion_inicial;

    Moneda() {}

    Moneda(float x, float y) {
        position = { x, y, 16, 32 };
        posicion_inicial = position;
       
    }

    // Método reset para restaurar estado inicial
    void reset() {
        position = posicion_inicial;
      
        CurrentFrame = 0;
        animationTimer = 0.0f;
    }

    void Draw(Texture2D texture) {

        Rectangle source = { transform_secuence[transform_status], 36, 8, 16};
        Rectangle dest = position;

        dest.x += 8;
        if (transform_timer == 4)
            transform_timer = 0;

        if (transform_timer == 0) {
            if (transform_status == 1 || transform_status == 3) {
                dest.y -= 16;
                //source.y = 44;
            }
            else if (transform_status == 2) {
                dest.y -= 32;
                //source.y = 52;
            }
       
            transform_status++;


            DrawTexturePro(texture, source, dest, { 0, 0 }, 0.0f, WHITE);
        }

        transform_timer++;
    }
};