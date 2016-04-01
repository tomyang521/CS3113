#include "platformer.h"

float platformer::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}


platformer::platformer() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Tom Yang - HW5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.66, 2.66, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);

	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	gravity_x = 0.0f;
	gravity_y = 1.5f*(-9.8f);

	sprites = LoadTexture("sheetRESIZED.png");
	character = LoadTexture("characters_1.png");
	state = STATE_GAME_LEVEL;

	Initial();
}

platformer::~platformer() {
	SDL_Quit();
}

void platformer::Initial() {
	SpriteSheet playerSprite = SpriteSheet(character, 12, 8, 18);
	Entity* player = new Entity();
	player->sprite = playerSprite;
	player->x = -7.75f;
	player->y = -1.0f;
	player->width = 0.2f;
	player->height = 0.2f;
	player->friction_x = 3.0f;
	player->isPlayer = true;

	entities.push_back(player);

	buildLevel();
}

void platformer::buildLevel() {
	ifstream infile("platformer.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
	}
}

bool platformer::readHeader(ifstream& stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool platformer::readLayerData(ifstream& stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val;
					}
					else {
						levelData[y][x] = 0;
					}
				}

			}
		}
	}
	return true;
}

void platformer::Update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		updateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		updateGameLevel(elapsed);
		break;
	}
}

void platformer::updateMainMenu(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_GAME_LEVEL;
				break;
			}
		}
	}

}

void platformer::updateGameLevel(float elapsed) {
	// keyboard controls
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_SPACE]) {
		if (!entities[0]->isJumping && entities[0]->collidedBottom) {
			entities[0]->jump();
		}

	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		if (!entities[0]->collidedRight){
			SpriteSheet playerSprite = SpriteSheet(character, 12, 8, 30);
			entities[0]->sprite = playerSprite;
			entities[0]->walkRight();
		}
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		if (!entities[0]->collidedLeft){
			SpriteSheet playerSprite = SpriteSheet(character, 12, 8, 18);
			entities[0]->sprite = playerSprite;
			entities[0]->walkLeft();
		}
	}
	else {
		entities[0]->idle();
	}
}

void platformer::FixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->collidedBottom) {
			entities[i]->isJumping = false;
			entities[i]->velocity_y = 0.0f;
		}

		if (entities[i]->collidedTop){
			entities[i]->velocity_y = 0.0f;
		}

		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * TIME;
			entities[i]->velocity_y += gravity_y * TIME;
		}

		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, TIME * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, TIME * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * TIME;
		entities[i]->velocity_y += entities[i]->acceleration_y * TIME;

		entities[i]->y += entities[i]->velocity_y * TIME;

		collisionY(entities[i]);

		entities[i]->x += entities[i]->velocity_x * TIME;

	}
}

void platformer::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	renderGameLevel();
	SDL_GL_SwapWindow(displayWindow);
}


void platformer::renderGameLevel() {
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float xloc = -entities[0]->x;
	float yloc = -entities[0]->y;

	if (yloc > 0.4){
		yloc = 0.4;
	}
	if (xloc > 8.0){
		xloc = 8.0;
	}
	if (xloc < -8.0){
		xloc = -8.0;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(xloc, yloc, 0.0f);

	glBindTexture(GL_TEXTURE_2D, sprites);
	glEnable(GL_TEXTURE_2D);

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);


	glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}
}

bool platformer::UpdateAndRender() {
	float test = 0.01;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0){
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), {
					u, v,
					u, v + (spriteHeight)-test,
					u + spriteWidth - test, v + (spriteHeight)-test,
					u + spriteWidth - test, v
				});
			}
		}
	}

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > TIME* MAX) {
		fixedElapsed = TIME* MAX;
	}
	while (fixedElapsed >= TIME) {
		fixedElapsed -= TIME;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}


bool platformer::isSolid(unsigned char tile) {
	switch (tile) {
	case 145:
		return true;
		break;
	case 132:
		return true;
		break;
	case 124:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void platformer::worldToTileCoordinates(float worldX, float worldY, int*gridX, int*gridY) {
	*gridX = (int)(((worldX * 16.0f) / TILE_SIZE) + (128.0f * 16.0f / 2.0f)) / 16.0f;
	*gridY = (int)(((worldY * 16.0f) / -TILE_SIZE) + (32.0f * 16.0f / 2.0f)) / 16.0f;
}

void platformer::collisionY(Entity *entity) {
	int tileX;
	int tileY;

	if (entity->y < -2.3f)
	{
		entity->y = -1.0f;
		entity->x = -7.75f;
		entity->collidedBottom = false;
		entity->collidedTop = false;
		entity->collidedLeft = false;
		entity->collidedRight = false;
	}
	worldToTileCoordinates(entity->x, entity->y - entity->height*0.5, &tileX, &tileY);

	if (isSolid(levelData[tileY][tileX])) {
		entity->y = -1.9f;
		entity->collidedBottom = true;
	}
}
