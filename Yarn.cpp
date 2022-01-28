#include "Yarn.h"

Yarn::Yarn(Game* InGame, HandManager* InHm)
	:Actor(InGame)
	,hm(InHm)
{
	mSDL = new YarnSDLComponent(this);
}

Yarn::~Yarn()
{

}

void Yarn::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

