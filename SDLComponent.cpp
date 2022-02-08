#include "SDLComponent.h"

SDLComponent::SDLComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mDrawOrder(drawOrder)
{
	mOwner->GetGame()->AddSDL(this);
}

SDLComponent::~SDLComponent()
{
	mOwner->GetGame()->RemoveSDL(this);
}

void SDLComponent::Draw(SDL_Renderer* renderer)
{

}