#pragma once
#include "Actor.h"

class GimmickGenerator : public Actor
{
public:
	GimmickGenerator(class Game* game);
	void UpdateActor(float deltaTime) override;

	void GenerateBlock();
	void GenerateBlade();

private:
	float passedTime;
	int generateCount;
};