#include "Entity.h"

class Player : public Entity {
public:
	Player(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float direction_x = 0);
	void Update(float elapsed);
};