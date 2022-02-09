#include "GimmickGenerator.h"
#include "Game.h"
#include "Block.h"
#include "Blade.h"
#include "PhysicsComponent.h"

GimmickGenerator::GimmickGenerator(Game* game)
	:Actor(game)
	,passedTime(0.0f)
	,generateCount(1)
{

}

void GimmickGenerator::UpdateActor(float deltaTime)
{
	passedTime += deltaTime;
	if (passedTime >= 5.0f * generateCount)
	{
		if (generateCount % 4 == 0)
		{
			GenerateBlade();
		}
		else
		{
			GenerateBlock();
		}
		++generateCount;
	}
}

void GimmickGenerator::GenerateBlock()
{
	Block* block = new Block(mGame, 30, 30);
	if (generateCount % 2 == 0)
	{
		block->SetPosition(Vector2(100, 200));
		block->GetPhysicsComponent()->AddImpact(Vector2(300, -300));
		if (generateCount % 5 == 0)
		{
			block = new Block(mGame, 50, 50);
			block->SetPosition(Vector2(900, 200));
			block->GetPhysicsComponent()->AddImpact(Vector2(-300, -300));
		}
	}
	else
	{
		block->SetPosition(Vector2(900, 200));
		block->GetPhysicsComponent()->AddImpact(Vector2(-300, -300));
		if (generateCount % 5 == 0)
		{
			block = new Block(mGame, 50, 50);
			block->SetPosition(Vector2(100, 200));
			block->GetPhysicsComponent()->AddImpact(Vector2(300, -300));
		}
	}
	mGame->PlayAudio(_T("Media\\–‚‰¤° Œø‰Ê‰¹ •¨‰¹16.wav"));
}

void GimmickGenerator::GenerateBlade()
{
	mGame->PlayAudio(_T("Media\\–‚‰¤° í“¬16.wav"));
}