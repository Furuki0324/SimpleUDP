#pragma once
#include "Hand.h"
#include "Yarn.h"
#include "Actor.h"
#include "Game.h"

class HandManager : public Actor
{
public:
	HandManager(class Game* game, class Hand* rightHand, class Hand* leftHand);

	void UpdateActor(float deltaTime);

	void GenerateYarn();
	void DestroyYarn();

private:
	Hand* rightHand;
	Hand* leftHand;

	Yarn* mYarn;
};