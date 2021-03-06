#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include <math.h>
#include <string>
#include <SDL_mixer.h>

using namespace std;

class SpaceInvaders{
public:
	SpaceInvaders();
	~SpaceInvaders();
	void shootBullet(Mix_Chunk * chunk);
	
	void startGame();
	bool isColliding(const Entity& e1, const Bullet& e2);
	bool debugColliding(const Entity& e1, const Bullet& e2);
	void update(float elapsed);
	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void updateGameOver(float elapsed);
	void updateGameStageCleared(float elapsed);
	void render();
	void renderMainMenu();
	void renderGameLevel();
	void renderGameOver();
	void renderGameStageCleared();
	bool UpdateAndRender();
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	//int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	vector<Entity*> entities;
	vector<Entity*> defences;

	Bullet playerBullet;

	GLuint spriteSheetTexture;
	GLuint fontSheetTexture;
	GLuint logoTexture;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state;
	int score;
	int roundScore;
	int highScore;
	float spriteDeathDelay;
	float playerDeathDelay;
	float delay;

	bool playerDead;
};