
#ifndef FLAG_H
#define FLAG_H

#include "PixelSprite.h"

#pragma once

enum class FlagType {
    ONE,
    TWO
};

class Flag : public PixelSprite {
public:
    Flag(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
        : PixelSprite(pge, sprPath, numFrames, frameDuration, width, height, ox, oy) {}

    Flag(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY) {}

    Flag(olc::PixelGameEngine& pge)
        : PixelSprite(pge, "assets/Decorations32x32.png", 0, 0, 1, 0.1f, 32, 96, 32, 32, 0, -8) {}

    Flag(olc::PixelGameEngine& pge, FlagType type)
        : PixelSprite(pge, "assets/Decorations32x32.png", 0, 0, 1, 0.1f, 32, 96, 0, 0, 0, -8)
    {
        if (type == FlagType::ONE) {
            setOffsetPosX(32);
            setOffsetPosY(32);
        }
        else {
            setSprOffsetX(0);
            setSprOffsetY(32);
        }
    }
};

#endif // FLAG_H