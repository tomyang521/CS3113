#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SpriteSheet.h"

class Entity {
public:
	Entity();
	~Entity();

	SpriteSheet sprite;

	float x = 0.0f;
	float y = 0.0f;
	float scale = 0.0f;
	float rotation = 0.0f;

	float width = 0.2f;
	float height = 0.2f;

	// movement
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	float acceleration_x = 0.0f;
	float acceleration_y = 0.0f;
	float friction_x = 0.0f;
	float friction_y = 0.0f;
	float mass = 0.0f;

	void jump();
	void walkRight();
	void walkLeft();
	void idle();

	// collision
	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;
	bool isStatic = false;

	bool collidesWith(Entity* entity);

	bool isPlayer = false;
	bool isEnemy = false;

	bool isDead = false;
	bool isJumping = false;
	bool faceLeft = false;

	void Render();


};