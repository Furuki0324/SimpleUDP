#pragma once

#include "Actor.h"
#include "Game.h"
#include "SDLComponent.h"

class Yarn : public Actor
{
public:
	Yarn(Game* game);
	~Yarn();

private:
	SDLComponent* mSDL;
};