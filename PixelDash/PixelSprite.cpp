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
	pge.DrawPartialSprite(
		fPosX,
		fPosY,
		spr,
		fOffsetX + (iGraphicCounter * fWidth),
		fOffsetY,
		fWidth,
		fHeight,
        1,
        0);
	pge.SetPixelMode(olc::Pixel::NORMAL);
}
