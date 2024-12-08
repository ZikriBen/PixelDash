#ifndef COIN_H
#define COIN_H

#include "PixelSprite.h"


class Coin : public PixelSprite {
public:
    // Constructor for Box class, inheriting from PixelSprite constructor
    Coin(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
        : PixelSprite(pge, sprPath, numFrames, frameDuration, width, height, ox, oy) {}

    // Overloaded constructor for position and offset
    Coin(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY) {
        int tileSize = 32;

        setOffsetPosX((tileSize / 2.0f) - (width / 2.0f));
        setOffsetPosY((tileSize) - (height / 2.0f));
    }

    void collect() { collected = true; }

    bool isCollected() const { return collected; }

    void Draw() override {
        if (!collected) {
            PixelSprite::Draw();
        }
    }

    void Update(float elapsedTime) override {
        // If collected, don't update any further logic for the coin
        if (!collected) {
            PixelSprite::Update(elapsedTime);
        }
    }

private:
    bool collected = false;
};

#endif // COIN_H