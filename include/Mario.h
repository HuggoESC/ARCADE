#pragma once
#ifndef A2DD_H
#define A2DD_H
#include "raylib.h"
#include <math.h>

class Mario
{
public:
	enum Powers {
		BASE = 0,
		SETA = 1,
		FLOR = 2,
		ESTRELLA = 3
	};

	bool mirando_derecha = true;
	bool canMoveLeft;
	bool canMoveRight;
	int sprite_status = 0;
	Rectangle position;
	bool canJump = true;
	Vector2 velocidad;
	Powers poder;

	Vector2 distancia;

	/* Constructores */
	Mario();

	Mario(float posX, float posY);

	void MoveX(float cantidad, bool hitObstacle) {
		distancia.x += cantidad;

		int move = round(cantidad);

		if (move != 0) {
			distancia.x -= move;

			// -1 -> Izquierda | 1  -> Derecha
			int direccion = move < 0 ? -1 : 1;

			while (move != 0) {
				if (!hitObstacle) {
					position.x += direccion;
					move -= direccion;
				}
				else {
					break;
				}
			}
		}
	};
};
#endif