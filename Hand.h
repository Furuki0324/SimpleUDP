#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "HandSpriteComponent.h"
#include "UDPSocket.h"

class Hand :public Actor
{
public:
	Hand(class Game* game, int port, bool right = true);
	~Hand();
	void UpdateActor(float deltaTime);

	UDPSocket* GetUDPSocket();
	
private:
	class HandSpriteComponent* hsc;
	class UDPSocket* socket;
};