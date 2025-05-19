#pragma once
#ifndef A2DD_H
#define A2DD_H
#include "raylib.h"

class Mario
{
public:
	enum Powers {
		BASE = 0,
		SETA = 1,
		FLOR = 2,
		ESTRELLA = 3
	};

	bool hasPlayedDieSound = false;
	bool mirando_derecha/* = true*/;
	bool canMoveLeft;
	bool canMoveRight;
	int sprite_status /*= 0*/;
	Rectangle position;
	bool canJump/* = true*/;
	float velocidad;
	float animTimer;
	/*const float animSpeed = 0.15f;*/
	Powers poder;
	bool isDead = false;
	bool deathAnimationInProgress = false;
	float deathVelocity = -350.0f;
	float deathAnimTimer = 0.0f;

	/* Constructores */
	Mario() {};

	Mario(float posX, float posY)
	{

		position = { posX, posY, 24, 32 };
		velocidad = 0;
		poder = BASE;
		mirando_derecha = true;
		canMoveLeft = true;
		canMoveRight = true;
		sprite_status = 0;
		canJump = true;
		animTimer = 0.0f;
		

	}
	~Mario(){}

};
#endif
