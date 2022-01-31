#include "BodySDLComponent.h"
#include "BodyUDPComponent.h"
#include "Body.h"

BodySDLComponent::BodySDLComponent(Body* owner, int drawOrder)
	:SDLComponent(owner, drawOrder)
	,mBodyPtr(owner)
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
	int width = 960;
	int height = 720;
	int count = 0;
	for (std::vector<float> vec : positions)
	{
		if (count == 11 || count == 23)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, vec[1] * width, vec[2] * height, positions[count + 1][1] * width, positions[count + 1][2] * height);
		}

		if (count == 11 || count == 12)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, vec[1] * width, vec[2] * height, positions[count + 12][1] * width, positions[count + 12][2] * height);
		}

		if ((count >= 11 && count <= 18) || (count >= 23 && count <= 30))
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, vec[1] * width, vec[2] * height, positions[count + 2][1] * width, positions[count + 2][2] * height);
		}
		count++;
	}


}