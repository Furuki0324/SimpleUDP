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
	void UpdateOnTitle(float deltaTime);
	void UpdateOnMainGame(float deltaTime);
	void UpdateOnResult(float deltaTime);
	class BodyUDPComponent* udpComponent;
	class BodyD2DDrawComponent* d2dComponent;

	//�E��̃R���W����
	class CircleComponent* rHandCircleComponent;
	//����̃R���W����
	class CircleComponent* lHandCircleComponent;

	Vector2 prevRightHandPos, prevLeftHandPos;		//1�t���[���O�̈ʒu
	Vector2 rightHandVector, leftHandVector;		//�t���[���Ԃ̈ړ���
};