#ifndef PLATFORM_H
#define PLATFORM_H

#include "PixelSprite.h"

#pragma once

enum class PlatformType {
    ONE,
    TWO,
    THREE,
    FOUR
};

class Platform : public PixelSprite {
public:
    Platform(olc::PixelGameEngine& pge, PlatformType type, float fposx = 0, float fposy = 0)
        : PixelSprite(pge, "assets/Decorations32x32.png", 0, 0, 1, 0.1f, 32, 40, (int)getOffsetX(type), (int)getOffsetY(type), 0, 0)
    {
        setSprOffsetX(getOffsetX(type));
        setSprOffsetY(getOffsetY(type));
        homeX = fposx;
        homeY = fposy;
    }

    float getHomeX() { return homeX; };
    float getHomeY() { return homeY; };

private:
    float homeX;
    float homeY;
    static float getOffsetX(PlatformType type) {
        switch (type) {
        case PlatformType::ONE:   return 64.0f;
        case PlatformType::TWO:   return 96.0f;
        case PlatformType::THREE: return 128.0f;
        case PlatformType::FOUR:  return 160.0f;
        }
        return 0.0f; // Default case
    }

    static float getOffsetY(PlatformType type) {
        return 40.0f;  // If all platforms share the same Y-offset, otherwise add logic here
    }
};

#endif // PLATFORM_H