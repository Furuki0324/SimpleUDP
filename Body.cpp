#include "Body.h"
#include "Game.h"
#include "BodyUDPComponent.h"
#include "BodySDLComponent.h"
#include "BodyD2DDrawComponent.h"

Body::Body(Game* game, int port)
	:Actor(game)
	,udpComponent(nullptr)
	,sdlComponent(nullptr)
{
	udpComponent = new BodyUDPComponent(this, port, 1);
	//sdlComponent = new BodySDLComponent(this, game);
	d2dComponent = new BodyD2DDrawComponent(this);
}

void Body::UpdateActor(float deltaTime) 
{
}

BodyUDPComponent* Body::GetUDPComponent()
{
	return udpComponent;
}