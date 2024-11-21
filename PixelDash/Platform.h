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
    Platform(olc::PixelGameEngine& pge, PlatformType type)
        : PixelSprite(pge, "assets/Decorations32x32.png", 0, 0, 1, 0.1, 32, 40, getOffsetX(type), getOffsetY(type), 0, 0)
    {
        setSprOffsetX(getOffsetX(type));
        setSprOffsetY(getOffsetY(type));
    }

private:
    static int getOffsetX(PlatformType type) {
        switch (type) {
        case PlatformType::ONE:   return 64;
        case PlatformType::TWO:   return 96;
        case PlatformType::THREE: return 128;
        case PlatformType::FOUR:  return 160;
        }
        return 0; // Default case
    }

    static int getOffsetY(PlatformType type) {
        return 40;  // If all platforms share the same Y-offset, otherwise add logic here
    }
};

#endif // PLATFORM_H