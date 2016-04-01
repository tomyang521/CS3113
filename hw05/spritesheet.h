#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet{
public:
	SpriteSheet();
	SpriteSheet(GLuint textureID, unsigned int spriteX, unsigned int spriteY, unsigned int index);

	void Draw(float width, float height, float x, float y, float rotation);


	GLuint textureID;
	unsigned int spriteX;
	unsigned int spriteY;
	unsigned int index;
};

GLuint LoadTexture(const char *image_path);