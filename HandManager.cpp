#include "HandManager.h"

HandManager::HandManager(Game* InGame, Hand* InRightHand, Hand* InLeftHand)
	:Actor(InGame)
	,rightHand(InRightHand)
	,leftHand(InLeftHand)
	,mYarn(nullptr)
{
	
}

void HandManager::UpdateActor(float deltaTime)
{
	
}

void HandManager::GenerateYarn()
{
	Game* game = GetGame();
	mYarn = new Yarn(game);
}

void HandManager::DestroyYarn()
{
	mYarn->SetState(Actor::State::EDead);
}