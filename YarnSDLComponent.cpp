#include "YarnSDLComponent.h"
#include "Yarn.h"

YarnSDLComponent::YarnSDLComponent(Yarn* owner, int drawOrder)
	:SDLComponent(owner)
	,mYarnPtr(owner)
{

}

void YarnSDLComponent::Draw(SDL_Renderer* renderer)
{
	std::vector<float> handPoint;
	handPoint = mYarnPtr->GetHandManager()->GetHandsPosition();

	DrawLine(renderer, handPoint);
}

void YarnSDLComponent::DrawLine(SDL_Renderer* renderer, std::vector<float> position)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, position[0], position[1], position[2], position[3]);
}