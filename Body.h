#pragma once
#include "Actor.h"
#include "Collision.h"

class Body :public Actor
{
public:
	Body(class Game* game, int port);
	//~Body();
	void UpdateActor(float deltaTime);
	class BodyUDPComponent* GetUDPComponent();

private:
	class BodyUDPComponent* udpComponent;
	class BodySDLComponent* sdlComponent;
};