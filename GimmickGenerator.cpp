#include "GimmickGenerator.h"
#include "Game.h"
#include "Block.h"
#include "Blade.h"
#include "PhysicsComponent.h"

GimmickGenerator::GimmickGenerator(Game* game)
	:Actor(game)
	,passedTime(0.0f)
	,previousTime(0.0f)
	,generateCount(0)
{

}

void GimmickGenerator::UpdateActor(float deltaTime)
{
	int interval;
	switch (mGame->GetScene())
	{
	case Game::Scene::Title:
		break;

	case Game::Scene::MainGame:
		passedTime += deltaTime;
		interval = 2.0f - 0.5f * (generateCount / 15);
		if (interval <= 0.5f)
		{
			interval = 0.5f;
		}
		if (passedTime >= previousTime + interval)
		{
			GenerateBlock();
			GenerateBlade();
			++generateCount;

			previousTime = passedTime;
		}
		break;

	case Game::Scene::Result:
		passedTime = 0;
		previousTime = 0;
		generateCount = 0;
		break;
	}
}

void GimmickGenerator::GenerateBlock()
{
	Block* block = new Block(mGame, 80, 80);
	if (generateCount % 2 == 0)
	{
		block->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width * -0.1f, mGame->GetRenderTarget()->GetSize().height * 0.6f));
		block->GetPhysicsComponent()->AddImpact(Vector2(400, -300));
		if (generateCount % 5 == 0)
		{
			block = new Block(mGame, 80, 80);
			block->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width * 1.1f, mGame->GetRenderTarget()->GetSize().height * 0.6f));
			block->GetPhysicsComponent()->AddImpact(Vector2(-170, -550));
		}
	}
	else
	{
		block->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width * 1.1f, mGame->GetRenderTarget()->GetSize().height * 0.6f));
		block->GetPhysicsComponent()->AddImpact(Vector2(-400, -300));
		if (generateCount % 5 == 0)
		{
			block = new Block(mGame, 80, 80);
			block->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width * -0.1f, mGame->GetRenderTarget()->GetSize().height * 0.6f));
			block->GetPhysicsComponent()->AddImpact(Vector2(170, -550));
		}
	}
	mGame->PlayAudio(_T("Media\\–‚‰¤° Œø‰Ê‰¹ •¨‰¹16.wav"));
}

void GimmickGenerator::GenerateBlade()
{
	if (generateCount % 3 == 0 && generateCount % 2 != 0)
	{
		Blade* blade = new HorizontalBlade(mGame);
		blade->SetPosition(Vector2(-1000, mGame->GetRenderTarget()->GetSize().height * 0.9f));
		blade->GetPhysicsComponent()->SetDirection(Vector2(1000.0f, 0.0f));
	}
	else if (generateCount % 7 == 0)
	{
		Blade* blade = new HorizontalBlade(mGame);
		blade->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width + 1000.0f, mGame->GetRenderTarget()->GetSize().height * 0.9f));
		blade->GetPhysicsComponent()->SetDirection(Vector2(-1000.0f, 0.0f));
	}
	else if (generateCount % 11 == 0)
	{
		Blade* blade = new VerticalBlade(mGame);
		blade->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width / 2 - 50.0f,-1000.0f));
		blade->GetPhysicsComponent()->SetDirection(Vector2(0.0f, 1000.0f));
	}

	else if (generateCount % 17 == 0)
	{
		Blade* blade = new VerticalBlade(mGame);
		blade->SetPosition(Vector2(mGame->GetRenderTarget()->GetSize().width / 2 + 50.0f, -1000.0f));
		blade->GetPhysicsComponent()->SetDirection(Vector2(0.0f, 1000.0f));
	}
	else
	{
		return;
	}
	mGame->PlayAudio(_T("Media\\–‚‰¤° Œø‰Ê‰¹ ƒƒ“ƒ|ƒCƒ“ƒg11.wav"));
}