#pragma once
#include "SpriteComponent.h"
#include <vector>

class HandSpriteComponent : public SpriteComponent
{
public:
	HandSpriteComponent(class Actor* owner, int drawOrder = 100);

	void SwitchHandSprite(bool closed);
	void SetHandTextures(std::vector<SDL_Texture*> textures);

private:
	std::vector<SDL_Texture*> mHandTextures;
};