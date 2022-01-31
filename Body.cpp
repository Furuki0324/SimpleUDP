#include "Body.h"
#include "Game.h"
#include "BodyUDPComponent.h"
#include "BodySDLComponent.h"

Body::Body(Game* game, int port)
	:Actor(game)
	,udpComponent(nullptr)
	,sdlComponent(nullptr)
{
	udpComponent = new BodyUDPComponent(this, port);
	sdlComponent = new BodySDLComponent(this);
}

void Body::UpdateActor(float deltaTime) {}

BodyUDPComponent* Body::GetUDPComponent()
{
	return udpComponent;
}