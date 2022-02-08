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

	//�E��̃R���W����
	class CircleComponent* rHandCircleComponent;
	//����̃R���W����
	class CircleComponent* lHandCircleComponent;
};