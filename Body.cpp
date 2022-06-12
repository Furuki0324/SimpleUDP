#include "Body.h"
#include "Game.h"
#include "SharedStruct.h"
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

	Circle circle(75.0f);
	rHandCircleComponent->SetCircle(circle);
	lHandCircleComponent->SetCircle(circle);
}

void Body::UpdateActor(float deltaTime) 
{
	D2D1_SIZE_F window = mGame->GetRenderTarget()->GetSize();

	//�����ł̍��E�͉�ʂɌ������Č����ꍇ
	Vector2 rightPos = udpComponent->GetPointPosition(15);		//�E���̎��
	rightPos.x *= window.width;
	rightPos.y *= window.height;
	rHandCircleComponent->UpdateWorldCircleTransform(rightPos);
	rightHandVector = rightPos - prevRightHandPos;
	prevRightHandPos = rightPos;

	Vector2 leftPos = udpComponent->GetPointPosition(16);		//�����̎��
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
	if ((prevLeftHandPos - prevRightHandPos).Length() == 0) { return; }		//���E�̎肪�����ʒu�̂܂܁i��x���g���b�L���O���Ă��Ȃ��j�Ȃ珈�����X�L�b�v

	if ((prevLeftHandPos - prevRightHandPos).Length() < 30.0f)
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
			//printf("Hit right hand.\n");
			//block->GetPhysicsComponent()->AddImpact();
		}

		if (Intersect(lHandCircleComponent->GetWorldCircle(), block->GetBoxComponent()->GetWorldBox()))
		{
			block->SetHit(true);
			//printf("Hit left hand.\n");
			//block->GetPhysicsComponent()->AddImpact();
		}
	}

	D2D1_SIZE_F window = mGame->GetRenderTarget()->GetSize();
	std::vector<std::vector<float>> positions = udpComponent->GetAllPosition();
	std::vector<MediaPipeData> mpData = udpComponent->GetMediaPipeData();
	//�̂̍ł������ʒu�E�Ⴂ�ʒu�E���̈ʒu�E�E�̈ʒu
	float top = 10000, bottom = 0, left = 10000, right = 0;
	for (std::vector<float> point : positions)
	{
		if (point[1] < left) { left = point[1]; }
		if (point[1] > right) { right = point[1]; }
		if (point[2] < top) { top = point[2]; }
		if (point[2] > bottom) { bottom = point[2]; }
	}

	for (MediaPipeData data : mpData)
	{
		if (data.x < left) { left = data.x; }
		if (data.x > right) { right = data.x; }
		if (data.y < top) { top = data.y; }
		if (data.y > bottom) { bottom = data.y; }
	}

	//���̎��_�ł�top�Ȃǂ�0~1�̒l�Ȃ̂ŉ�ʃT�C�Y���|����
	left *= window.width;
	right *= window.width;
	top *= window.height;
	bottom *= window.height;
	//printf("Left:%f Right:%f Top:%f Bottom:%f\n", left, right, top, bottom);

	auto& blades = mGame->GetBlades();
	for (auto blade : blades)
	{
		if (blade->GetHit())
		{
			//printf("\x1b[41mContinue.\x1b[m\n");
			continue;
		}

		if (blade->GetIsVertical())
		{
			//���������̃u���[�h�ɑ΂��Ă͑̂̍��E�̕�����ɂ���
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
			//���������̃u���[�h�ɑ΂��Ă͑̂̏㉺�̍�������ɂ���
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
	if ((prevLeftHandPos - prevRightHandPos).Length() < 30.0f)
	{
		mGame->SetScene(Game::Scene::MainGame);
	}
}

BodyUDPComponent* Body::GetUDPComponent()
{
	return udpComponent;
}