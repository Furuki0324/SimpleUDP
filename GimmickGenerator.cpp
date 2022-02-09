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
	if (mGame->GetScene() == Game::Scene::MainGame
		)
	{
		passedTime += deltaTime;
		if (passedTime >= 2.0f * generateCount)
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
	Blade* blade = new HorizontalBlade(mGame);
	blade->SetPosition(Vector2(-1000, mGame->GetRenderTarget()->GetSize().height * 0.9f));
	blade->GetPhysicsComponent()->SetDirection(Vector2(1000.0f, 0.0f));
	mGame->PlayAudio(_T("Media\\–‚‰¤° Œø‰Ê‰¹ ƒƒ“ƒ|ƒCƒ“ƒg11.wav"));
}