#include "Game.h"
#include <iostream>

const unsigned int window_width = 640;
const unsigned int window_height = 480;



int main(int argc, char** argv)
{
	Game game = Game();

	bool initSDL = game.InitializeSDL();

	bool initD2D = game.InitializeDirect2D();

	if (initSDL && initD2D)
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}