#include "Enemy.h"
#include "Level.h"

Enemy::Enemy(olc::PixelGameEngine& pge, const std::string& sprPath,
    float posX, float posY, int numFrames, float frameDuration,
    int width, int height, int ox, int oy,
    float offsetPosX, float offsetPosY)
    : PixelSprite(pge, sprPath, posX, posY, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY),
    speed(2.5f), health(3), bIsAttacking(false), bIsPatrolling(true), patrolRange(5.0f), pivotX(posX), homeX(posX), homeY(posY) {

    animations = {
        {AnimationState::IDLE, {11, 34, 28, 0.1, 2, 0, 0, 0}},
        {AnimationState::RUN, {5, 34, 28, 0.1, 2, 0, 0, 28}},
        {AnimationState::JUMP, {1, 34, 28, 0.1, 2, 0, 0, 56}},
        {AnimationState::FALL, {1, 34, 28, 0.1, 2, 0, 0, 84}},
        {AnimationState::ATTACK, {4, 34, 28, 0.1, 2, 0, 0, 112}},
        {AnimationState::HIT, {1, 34, 28, 0.1, 2, 0, 0, 140}},
        {AnimationState::DEAD, {3, 34, 28, 0.1, 2, 0, 0, 168}}
    };

    eGraphicState = AnimationState::DEAD;
    currentAnimation = animations[eGraphicState];
}

void Enemy::Update(float fElapsedTime) {
    // Patrol behavior: move back and forth within patrol range
    
    if (bIsPatrolling) {
        if (getHomeX() > pivotX + patrolRange) {
            speed = -std::abs(speed);
            eFacingDirection = PixelSprite::RIGHT;
        }
        else if (getHomeX() < pivotX - patrolRange) {
            speed = std::abs(speed);
            eFacingDirection = PixelSprite::LEFT;
        }

        homeX = getHomeX() + speed * fElapsedTime;
    }

    // Update sprite animation and state
    PixelSprite::Update(fElapsedTime);
}

void Enemy::Draw() {
    PixelSprite::Draw();
    pge.DrawRect(getRect().x, getRect().y, getRect().width, getRect().height, olc::RED);
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        bIsPatrolling = false; // Stop patrolling when dead
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}
