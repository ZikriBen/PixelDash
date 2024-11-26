
#ifndef TIMED_SPRITE_H
#define TIMED_SPRITE_H

#include "PixelSprite.h"

#pragma once

class Level;

class TimedSprite : public PixelSprite
{
private:
    float fStartTime = 0.0f;
    float fEndTime = 0.0f;
    float fTimeOut = 0.0f;
    float fTimePassed = 0.0f;
    bool bActive = true;

public:
    TimedSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY, float fStartTime, float fEndTime);

    float getStartTime() { return fStartTime; };
    float getEndTime() { return fEndTime; };
    float getTimeOut() { return fTimeOut; };
    bool isActive() const { return bActive; }
    
    bool operator<(const TimedSprite& other) const {
        return fStartTime > other.fStartTime; // Min-heap (earliest time first)
    }

    bool isExpired(float currentTime) const {
        return currentTime >= fEndTime + fStartTime;
    }

    void Update(float fElapsedTime) override;
};

#endif // TIMED_SPRITE_H
