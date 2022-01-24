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
	Vector2 rightHandPos = rightHand->GetPosition();
	Vector2 leftHandPos = leftHand->GetPosition();

	bool isRightHandClosed = rightHand->isHandClosed();
	bool isLeftHandClosed = leftHand->isHandClosed();
	bool isBothHandClosed = isRightHandClosed && isLeftHandClosed;

	float handsLength = (rightHandPos - leftHandPos).Length();

	if (handsLength < 50)
	{
		if(!mYarn && isBothHandClosed) GenerateYarn();
	}
	else if (mYarn)
	{
		DestroyYarn();
	}
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