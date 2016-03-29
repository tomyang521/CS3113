#include "spaceinvaders.h"

int main(int argc, char *argv[]) {
	SpaceInvaders game;
	while (!(game.UpdateAndRender())){}
	return 0;
}