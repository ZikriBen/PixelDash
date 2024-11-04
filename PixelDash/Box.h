#ifndef BOX_H
#define BOX_H

#include "PixelSprite.h"
#include "Level.h"

class Rect;

class Box : public PixelSprite {
public:
    // Constructor for Box class, inheriting from PixelSprite constructor
    Box(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
        : PixelSprite(pge, sprPath, numFrames, frameDuration, width, height, ox, oy) {}

    // Overloaded constructor for position and offset
    Box(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY) {}

    Rect getHitbox();
    void hit(float);
private:
    float lastHitTime = -1.0;      // Tracks the last time this box was hit
    const float cooldownTime = 0.5f;  // Cooldown duration in seconds, adjust as needed
};

#endif // BOX_H