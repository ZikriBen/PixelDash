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
    void Draw() override;
    void takeDamage(int damage);
    bool isAlive() const;
    float getHomeX() { return homeX; };
    float getHomeY() { return homeY; };

private:
    float speed;          // Enemy movement speed
    int health;           // Health of the enemy
    bool bIsAttacking;    // If the enemy is attacking
    bool bIsPatrolling;   // If the enemy is in patrol mode
    float patrolRange;    // Patrol range from the starting position
    float homeX;          
    float homeY;
    float pivotX;
};

#endif // ENEMY_H
