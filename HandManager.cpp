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

	if (handsLength > 50)
	{
		if(!mYarn && isBothHandClosed) GenerateYarn();
	}
}

void HandManager::GenerateYarn()
{
	Game* game = GetGame();
	mYarn = new Yarn(game, this);
}

void HandManager::DestroyYarn()
{
	mYarn->SetState(Actor::State::EDead);
}

std::vector<float> HandManager::GetHandsPosition()
{
	std::vector<float> vector;
	Vector2 rightHandPos = rightHand->GetPosition();
	Vector2 leftHandPos = leftHand->GetPosition();

	vector.push_back(rightHandPos.x);
	vector.push_back(rightHandPos.y);
	vector.push_back(leftHandPos.x);
	vector.push_back(leftHandPos.y);

	return vector;
}