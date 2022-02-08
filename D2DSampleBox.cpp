#include "D2DSampleBox.h"
#include "D2DDrawComponent.h"

D2DSampleBox::D2DSampleBox(Game* game)
	:Actor(game)
{
	D2DDrawComponent* draw = new D2DDrawComponent(this);
}