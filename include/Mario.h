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
	bool mirando_derecha = true;
	bool canMoveLeft;
	bool canMoveRight;
	int sprite_status = 0;
	Rectangle position;
	bool canJump = true;
	float velocidad;
	Powers poder;
	
	/* Constructores */
	Mario();
	    
	Mario(float posX, float posY);
};
#endif
