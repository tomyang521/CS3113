#pragma once

#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Entity.h"

#define TIME 0.01f
#define MAX 1

#define SPRITE_COUNT_X 5
#define SPRITE_COUNT_Y 7
#define TILE_SIZE 0.2f
using namespace std;

class platformer{
public:
	platformer();
	~platformer();

	void Initial();

	void Update(float elapsed);
	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void FixedUpdate();

	void Render();
	void renderMainMenu();
	void renderGameLevel();

	bool UpdateAndRender();

	void buildLevel();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	void loadLevel();

	bool isSolid(unsigned char tile);
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	void platformer::collisionY(Entity* entity);

	float lerp(float v0, float v1, float t);

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	unsigned int mapWidth;
	unsigned int mapHeight;

	float gravity_x;
	float gravity_y;
	unsigned char** levelData;

	SDL_Window* displayWindow;

	GLuint character;
	GLuint sprites;

	vector<Entity*> entities;
	vector<float> vertexData;
	vector<float> texCoordData;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL };
	int state;
};