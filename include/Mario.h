#pragma once
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

	enum EstadoMario {
		NORMAL,
		TRANSFORMANDOSE,
		TOCANDO_BANDERA,
		BAJANDO_BANDERA,
		CAMINANDO_CASTILLO,
		LEVEL_COMPLETED
	};

	EstadoMario estado = NORMAL;

	bool hasPlayedDieSound = false;
	bool mirando_derecha/* = true*/;
	int sprite_status /*= 0*/;
	int transform_status = 0;
	int transform_timer = 0;
	int transform_secuence[10] = {0, 18, 0, 18, 0, 18, 36, 0, 18, 36 };
	int inverse_transform_secuence[10] = {36, 18, 0, 36, 18, 0, 18, 0, 18, 0 };

	Rectangle position;

	Rectangle pies;
	Rectangle cabeza;
	Rectangle derecha;
	Rectangle izquierda;

	bool canJump /* = true*/;
	bool isJumping = false;
	float jumpTime = 0.0f;
	float maxjumptime = 0.25f;
	float velocidad;
	float velocidadX = 0.0f;
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
		position = { posX, posY, 32, 32 };
		
		pies = { posX + 4, posY + 30, 24, 1 };
		cabeza = { posX + 4, posY, 24, 1 };
		derecha = { posX + 30, posY + 4, 1, 24 };
		izquierda = { posX, posY + 4, 1, 24 };

		velocidad = 0;
		poder = BASE;
		mirando_derecha = true;
		sprite_status = 0;
		canJump = true;
		animTimer = 0.0f;
		
	}

	~Mario(){}


	/* Funciones */
	void SetX(float distance) {
		position.x = distance;
		ActualizarHitboxes();

	}

	void SetY(float distance) {
		position.y = distance;
		ActualizarHitboxes();
	}

	
	void MoveX(float distance) {
		position.x += distance;
		ActualizarHitboxes();
	}

	void MoveY(float distance) {
		position.y += distance;
		ActualizarHitboxes();
	}

	void ActualizarHitboxes() {
		pies = { position.x + 4, position.y + position.height - 4, position.width - 8, 4 };
		cabeza = { position.x + 4, position.y, position.width - 8, 4 };

		if (poder == SETA) {
			izquierda = { position.x, position.y + 6, 4, position.height - 12 };
			derecha = { position.x + position.width - 4, position.y + 6, 4, position.height - 12 };
		}
		else {
			
			izquierda = { position.x, position.y + 4, 4, position.height - 8 };
			derecha = { position.x + position.width - 4, position.y + 4, 4, position.height - 8 };
		}
	}
};
