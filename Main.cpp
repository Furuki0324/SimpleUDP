#include "Game.h"

int main(int argc, char** argv)
{
	Game game(640,480);

	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}