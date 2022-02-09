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

	//右手のコリジョン
	class CircleComponent* rHandCircleComponent;
	//左手のコリジョン
	class CircleComponent* lHandCircleComponent;

	Vector2 prevRightHandPos, prevLeftHandPos;		//1フレーム前の位置
	Vector2 rightHandVector, leftHandVector;		//フレーム間の移動量
};