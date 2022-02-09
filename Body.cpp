#include "Body.h"
#include "Game.h"
#include "BodyUDPComponent.h"
#include "BodyD2DDrawComponent.h"
#include "BoxComponent.h"
#include "CircleComponent.h"
#include "Block.h"
#include "Blade.h"
#include "PhysicsComponent.h"

Body::Body(Game* game, int port)
	:Actor(game)
	,udpComponent(nullptr)
	,prevRightHandPos(Vector2::Zero)
	,prevLeftHandPos(Vector2::Zero)
	,rightHandVector(Vector2::Zero)
	,leftHandVector(Vector2::Zero)
{
	udpComponent = new BodyUDPComponent(this, port, 1);
	d2dComponent = new BodyD2DDrawComponent(this);

	rHandCircleComponent = new CircleComponent(this);
	lHandCircleComponent = new CircleComponent(this);

	Circle circle(50.0f);
	rHandCircleComponent->SetCircle(circle);
	lHandCircleComponent->SetCircle(circle);
}

void Body::UpdateActor(float deltaTime) 
{
	D2D1_SIZE_F window = mGame->GetRenderTarget()->GetSize();

	//ここでの左右は画面に向かって見た場合
	Vector2 rightPos = udpComponent->GetPointPosition(19);		//右側の手の人差し指
	rightPos.x *= window.width;
	rightPos.y *= window.height;
	rHandCircleComponent->UpdateWorldCircleTransform(rightPos);
	rightHandVector = rightPos - prevRightHandPos;
	prevRightHandPos = rightPos;

	Vector2 leftPos = udpComponent->GetPointPosition(20);		//左側の手の人差し指
	leftPos.x *= window.width;
	leftPos.y *= window.height;
	lHandCircleComponent->UpdateWorldCircleTransform(leftPos);
	leftHandVector = leftPos - prevLeftHandPos;
	prevLeftHandPos = leftPos;

	switch (mGame->GetScene())
	{
	case Game::Scene::Title:
		UpdateOnTitle(deltaTime);
		break;

	case Game::Scene::MainGame:
		UpdateOnMainGame(deltaTime);
		break;

	case Game::Scene::Result:
		UpdateOnResult(deltaTime);
		break;
	}

}

void Body::UpdateOnTitle(float deltaTime)
{
	if ((prevLeftHandPos - prevRightHandPos).Length() == 0) { return; }		//左右の手が初期位置のまま（一度もトラッキングしていない）なら処理をスキップ

	if ((prevLeftHandPos - prevRightHandPos).Length() < 100.0f)
	{
		mGame->SetScene(Game::Scene::MainGame);
	}
}

void Body::UpdateOnMainGame(float deltaTime)
{
	auto& blocks = mGame->GetBlocks();
	for (auto block : blocks)
	{
		if (block->GetHit()) { continue; }

		if (Intersect(rHandCircleComponent->GetWorldCircle(), block->GetBoxComponent()->GetWorldBox()))
		{
			block->SetHit(true);
			//block->GetPhysicsComponent()->AddImpact();
		}

		if (Intersect(lHandCircleComponent->GetWorldCircle(), block->GetBoxComponent()->GetWorldBox()))
		{
			block->SetHit(true);
			//block->GetPhysicsComponent()->AddImpact();
		}
	}

	std::vector<std::vector<float>> positions = udpComponent->GetAllPosition();
	//体の最も高い位置・低い位置・左の位置・右の位置
	float top = 10000, bottom = 0, left = 10000, right = 0;
	for (std::vector<float> point : positions)
	{
		if (point[1] < left) { left = point[1]; }
		if (point[1] > right) { right = point[1]; }
		if (point[2] < top) { top = point[2]; }
		if (point[2] > bottom) { bottom = point[2]; }
	}
	auto& blades = mGame->GetBlades();
	for (auto blade : blades)
	{
		if (blade->GetHit()) { continue; }

		if (blade->GetIsVertical())
		{
			//垂直方向のブレードに対しては体の左右の幅を基準にする
			if (blade->GetPosition().y > top && blade->GetPosition().y < bottom)
			{
				LineSegment line(Vector2(left, blade->GetPosition().y * 0.9f), Vector2(right, blade->GetPosition().y * 0.9f));
				if (Intersect(line, blade->GetBoxComponent()->GetWorldBox()))
				{
					blade->SetHit(true);
					continue;
				}
			}
		}
		else
		{
			//水平方向のブレードに対しては体の上下の高さを基準にする
			if (blade->GetPosition().x > left && blade->GetPosition().x < right)
			{
				LineSegment line(Vector2(blade->GetPosition().x * 0.9f, top), Vector2(blade->GetPosition().x * 0.9f, bottom));
				if (Intersect(line, blade->GetBoxComponent()->GetWorldBox()))
				{
					blade->SetHit(true);
					continue;
				}
			}
		}
	}
}

void Body::UpdateOnResult(float deltaTime)
{
	if ((prevLeftHandPos - prevRightHandPos).Length() < 100.0f)
	{
		mGame->SetScene(Game::Scene::MainGame);
	}
}

BodyUDPComponent* Body::GetUDPComponent()
{
	return udpComponent;
}