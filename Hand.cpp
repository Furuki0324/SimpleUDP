#include "Hand.h"
#include "Game.h"

Hand::Hand(Game* game, int inPort, bool right)
	:Actor(game)
	, hsc(nullptr)
	, udpComponent(nullptr)
{
	hsc = new HandSpriteComponent(this);
	udpComponent = new HandUDPComponent(this, inPort);
	std::vector<SDL_Texture*> texs;
	if (right)
	{
		texs = {
		game->GetTexture("Asset/OpenHand_R.png"),
		game->GetTexture("Asset/CloseHand_R.png")
		};
	}
	else
	{
		texs = {
			game->GetTexture("Asset/OpenHand.png"),
			game->GetTexture("Asset/CloseHand.png")
		};
	}

	hsc->SetHandTextures(texs);
}

Hand::~Hand()
{}

UDPSocket* Hand::GetUDPSocket()
{
	return udpComponent;
}

void Hand::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	//UDPSocketから手首の座標を取得。もし手を一度も検出していなかったら（起動時など）座標0を取得します。
	Vector2 wristPos = udpComponent->GetPointPosition(0);
	SetPosition(wristPos);

	//親指と人差し指の距離を計測。一定値以下なら物をつまむ時の形をしていると判断します。
	Vector2 thumbPos = udpComponent->GetPointPosition(4);
	Vector2 indexPos = udpComponent->GetPointPosition(8);
	float TILength = (thumbPos - indexPos).Length();

	isClosed = TILength < 20.0f;
	hsc->SwitchHandSprite(isClosed);
}