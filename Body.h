#pragma once
#include "Actor.h"

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
	class BodyD2DDrawComponent* d2dComponent;

	//右手のコリジョン
	class CircleComponent* rHandCircleComponent;
	//左手のコリジョン
	class CircleComponent* lHandCircleComponent;
};