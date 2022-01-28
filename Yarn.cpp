#include "Yarn.h"

Yarn::Yarn(Game* InGame, HandManager* InHm)
	:Actor(InGame)
	,hm(InHm)
{
	mSDL = new SDLComponent(this, this);
}

Yarn::~Yarn()
{

}

void Yarn::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

