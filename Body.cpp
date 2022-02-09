#include "Body.h"
#include "Game.h"
#include "BodyUDPComponent.h"
#include "BodyD2DDrawComponent.h"
#include "BoxComponent.h"
#include "CircleComponent.h"
#include "Block.h"

Body::Body(Game* game, int port)
	:Actor(game)
	,udpComponent(nullptr)
	,sdlComponent(nullptr)
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

	Vector2 rightPos = udpComponent->GetPointPosition(15);
	rightPos.x *= window.width;
	rightPos.y *= window.height;
	rHandCircleComponent->UpdateWorldCircleTransform(rightPos);

	Vector2 leftPos = udpComponent->GetPointPosition(16);
	leftPos.x *= window.width;
	leftPos.y *= window.height;
	lHandCircleComponent->UpdateWorldCircleTransform(leftPos);

	auto& blocks = mGame->GetBlocks();
	for (auto block : blocks)
	{
		if (Intersect(rHandCircleComponent->GetWorldCircle(), block->GetBoxComponent()->GetWorldBox())
			/*|| Intersect(lHandCircleComponent->GetWorldCircle(), block->GetBoxComponent()->GetWorldBox())*/)
		{
			printf("centerX:%f centerY:%f radius:%f\n", rHandCircleComponent->GetWorldCircle().mCenter.x, rHandCircleComponent->GetWorldCircle().mCenter.y, rHandCircleComponent->GetWorldCircle().mRadius);
			block->SetHit(true);
		}
	}
}

BodyUDPComponent* Body::GetUDPComponent()
{
	return udpComponent;
}