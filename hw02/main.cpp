#include <math.h>
#include <stdlib.h>
#include <random>
#include <iostream>
#include <SDL_mixer.h>

#include "Entity.h"

SDL_Window* displayWindow;
using namespace std;

void Setup(Entity* ball){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Tom Yang - HW04", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	ball->xDirection = 0.0f;
	ball->yDirection = 0.0f;
}

void resetBall(Entity* ball){
	ball->xPosition = 0.0f;
	ball->yPosition = 0.0f;

	float randomX = ((float)rand() / (float)RAND_MAX) - 0.5f;
	float randomY = ((float)rand() / (float)RAND_MAX) - 0.5f;

	if (randomX < 0.0f && randomX > -0.3f){
		ball->xDirection = randomX - 0.25f;
	}
	else if (randomX > 0.0f && randomX < 0.3f){
		ball->xDirection = randomX + 0.25f;
	}
	else{
		ball->xDirection = randomX;
	}
	if (randomY < 0.0f && randomY > -0.3f){
		ball->yDirection = randomY - 0.25f;
	}
	else if (randomY > 0.0f && randomY < 0.3f){
		ball->yDirection = randomY + 0.25f;
	}
	else{
		ball->yDirection = randomY;
	}
}

bool startGame(Entity* player1, Entity* player2, Entity* ball){
	SDL_Event event;

	player1->yDirection = 0.0f;
	player2->yDirection = 0.0f;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}

		else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
			resetBall(ball);
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]){
		player1->yDirection = 1.75f;
	}
	if (keys[SDL_SCANCODE_S]){
		player1->yDirection = -1.75f;
	}
	if (keys[SDL_SCANCODE_UP]){
		player2->yDirection = 1.75f;
	}
	if (keys[SDL_SCANCODE_DOWN]){
		player2->yDirection = -1.75f;
	}
	return true;
}

bool checkPaddleCollision(Entity* paddle, Entity* ball){
	return ((ball->xPosition < (paddle->xPosition + paddle->width / 800.0f)) &&
		((ball->xPosition + ball->width / 800.0f) > paddle->xPosition) &&
		(ball->yPosition < (paddle->yPosition + paddle->height / 600.0f)) &&
		((ball->yPosition + ball->height / 600.0f) > paddle->yPosition));
}

bool checkWallCollision(Entity* ball){
	return (((ball->yPosition + ball->height / 600.0f >= 1.0f) && ball->yDirection > 0) || (ball->yPosition <= -1.0f && ball->yDirection < 0));
}


void increaseSpeed(Entity* ball, float elapsed){
	float time = elapsed / 75;
	if (ball->xDirection > 0.0f)
		ball->xDirection += time;
	else if (ball->xDirection < 0.0f)
		ball->xDirection -= time;
	if (ball->yDirection > 0.0f)
		ball->yDirection += time;
	else if (ball->yDirection < 0.0f)
		ball->yDirection -= time;
}

void Update(Entity* player1, Entity* player2, Entity* ball, float elapsed, Mix_Chunk *hit){
	player1->yPosition += player1->yDirection * elapsed;
	player2->yPosition += player2->yDirection * elapsed;
	ball->xPosition += ball->xDirection * elapsed;
	ball->yPosition += ball->yDirection * elapsed;

	increaseSpeed(ball, elapsed);

	if (checkWallCollision(ball)) {
		ball->yDirection = -ball->yDirection;
		Mix_PlayChannel(-1, hit, 0);
	}

	if (checkPaddleCollision(player1, ball)){
		if (ball->xDirection < 0){
			ball->xDirection = -ball->xDirection;
			Mix_PlayChannel(-1, hit, 0);
		}

	}
	if (checkPaddleCollision(player2, ball)){
		if (ball->xDirection > 0){
			ball->xDirection = -ball->xDirection;
			Mix_PlayChannel(-1, hit, 0);
		}
	}

	if (ball->xPosition > 1.33f){
		resetBall(ball);
		player1->score++;
	}

	if (ball->xPosition < -1.33f){
		resetBall(ball);
		player2->score++;	
	}

	if (player1->yPosition > 1.0f - player1->height / 600.0f) {
		player1->yPosition = 1.0f - player1->height / 600.0f;
	}
	if (player2->yPosition > 1.0f - player2->height / 600.0f) {
		player2->yPosition = 1.0f - player2->height / 600.0f;
	}
	if (player1->yPosition < -1.0f){
		player1->yPosition = -1.0f;
	}
	if (player2->yPosition < -1.0f){
		player2->yPosition = -1.0f;
	}
}

void Render(Entity* player1, Entity* player2, Entity* ball){
	if (player1->score == player2->score)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	else if (player1->score > player2->score)
		glClearColor(0.0f, 0.6f, 0.0f, 1.0f);
	else if (player1->score < player2->score)
		glClearColor(0.0f, 0.0f, 0.6f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	player1->LoadTexture("someTexture.png");
	player2->LoadTexture("someTexture.png");
	ball->LoadTexture("someTexture.png");

	player1->DrawSprite();
	player2->DrawSprite();
	ball->DrawSprite();

	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
	Entity* player1 = new Entity(-1.31f, 0.0f, 50.0f, 200.0f, 0.0f);
	Entity* player2 = new Entity(1.25f, 0.0f, 50.0f, 200.0f, 0.0f);
	Entity* ball = new Entity(0.0f, 0.0f, 50.0f, 50.0f, 0.0f);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *hit;
	hit = Mix_LoadWAV("hitSound.wav");

	Mix_Music *song;
	song = Mix_LoadMUS("dreaming.mp3");

	Mix_PlayMusic(song, -1);

	Setup(ball);

	float lastFrameTicks = 0.0f;

	while (startGame(player1, player2, ball)) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		startGame(player1, player2, ball);
		Update(player1, player2, ball, elapsed, hit);
		Render(player1, player2, ball);
	}
	SDL_Quit();
	return 0;
}