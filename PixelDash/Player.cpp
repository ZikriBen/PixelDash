#include "Player.h"

Player::Player(olc::PixelGameEngine& pge, Level& lvl) : pge(pge), lvl(lvl), eLifeState(Player::ALIVE), bSoundOn(true) {
	spr = new olc::Sprite("assets/IdleRun.png");
	bubble = new TimedSprite(pge, "assets/Hello30x16.png", 0, 0, 5, 0.15f, 30, 16, 0, 0, 0, 0, 1, 0.15f * 6);
	bubble->setLoop(true);
	bubble->setAnimation(true);
	lvl.addTimedSprite(bubble);
	fWidth = 37.0f;
	fHeight = 28.0f;
	fGraphicTimer = 0.0f;
	iGraphicCounter = 0;
	
	fPlayerPosX = 5;
	fPlayerPosY = 8;

	fPlayerVelX = 0.0f;
	fPlayerVelY = 0.0f;
	eGraphicState = AnimationState::IDLE;
	bPlayerOnGround = false;
	bIsAttacking = false;
	bForceAnimation = false;
	
	eFacingDirection = Player::RIGHT;
	animations = {
		{AnimationState::IDLE, {10, 37, 28, 0.1f, 4, 41, 0, 16}},
		{AnimationState::RUN, {7, 37, 28, 0.1f, 4, 41, 58, 16}},
		{AnimationState::FALL, {1, 37, 28, 0.1f, 4, 41, 116, 16}},  
		{AnimationState::JUMP, {1, 37, 28, 0.1f, 4, 41, 174, 16}},
		{AnimationState::ATTACK, {3, 59, 58, 0.1f, -11, 12, 232, 0}},
		{AnimationState::DOOR_IN,  {8, 39, 42, 0.1f, -11, 41, 290, 0}},
		{AnimationState::DOOR_OUT,  {8, 41, 42, 0.1f, -11, 35, 348, 0}},
		{AnimationState::HIT,  {2, 37, 42, 0.1f, -11, 41, 406, 0}}
	};

	fAttackOffsetCorrection = animations[AnimationState::ATTACK].frameWidth - fWidth;
}

void Player::Update(float fElapsedTime) {
	setGraphicTimer(getGraphicTimer() + fElapsedTime);
	currentAnimation = animations[eGraphicState];

	nOffsetCorrection = 0;

	if (eFacingDirection == LEFT) {
		nOffsetCorrection = (int)(currentAnimation.frameWidth - fWidth);
	}
	
	Level& lb = Level::getInstance();

	if (PixelSprite* ps = lb.checkCollisionWithDecorations(getPlayerRect())) {
		if (Coin* c = dynamic_cast<Coin*>(ps)) {
			if (!c->isCollected()) {
				c->collect();
				lb.removeDecoration('C', c);
				earn();
			}
		}

		else if (Heart* h = dynamic_cast<Heart*>(ps)) {
			if (!h->isCollected()) {
				h->collect();
				lb.removeDecoration('H', h);
				heal();
			}
		}
	}

	if (lvl.checkCollisionWithEnemies(getPlayerRect())) {
		hit();
	}
	
	if (lvl.checkCollisionWithProjectiles(getPlayerRect())) {
		hit();
	}

	if (bForceAnimation) {
		handleForceAnimation();
	}
	else {
		handleContinousAnimation();
	}
}

void Player::Draw() {
	pge.SetPixelMode(olc::Pixel::MASK);
	float newPlayerPosX = ((fPlayerPosX - fOffsetX) * TILE_WIDTH) - nOffsetCorrection;
	float newPlayerPosY = ((fPlayerPosY - fOffsetY) * TILE_WIDTH) + currentAnimation.iOffsetPosY;

	pge.DrawPartialSprite(
		(int32_t)newPlayerPosX,
		(int32_t)newPlayerPosY,
		spr,
		(int32_t)((currentAnimation.iSprOffsetX + currentAnimation.frameWidth) * (float)(iGraphicCounter)),
		currentAnimation.iSpecialOffsetY + currentAnimation.iSprOffsetY,
		currentAnimation.frameWidth,
		currentAnimation.frameHeight,
		1,
		eFacingDirection
	);
	pge.SetPixelMode(olc::Pixel::NORMAL);
	
	if (bubble) {
		bubble->setPosX(newPlayerPosX + 35);
		bubble->setPosY(newPlayerPosY - 7);
	}
	//pge.DrawRect(getPlayerRect().x, getPlayerRect().y, currentAnimation.frameWidth, currentAnimation.frameHeight, olc::GREEN);
	//pge.DrawRect(GetAttackHitbox().x, GetAttackHitbox().y, hitBoxWidth, hitBoxHeight, olc::RED);
}

