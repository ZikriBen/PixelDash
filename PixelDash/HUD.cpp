#include "HUD.h"

HUD* HUD::instance = nullptr;

std::pair<int, int> getDigits(int n) {
	return { n / 10, n % 10 };
}

std::pair<int, int> normalize(std::pair<int, int> p) {
	return { p.first == 0 ? 9 : p.first - 1 , p.second == 0 ? 9 : p.second - 1 };
}

HUD& HUD::getInstance()
{
	if (!instance)
	{
		throw std::runtime_error("Level::Init must be called before getInstance!");
	}
	return *instance;
}

HUD::~HUD()
{
	delete liveBar;
	liveBar = nullptr;

	for (auto heart : lifeHearts) {
		delete heart;
		heart = nullptr;
	}
}

void HUD::init(olc::PixelGameEngine& pge)
{
	if (!instance) {
		instance = new HUD();
		instance->pge = &pge;
		instance->liveBar = new PixelSprite(pge, "assets/LiveBar.png", 1, 0.1f, 66, 34, 4, 0);
		instance->liveBar->setPosY(1);

		instance->smallDiamond = new PixelSprite(pge, "assets/SmallDiamond.png", 7, 0.1f, 18, 14, 0, 0);
		instance->smallDiamond->setLoop(true);
		instance->smallDiamond->setAnimation(true);
		instance->smallDiamond->setPosX(11);
		instance->smallDiamond->setPosY(25);

		instance->numberOne = new PixelSprite(pge, "assets/Numbers.png", 30, 27, 1, 0.1f, 6, 8, 54, 0, 0, 0);
		instance->numberTwo = new PixelSprite(pge, "assets/Numbers.png", 35, 27, 1, 0.1f, 6, 8, 54, 0, 0, 0);

		for (int i = 0; i < instance->m_nTotalLife; ++i) {
			int offset = 11;
			PixelSprite* heart = new PixelSprite(pge, "assets/SmallHeart.png", 7, 0.1f, 18, 14, 4, 0);
			heart->setLoop(true);
			heart->setAnimation(true);
			heart->setPosX((float)(offset * (i + 1)));
			heart->setPosY((float)offset);

			instance->lifeHearts.emplace_back(heart);
		}
	}
}

void HUD::Update(float fElapsedTime)
{
	liveBar->Update(fElapsedTime);
	smallDiamond->Update(fElapsedTime);
	
	std::pair<int, int> score = normalize(getDigits(m_nScore));

	numberOne->setSprOffsetX(numberOne->getWidth() * score.first);
	numberTwo->setSprOffsetX(numberTwo->getWidth() * score.second);

	numberOne->Update(fElapsedTime);
	numberTwo->Update(fElapsedTime);

	for (int i = 0; i < m_nLife; ++i) {
		lifeHearts[i]->Update(fElapsedTime);
	}
}

HUD::HUD()
{
}

void HUD::Draw()
{
	liveBar->Draw();
	smallDiamond->Draw();
	numberOne->Draw();
	numberTwo->Draw();
	//pge.DrawString(4, 4, "HEALTH:");

	for (int i = 0; i < m_nLife; ++i) {
		lifeHearts[i]->Draw();
	}
}
