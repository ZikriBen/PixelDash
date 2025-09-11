#include "Enemy.h"
#include "Level.h"

Enemy::Enemy(olc::PixelGameEngine& pge, const std::string& sprPath,
    float posX, float posY, int numFrames, float frameDuration,
    int width, int height, int ox, int oy,
    float offsetPosX, float offsetPosY)
    : PixelSprite(pge, sprPath, posX, posY, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY),
    speed(2.5f), health(999), bIsAttacking(false), bIsPatrolling(true), patrolRange(5.0f), pivotX(posX), homeX(posX), homeY(posY) {

    animations = {
        {AnimationState::IDLE, {11, 34, 28, 0.1f, 2, 0, 0, 0}},
        {AnimationState::RUN, {5, 34, 28, 0.1f, 2, 0, 0, 28}},
        {AnimationState::JUMP, {1, 34, 28, 0.1f, 2, 0, 0, 56}},
        {AnimationState::FALL, {1, 34, 28, 0.1f, 2, 0, 0, 84}},
        {AnimationState::ATTACK, {4, 34, 28, 0.1f, 2, 0, 0, 112}},
        {AnimationState::HIT, {2, 34, 28, 0.1f, 2, 0, 0, 140}},
        {AnimationState::DEAD, {3, 34, 28, 0.1f, 2, 0, 0, 168}}
    };
    baseVelX = speed;
    setVelX(baseVelX);
    setGraphicState(AnimationState::RUN);
    eFacingDirection = PixelSprite::LEFT;
}

void Enemy::Update(float fElapsedTime) {

    if (eGraphicState == AnimationState::DEAD) {
        bIsPatrolling = false;
        setLoop(false);
    }

    else if (eGraphicState == AnimationState::HIT) {
        float currVelX = getVelX();
        setVelX(0.0f);
        // Wait until HIT animation finishes before returning to RUN
        if (getGraphicCounter() >= currentAnimation.iNumFrames) {
            setGraphicState(AnimationState::RUN);
            setVelX(baseVelX * (eFacingDirection ? 1 : -1));
        }
    }
    
    else if (bIsPatrolling) {
        patrol(fElapsedTime);
    }

    // Update sprite animation and state
    PixelSprite::Update(fElapsedTime);
}

void Enemy::Draw(float startX, float startY, float fTileOffsetX, float fTileOffsetY) {
	float screenX = (getHomeX() - startX) * TILE_WIDTH - fTileOffsetX;
	float screenY = (getHomeY() - startY) * TILE_HEIGHT - fTileOffsetY - 27;
	setPosX(screenX);
	setPosY(screenY);
    PixelSprite::Draw();
    //DrawRect();
}

void Enemy::hit(float currentTime) {
    // Only respond to hit if the cooldown has passed
    if (!isAlive()) {
        return;
    }
    if (currentTime - lastHitTime >= cooldownTime) {
        lastHitTime = currentTime;  // Update the last hit time
        std::cout << "Enemy hit!" << std::endl;
        takeDamage(1);
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    resetGraphicState(AnimationState::HIT);

    if (!isAlive()) {
        resetGraphicState(AnimationState::DEAD);
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::patrol(float fElapsedTime) {
    Level& lb = Level::getInstance();
    setGraphicState(AnimationState::RUN);

    // Check if we hit patrol boundaries or a wall
    bool hitRightBoundary = getHomeX() >= pivotX + patrolRange ||
        !lb.isMoveable((int)(getHomeX() + 2.0f), (int)getHomeY());

    bool hitLeftBoundary = getHomeX() <= pivotX - patrolRange ||
        !lb.isMoveable((int)(getHomeX() - 1.0f), (int)getHomeY());

    // Switch direction if necessary
    if (hitRightBoundary) {
        setVelX(-std::abs(speed)); // Move left
        eFacingDirection = PixelSprite::RIGHT;
    }
    else if (hitLeftBoundary) {
        setVelX(std::abs(speed));  // Move right
        eFacingDirection = PixelSprite::LEFT;
    }

    // Update position
    homeX += getVelX() * fElapsedTime;

    setPosX(homeX);  // Apply the updated position
}