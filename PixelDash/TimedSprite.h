
#ifndef TIMED_SPRITE_H
#define TIMED_SPRITE_H

#include "PixelSprite.h"

#pragma once

class Level;

class TimedSprite : public PixelSprite
{
private:
    float fTimeout = 0.0f;
    float ftimePassed = 0.0f;

public:
    TimedSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY, float fTimeout);

    void setTimeOut(float timeout);
    void Update(float elapsedTime) override;
    void Draw() override;
};

#endif // TIMED_SPRITE_H
