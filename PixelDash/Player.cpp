#include "Player.h"

Player::Player(olc::PixelGameEngine& pge) : pge(pge), eLifeState(Player::ALIVE), bSoundOn(true) {
	spr = new olc::Sprite("assets/IdleRun.png");
	fWidth = 37.0f;
	fHeight = 28.0f;
	fGraphicTimer = 0.0f;
	iGraphicCounter = 0;
	
	fPlayerPosX = 5;
	fPlayerPosY = 8;

	fPlayerVelX = 0.0f;
	fPlayerVelY = 0.0f;
	eGraphicState = Player::IDLE;
	bPlayerOnGround = false;
	eFacingDirection = Player::RIGHT;
	eGraphicState = Player::IDLE;
}

void Player::Update(float fElapsedTime) {

	fGraphicTimer += fElapsedTime;
	if (fGraphicTimer > 0.2f) {
		fGraphicTimer -= 0.2f;
		iGraphicCounter++;
		iGraphicCounter %= 2;
	}

}

void Player::Draw(float fOffsetX, float fOffsetY, float oy) {
	
	pge.SetPixelMode(olc::Pixel::MASK);
	pge.DrawPartialSprite(
		(fPlayerPosX - fOffsetX) * 32,
		((fPlayerPosY - fOffsetY) * 32) + 4,
		spr,
		(41.0f + 37.0f) * (float)(iGraphicCounter),
		16 + oy,
		37,
		28,
		1,
		eFacingDirection
	);
	pge.SetPixelMode(olc::Pixel::NORMAL);
}
