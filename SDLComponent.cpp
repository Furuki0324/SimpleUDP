#include "SDLComponent.h"

SDLComponent::SDLComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mDrawOrder(drawOrder)
{
	mOwner->GetGame()->AddSDL(this);
}

void SDLComponent::Draw(SDL_Renderer* renderer)
{

}

void SDLComponent::DrawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2)
{

}