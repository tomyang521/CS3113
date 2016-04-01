#include "platformer.h"

SDL_Window* displayWindow;
int main(int argc, char *argv[])
{
	platformer app;
	while (!app.UpdateAndRender()) {}
	return 0;
}