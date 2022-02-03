#include "BodySDLComponent.h"
#include "BodyUDPComponent.h"
#include "Body.h"

BodySDLComponent::BodySDLComponent(Body* owner, Game* game, int drawOrder)
	:SDLComponent(owner, drawOrder)
	,mGame(game)
	,mBodyPtr(owner)
	,mBodyUDP(mBodyPtr->GetUDPComponent())
	,rightHandVector(0,0)
	,leftHandVector(0,0)
{
	
}

void BodySDLComponent::Draw(SDL_Renderer* renderer)
{
	bodyPositions.clear();
	bodyPositions = mBodyUDP->GetAllPosition();
	
	Vector2 newRightHand = mBodyUDP->GetPointPosition(16);
	Vector2 newLeftHand = mBodyUDP->GetPointPosition(15);
	
	rightHandVector = newRightHand - rightHandPosition;
	leftHandVector = newLeftHand - leftHandPosition;

	printf("RightHandVector:%f LeftHandVector:%f\n", rightHandVector.Length(), leftHandVector.Length());

	DrawLine(renderer, bodyPositions);

	rightHandPosition = newRightHand;
	leftHandPosition = newLeftHand;
}

void BodySDLComponent::DrawLine(SDL_Renderer* renderer, std::vector<std::vector<float>> positions)
{
	Vector2 windowSize = mGame->GetWindowSize();
	int count = 0;
	for (std::vector<float> vec : positions)
	{
		//“·‘Ì‚Ì—ÖŠs‚ð•`‰æ
		if (count == 11 || count == 23)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, vec[1] * windowSize.x, vec[2] * windowSize.y, positions[count + 1][1] * windowSize.x, positions[count + 1][2] * windowSize.y);
		}

		if (count == 11 || count == 12)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, vec[1] * windowSize.x, vec[2] * windowSize.y, positions[count + 12][1] * windowSize.x, positions[count + 12][2] * windowSize.y);
		}

		if ((count >= 11 && count <= 18) || (count >= 23 && count <= 30))
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, vec[1] * windowSize.x, vec[2] * windowSize.y, positions[count + 2][1] * windowSize.x, positions[count + 2][2] * windowSize.y);
		}

		//Žè‚ÌˆÚ“®—Ê‚Ì‘å‚«‚³‚ÆŒü‚«‚ð•`‰æ
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		if (count == 15)
		{
			SDL_RenderDrawLine(renderer, vec[1] * windowSize.x, vec[2] * windowSize.y, (vec[1] + leftHandVector.x * 20) * windowSize.x, (vec[2] + leftHandVector.y * 20) * windowSize.y);
		}
		if (count == 16)
		{
			SDL_RenderDrawLine(renderer, vec[1] * windowSize.x, vec[2] * windowSize.y, (vec[1] + rightHandVector.x * 20) * windowSize.x, (vec[2] + rightHandVector.y * 20) * windowSize.y);
		}
		count++;
	}


}