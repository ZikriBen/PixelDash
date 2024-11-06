#include "HUD.h"


std::pair<int, int> getDigits(int n) {
	return { n / 10, n % 10 };
}

std::pair<int, int> normalize(std::pair<int, int> p) {
	return { p.first == 0 ? 9 : p.first - 1 , p.second == 0 ? 9 : p.second - 1 };
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

void HUD::init()
{
	liveBar = new PixelSprite(pge, "assets/LiveBar.png", 1, 0.1, 66, 34, 4, 0);
	liveBar->setPosY(1);
	
	smallDiamond = new PixelSprite(pge, "assets/SmallDiamond.png", 7, 0.1, 18, 14, 0, 0);
	smallDiamond->setLoop(true);
	smallDiamond->setAnimation(true);
	smallDiamond->setPosX(11);
	smallDiamond->setPosY(25);

	numberOne = new PixelSprite(pge, "assets/Numbers.png", 30, 27, 1, 0.1, 6, 8, 54, 0, 0, 0);
	numberTwo = new PixelSprite(pge, "assets/Numbers.png", 35, 27, 1, 0.1, 6, 8, 54, 0, 0, 0);

	for (int i = 0; i < m_nTotalLife; ++i) {
		int offset = 11;
		PixelSprite* heart = new PixelSprite(pge, "assets/SmallHeart.png", 7, 0.1, 18, 14, 4, 0);
		heart->setLoop(true);
		heart->setAnimation(true);
		heart->setPosX(offset * (i + 1));
		heart->setPosY(offset);

		lifeHearts.emplace_back(heart);
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
