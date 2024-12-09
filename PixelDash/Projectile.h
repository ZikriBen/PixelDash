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
    Projectile(olc::PixelGameEngine& pge, float posX, float posY, float velX, float velY, float homex, float homey)
        : PixelSprite(pge, "assets/Projectile.png", posX, posY, 1, 0.1f, 44, 28, 0, 0, 0, 0),
        velX(velX), velY(velY), gravity(8.0f), active(true) {
        homeX = homex;
        homeY = homey;
    }

    void Update(float fElapsedTime) {
        if (!active) return;

        // Apply velocity and gravity
        velY += gravity * fElapsedTime;

		homeX += velX * fElapsedTime;
		homeY += velY * fElapsedTime;

		setPosX(homeX);  // Apply the updated position
        setPosY(homeY);  // Apply the updated position
        PixelSprite::Update(fElapsedTime);
       

        // Check if projectile goes out of bounds
        //if (getPosY() > 1000 || getPosX() < 0 || getPosX() > 1000) { // Example bounds
        //    active = false;
        //}
    }

    void Draw(float startX, float startY, float fTileOffsetX, float fTileOffsetY) {
		float screenX = (getHomeX() - startX) * 32 - fTileOffsetX;
		float screenY = (getHomeY() - startY) * 32 - fTileOffsetY - 27;
		setPosX(screenX);
		setPosY(screenY);
        if (active) {
            PixelSprite::Draw();
        }
    }

    bool isActive() const { return active; }

    float getHomeX() { return homeX; };
    float getHomeY() { return homeY; };
};
