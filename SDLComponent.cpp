#include "SDLComponent.h"
#include "Yarn.h"

SDLComponent::SDLComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mDrawOrder(drawOrder)
{
	mOwner->GetGame()->AddSDL(this);
}

void SDLComponent::Draw(SDL_Renderer* renderer)
{

}