
#ifndef WINDOW_H
#define WINDOW_H

#include "PixelSprite.h"

#pragma once

enum class WindowType {
    ONE,
    TWO
};

class Window : public PixelSprite {
public:
    Window(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
        : PixelSprite(pge, sprPath, numFrames, frameDuration, width, height, ox, oy) {}

    Window(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
        : PixelSprite(pge, sprPath, posx, posy, numFrames, frameDuration, width, height, ox, oy, offsetPosX, offsetPosY) {}

    Window(olc::PixelGameEngine& pge)
        : PixelSprite(pge, "assets/Decorations32x32.png", 0, 0, 1, 0.1f, 64, 64, 64, 96, 0, 0) {}

    Window(olc::PixelGameEngine& pge, WindowType type)
        : PixelSprite(pge, "assets/Decorations32x32.png", 0, 0, 1, 0.1f, 64, 64, (type == WindowType::ONE ? 64 : 128), 96, 0, 0)
    {
        if (type == WindowType::ONE) {
            setSprOffsetX(64);
            setSprOffsetY(96);
        }
        else {
            setSprOffsetX(128);
            setSprOffsetY(96);
        }
    }
};

#endif // WINDOW_H