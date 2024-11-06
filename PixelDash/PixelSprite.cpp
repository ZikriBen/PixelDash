#include "olcPixelGameEngine.h"
#include "PixelSprite.h"

PixelSprite::PixelSprite(olc::PixelGameEngine& pge, std::string sprPath, int numFrames, float frameDuration, int width, int height, int ox, int oy)
    :pge(pge), iNumFrames(numFrames), fFrameDuration(frameDuration), fWidth(width), fHeight(height), fOffsetX(ox), fOffsetY(oy)
{
    spr = new olc::Sprite(sprPath);
    iScale = 1;
    iFlip = 0;
    fGraphicTimer = 0.0f; 
    iGraphicCounter = 0;
    bIsAnimate = false;
    bIsLoop = false;
    bSoundOn = true;
    fCurrentHealth = 0.0f;
    fPosX = 0.0f;
    fPosY = 0.0f;
}

PixelSprite::PixelSprite(olc::PixelGameEngine& pge, std::string sprPath, float posx, float posy, int numFrames, float frameDuration, int width, int height, int ox, int oy, float offsetPosX, float offsetPosY)
    :pge(pge), fPosX(posx), fPosY(posy), iNumFrames(numFrames), fFrameDuration(frameDuration), fWidth(width), fHeight(height), fOffsetX(ox), fOffsetY(oy), fPosOffsetX(offsetPosX), fPosOffsetY(offsetPosY)
{
    spr = new olc::Sprite(sprPath);
    iScale = 1;
    iFlip = 0;
    fGraphicTimer = 0.0f;
    iGraphicCounter = 0;
    bIsAnimate = false;
    bIsLoop = false;
    bSoundOn = true;
    fCurrentHealth = 0.0f;
}

PixelSprite::~PixelSprite()
{
    delete spr;
    spr = nullptr;
}

void PixelSprite::Update(float fElapsedTime)
{
    fGraphicTimer += fElapsedTime;
 
    if (bIsAnimate) {

        if (fGraphicTimer > fFrameDuration) {
            fGraphicTimer = 0;
            iGraphicCounter++;
            iGraphicCounter %= iNumFrames + 1;
            
            if (!bIsLoop) {
                if (iGraphicCounter >= iNumFrames) {
                    fGraphicTimer = 0;
                    bIsAnimate = false;
                }
            }
        }
    }
    
}

void PixelSprite::Draw()
{
	pge.SetPixelMode(olc::Pixel::MASK);
    //pge.DrawRect(fPosX + fPosOffsetX, fPosY + fPosOffsetY, fWidth, fHeight);
    pge.DrawPartialSprite(
		fPosX + fPosOffsetX,
		fPosY + fPosOffsetY,
		spr,
		fOffsetX + (iGraphicCounter * fWidth),
		fOffsetY,
		fWidth,
		fHeight,
        iScale,
        0);
	pge.SetPixelMode(olc::Pixel::NORMAL);
}

Rect PixelSprite::getRect() {
    return Rect(getPosX(), getPosY(), getWidth(), getHeight());
}