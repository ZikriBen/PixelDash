#include "PixelSprite.h"
#include "Rect.h"

#pragma once
class Projectile : public PixelSprite {
private:
    float velX, velY;  // Velocity
    float gravity;     // Gravity affecting the projectile
    bool active;       // Whether the projectile is still active
    float homeX;
    float homeY;

public:
    Projectile(olc::PixelGameEngine& pge, float posX, float posY, float velX, float velY)
        : PixelSprite(pge, "assets/Projectile.png", posX, posY, 1, 0.1f, 44, 28, 0, 0, 0, 0),
        velX(velX), velY(velY), gravity(300.0f), active(true) {}

    void Update(float fElapsedTime) {
        if (!active) return;

        // Apply velocity and gravity
        
        velY += gravity * fElapsedTime;
        setPosX(getPosX() + velX * fElapsedTime);
        setPosY(getPosY() + velY * fElapsedTime);

        // Check if projectile goes out of bounds
        //if (getPosY() > 1000 || getPosX() < 0 || getPosX() > 1000) { // Example bounds
        //    active = false;
        //}

        // Check collision with player
        /*Rect projRect(posX, posY, frameWidth, frameHeight);
        if (Level::getInstance().checkCollisionWithPlayer(projRect)) {
            Level::getInstance().damagePlayer();
            active = false;
        }*/
    }

    void Draw() {
        if (active) {
            PixelSprite::Draw();
        }
    }

    bool isActive() const { return active; }
};
