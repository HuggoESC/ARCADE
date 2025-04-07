#pragma once
#ifndef MARIO_H
#define MARIO_H
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

	bool mirando_derecha = true;
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
