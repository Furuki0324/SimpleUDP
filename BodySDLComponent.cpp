#include "BodySDLComponent.h"
#include "BodyUDPComponent.h"
#include "Body.h"

BodySDLComponent::BodySDLComponent(Body* owner, Game* game, int drawOrder)
	:SDLComponent(owner, drawOrder)
	,mBodyPtr(owner)
	,mGame(game)
{

}

void BodySDLComponent::Draw(SDL_Renderer* renderer)
{
	bodyPositions.clear();
	bodyPositions = mBodyPtr->GetUDPComponent()->GetAllPosition();
	//printf("Updating\n");
	DrawLine(renderer, bodyPositions);
}

void BodySDLComponent::DrawLine(SDL_Renderer* renderer, std::vector<std::vector<float>> positions)
{
	Vector2 windowSize = mGame->GetWindowSize();
	int count = 0;
	for (std::vector<float> vec : positions)
	{
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
		count++;
	}


}