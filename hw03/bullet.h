#pragma once

#include "spritesheet.h"

class Bullet {
public:
	Bullet();
	Bullet(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed);
	void Update(float elapsed);
	void Render();

	SpriteSheet sprite;

	float scale;
	float x;
	float y;
	float rotation;
	float speed;
	float visible;
	bool playerBullet;
};