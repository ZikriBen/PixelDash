#ifndef ENEMY_H
#define ENEMY_H

#include "PixelSprite.h"
#include "Rect.h"

class Enemy : public PixelSprite {
public:
    // Constructor
    Enemy(olc::PixelGameEngine& pge, const std::string& sprPath,
        float posX, float posY, int numFrames, float frameDuration,
        int width, int height, int ox, int oy,
        float offsetPosX, float offsetPosY);

    // Behavior methods
    void Update(float fElapsedTime) override;
    void Draw(float startX, float startY, float fTileOffsetX, float fTileOffsetY);
    void takeDamage(int damage);
    bool isAlive() const;
    float getHomeX() { return homeX; };
    float getHomeY() { return homeY; };
    void hit(float currentTime);
    void patrol(float fElapsedTime);
    void setVelX(float newVel) { fEnemyVelX = newVel; };
    void setVelY(float newVel) { fEnemyVelY = newVel; };
    float getVelX() { return fEnemyVelX; };
    float getVelY() { return fEnemyVelY; };
    bool getEnemyOnGround() { return bEnemyOnGround; }
    void setEnemyOnGround(bool newBool) { bEnemyOnGround = newBool; }

private:
    float speed;          // Enemy movement speed
    int health;           // Health of the enemy
    bool bIsAttacking;    // If the enemy is attacking
    bool bIsPatrolling;   // If the enemy is in patrol mode
    float patrolRange;    // Patrol range from the starting position
    float homeX;          
    float homeY;
    float pivotX;
    float lastHitTime = -1.0;      // Tracks the last time this box was hit
    const float cooldownTime = 0.05f;  // Cooldown duration in seconds, adjust as needed
    float fEnemyVelX;
    float fEnemyVelY;
    bool bEnemyOnGround;
    float baseVelX;
};

#endif // ENEMY_H
