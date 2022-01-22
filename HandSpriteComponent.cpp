#include "HandSpriteComponent.h"
#include "Actor.h"

HandSpriteComponent::HandSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner,drawOrder)
{}

void HandSpriteComponent::SetHandTextures(std::vector<SDL_Texture*> textures)
{
	mHandTextures = textures;

	SetTexture(mHandTextures[0]);
}

void HandSpriteComponent::SwitchHandSprite(bool closed)
{
	if (!closed)
	{
		SetTexture(mHandTextures[0]);
	}
	else
	{
		SetTexture(mHandTextures[1]);
	}
}