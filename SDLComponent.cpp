#include "SDLComponent.h"
#include "Yarn.h"

SDLComponent::SDLComponent(Actor* owner, Yarn* yarn, int drawOrder)
	:Component(owner)
	,mDrawOrder(drawOrder)
	,mYarnPtr(yarn)
{
	mOwner->GetGame()->AddSDL(this);
}

void SDLComponent::Draw(SDL_Renderer* renderer)
{
	std::vector<float> handPoint;
	handPoint = mYarnPtr->GetHandManager()->GetHandsPosition();

	DrawLine(renderer, handPoint);
}

void SDLComponent::DrawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2)
{

}

/**
* @param handPoint contains [x1, y1, x2, y2].
*/
void SDLComponent::DrawLine(SDL_Renderer* renderer, std::vector<float> handPoint)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, handPoint[0], handPoint[1], handPoint[2], handPoint[3]);
}