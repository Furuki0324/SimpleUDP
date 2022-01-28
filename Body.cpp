#include "Body.h"
#include "Game.h"

Body::Body(Game* game, u_short port)
	:Actor(game)
	,udpComponent(nullptr)
{
	udpComponent = new BodyUDPComponent(this, port);
}

void Body::UpdateActor(float deltaTime) {}