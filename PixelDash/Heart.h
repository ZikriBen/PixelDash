#ifndef HEART_H
#define HEART_H

#include "PixelSprite.h"


class Heart : public PixelSprite {
public:
    // Constructor for Box class, inheriting from PixelSprite constructor
    Heart(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
        : PixelSprite(pge, sprPath, numFrames, frameDuration, width, height, ox, oy) {}

    // Overloaded constructor for position and offset
    Heart(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY) {}

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

#endif // HEART_H