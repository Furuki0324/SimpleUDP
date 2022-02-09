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

	DrawLine(renderer);
}

void BodyD2DDrawComponent::DrawLine(ID2D1HwndRenderTarget* renderer)
{
	ID2D1SolidColorBrush* pBodyBrush = nullptr;
	D2D1_SIZE_F size = renderer->GetSize();

	renderer->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&pBodyBrush
	);


	for (int i = 0; i < bodyPositions.size(); ++i)
	{
		//“·‘Ì‚Ì•`‰æ
		if (i == 11 || i == 23)
		{
			renderer->DrawLine(
				D2D1::Point2F(bodyPositions[i][1] * size.width, bodyPositions[i][2] * size.height),
				D2D1::Point2F(bodyPositions[i + 1][1] * size.width, bodyPositions[i + 1][2] * size.height),
				pBodyBrush
			);
		}
		if (i == 11 || i == 12)
		{
			renderer->DrawLine(
				D2D1::Point2F(bodyPositions[i][1] * size.width, bodyPositions[i][2] * size.height),
				D2D1::Point2F(bodyPositions[i + 12][1] * size.width, bodyPositions[i + 12][2] * size.height),
				pBodyBrush
			);
		}

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
}