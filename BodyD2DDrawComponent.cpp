#include "BodyD2DDrawComponent.h"
#include "Body.h"
#include "BodyUDPComponent.h"
#include "Game.h"

BodyD2DDrawComponent::BodyD2DDrawComponent(Body* owner, int drawOrder)
	:D2DDrawComponent(owner,drawOrder)
	,mBodyUDP(owner->GetUDPComponent())
{}

void BodyD2DDrawComponent::Draw(ID2D1HwndRenderTarget* renderer)
{
	bodyPositions.clear();
	bodyPositions = mBodyUDP->GetAllPosition();

	Vector2 newRightHand = mBodyUDP->GetPointPosition(16);
	Vector2 newLeftHand = mBodyUDP->GetPointPosition(15);

	rightHandVector = newRightHand - rightHandPosition;
	leftHandVector = newLeftHand - leftHandPosition;

	DrawLine(renderer);

	rightHandPosition = newRightHand;
	leftHandPosition = newLeftHand;
}

void BodyD2DDrawComponent::DrawLine(ID2D1HwndRenderTarget* renderer)
{
	ID2D1SolidColorBrush* pBodyBrush = nullptr;
	ID2D1SolidColorBrush* pHandVectorBrush = nullptr;
	D2D1_SIZE_F size = renderer->GetSize();

	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&pBodyBrush
	);

	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&pHandVectorBrush
	);

	for (int i = 0; i < bodyPositions.size(); ++i)
	{
		//ŽlŽˆ‚Ì•`‰æ
		if ((i >= 11 && i <= 18) || (i >= 23 && i <= 30))
		{
			renderer->DrawLine(
				D2D1::Point2F(bodyPositions[i][1] * size.width, bodyPositions[i][2] * size.height),
				D2D1::Point2F(bodyPositions[i + 2][1] * size.width, bodyPositions[i + 2][2] * size.height),
				pBodyBrush
			);
		}
	}
	//Žè‚ÌˆÚ“®—Ê‚ð‰ÂŽ‹‰»
	renderer->DrawLine(
		D2D1::Point2F(bodyPositions[15][1] * size.width, bodyPositions[15][2] * size.height),
		D2D1::Point2F((bodyPositions[15][1] + leftHandVector.x) * size.width, (bodyPositions[15][2] + leftHandVector.y) * size.height),
		pHandVectorBrush,
		3.0f
	);

	renderer->DrawLine(
		D2D1::Point2F(bodyPositions[16][1] * size.width, bodyPositions[16][2] * size.height),
		D2D1::Point2F((bodyPositions[16][1] + rightHandVector.x) * size.width, (bodyPositions[16][2] + rightHandVector.y) * size.height),
		pHandVectorBrush,
		3.0f
	);

	renderer->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(bodyPositions[15][1] * size.width, bodyPositions[15][2] * size.height),
			leftHandVector.x,
			leftHandVector.y
		),
		pHandVectorBrush,
		3.0f
	);

	renderer->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(bodyPositions[16][1] * size.width, bodyPositions[16][2] * size.height),
			rightHandVector.x,
			rightHandVector.y
		),
		pHandVectorBrush,
		3.0f
	);

	//printf("LeftHand:%f,%f\n", leftHandVector.x, leftHandVector.y);
	//printf("Size:%d Draw finish. (BodyD2DDrawComponent-40)\n", bodyPositions.size());
}