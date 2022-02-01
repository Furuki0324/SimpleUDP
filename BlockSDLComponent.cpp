#include "Block.h"
#include "BlockSDLComponent.h"

BlockSDLComponent::BlockSDLComponent(Block* owner, float width, float height, int drawOrder)
	:SDLComponent(owner)
	,mWidth(width)
	,mHeight(height)
{

}

void BlockSDLComponent::Draw(SDL_Renderer* renderer)
{
	Vector2 pos = mOwner->GetPosition();

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, pos.x - (mWidth / 2), pos.y - (mHeight / 2), pos.x + (mWidth / 2), pos.y - (mHeight / 2));
	SDL_RenderDrawLine(renderer, pos.x + (mWidth / 2), pos.y - (mHeight / 2), pos.x + (mWidth / 2), pos.y + (mHeight / 2));
	SDL_RenderDrawLine(renderer, pos.x + (mWidth / 2), pos.y + (mHeight / 2), pos.x - (mWidth / 2), pos.y + (mHeight / 2));
	SDL_RenderDrawLine(renderer, pos.x - (mWidth / 2), pos.y + (mHeight / 2), pos.x - (mWidth / 2), pos.y - (mHeight / 2));
}