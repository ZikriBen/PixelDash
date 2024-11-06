#include "HUD.h"

HUD::~HUD()
{
	delete liveBar;
	liveBar = nullptr;

	for (auto heart : lifeHearts) {
		delete heart;
		heart = nullptr;
	}
}

void HUD::init()
{
	liveBar = new PixelSprite(pge, "assets/LiveBar.png", 1, 0.1, 66, 34, 4, 0);
	liveBar->setPosY(1);

	for (int i = 0; i < 3; ++i) {
		int offset = 11;
		PixelSprite* heart = new PixelSprite(pge, "assets/SmallHeart.png", 7, 0.1, 18, 14, 4, 0);
		heart->setLoop(true);
		heart->setAnimation(true);
		heart->setPosX(offset * (i + 1));
		heart->setPosY(offset);

		lifeHearts.emplace_back(heart);
	}
}

void HUD::Draw()
{
	liveBar->Draw();


	for (auto heart : lifeHearts) {
		heart->Draw();
	}
}

void HUD::Update(float fElapsedTime)
{
	liveBar->Update(fElapsedTime);

	for (auto heart : lifeHearts) {
		heart->Update(fElapsedTime);
	}
}
