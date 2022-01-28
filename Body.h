#pragma once
#include "Actor.h"
#include "BodyUDPComponent.h"

class Body :public Actor
{
public:
	Body(class Game* game, u_short port);
	//~Body();
	void UpdateActor(float deltaTime);

private:
	class BodyUDPComponent* udpComponent;
};