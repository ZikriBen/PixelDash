#include "PixelSprite.h"
#include "Rect.h"
#include "GameConfig.h"

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
        velX(velX), velY(velY), gravity(6.0f), active(true) {
        homeX = homex;
        homeY = homey;
    }

    void Update(float fElapsedTime) {
        if (!active) return;

        // Apply velocity and gravity
        velY += gravity * fElapsedTime;

		homeX += velX * fElapsedTime;
		homeY += velY * fElapsedTime;
        
        // Apply the updated position
		setPosX(homeX);  
        setPosY(homeY);  // Apply the updated position
        PixelSprite::Update(fElapsedTime);

        // Check if projectile goes out of bounds
        if (homeY > LEVEL_HEIGHT || homeY < 0 || homeX > LEVEL_WIDTH || homeX < 0) {
            active = false;
        }
    }

    void Draw(float startX, float startY, float fTileOffsetX, float fTileOffsetY) {
		float screenX = (getHomeX() - startX) * TILE_WIDTH - fTileOffsetX;
		float screenY = (getHomeY() - startY) * TILE_HEIGHT - fTileOffsetY - 27;
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