bool Player::IsDoor() {
	return Level::getInstance().isDoor(fPlayerPosX, fPlayerPosY);
}

void Player::openDoor()
{
	eGraphicState = Player::AnimationState::DOOR_IN;
	bForceAnimation = true;
	iGraphicCounter = 0;
	fGraphicTimer = 0.0f;
	Level::getInstance().openDoor();
}

Rect Player::getPlayerRect() {
	return Rect(
		(int)(((fPlayerPosX - fOffsetX) * TILE_WIDTH) - nOffsetCorrection),
		(int)(((fPlayerPosY - fOffsetY) * TILE_WIDTH) + currentAnimation.iOffsetPosY),
		currentAnimation.frameWidth,
		currentAnimation.frameHeight
	);
}

void Player::attack() {
	eGraphicState = Player::AnimationState::ATTACK;
	setPlayerIsAttacking(true);
	setForceAnimation(true);
	setGraphicCounter(0);  // Reset counter to start the attack animation from frame 0
	setGraphicTimer(0.0f); // Reset timer for consistent animation speed
}

Rect Player::GetAttackHitbox() {
	nOffsetCorrection = 0;
	float hitboxX = ((fPlayerPosX - fOffsetX) * TILE_WIDTH) + fWidth;;

	if (eFacingDirection == LEFT) {
		nOffsetCorrection = (int)(currentAnimation.frameWidth - fWidth);
		hitboxX = ((fPlayerPosX - fOffsetX) * TILE_WIDTH) - nOffsetCorrection;
	}
	
	return Rect((int)hitboxX, (int)((fPlayerPosY - fOffsetY) * TILE_WIDTH), hitBoxWidth, hitBoxHeight);
}

void Player::handleForceAnimation() {
	// Play animation until the last frame, then reset
	if (getGraphicTimer() > currentAnimation.frameDuration) {
		setGraphicTimer(getGraphicTimer() - currentAnimation.frameDuration);
		incGraphicCounter();

		if (getGraphicCounter() >= currentAnimation.iNumFrames) {
			// Attack animation is complete, reset counter and switch back to idle/run
			setGraphicCounter(0); // Reset the counter
			eGraphicState = (std::abs(getVelX()) >= 0.5f) ? AnimationState::RUN : AnimationState::IDLE;
			currentAnimation = animations[eGraphicState]; // Force update to new animation
			bForceAnimation = false;

			if (bIsAttacking) {
				if (PixelSprite* ps = Level::getInstance().checkCollisionWithDecorations(GetAttackHitbox())) {
					if (Box* b = dynamic_cast<Box*>(ps)) {
						b->hit(lvl.getTotalTime());
					}
				}
				if (PixelSprite* ps = Level::getInstance().checkCollisionWithEnemies(GetAttackHitbox())) {
					if (Enemy *e = dynamic_cast<Enemy*>(ps)) {
						e->hit(lvl.getTotalTime());
					}
					if (Cannon* c = dynamic_cast<Cannon*>(ps)) {
						c->hit(lvl.getTotalTime());
					}
				}
				bIsAttacking = false;
			}
		}
	}
}

void Player::handleContinousAnimation() {
	if (currentAnimation.iNumFrames > 1 && getGraphicTimer() > currentAnimation.frameDuration) {
		setGraphicTimer(getGraphicTimer() - currentAnimation.frameDuration);
		incGraphicCounter();
		setGraphicCounter((float)(getGraphicCounter() % currentAnimation.iNumFrames)); // Wrap within valid frame range
	}
	else if (currentAnimation.iNumFrames == 1) {
		setGraphicCounter(0);
	}
	
	eGraphicState = (std::abs(getVelX()) >= 0.5f) ? AnimationState::RUN : AnimationState::IDLE;
}

void Player::hit() {
	if (bIsAttacking) {
		return;
	}
	float currentTime = lvl.getTotalTime();
	//std::cout << fPlayerPosY << std::endl;
	if (currentTime - fLastHitTime >= fCooldownTime) {
		fLastHitTime = currentTime;  // Update the last hit time
		eGraphicState = AnimationState::HIT;
		setForceAnimation(true);
		setGraphicCounter(0);  // Reset counter to start the attack animation from frame 0
		setGraphicTimer(0.0f); // Reset timer for consistent animation speed
		HUD::getInstance().decrLife();
	}
	
}

void Player::heal() {
	HUD::getInstance().incrLife();
}

void Player::earn() {
	HUD::getInstance().incScore(1);
}