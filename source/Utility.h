#pragma once
#include <plugin.h>
#include "game_sa\CTimer.h"

static void DrawSixRects(float x, float y, float width, float height, float distBetweenBar, CRGBA const &color) {
    float nextbar = width + distBetweenBar;
    CSprite2d::DrawRect(CRect((int)(x)-0.5f, (int)(y)-0.5f, (int)(x + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
    CSprite2d::DrawRect(CRect((int)(x + nextbar) - 0.5f, (int)(y)-0.5f, (int)(x + nextbar + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
    CSprite2d::DrawRect(CRect((int)(x + nextbar * 2) - 0.5f, (int)(y)-0.5f, (int)(x + nextbar * 2 + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
    CSprite2d::DrawRect(CRect((int)(x + nextbar * 3) - 0.5f, (int)(y)-0.5f, (int)(x + nextbar * 3 + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
    CSprite2d::DrawRect(CRect((int)(x + nextbar * 4) - 0.5f, (int)(y)-0.5f, (int)(x + nextbar * 4 + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
    CSprite2d::DrawRect(CRect((int)(x + nextbar * 5) - 0.5f, (int)(y)-0.5f, (int)(x + nextbar * 5 + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
}

static void DrawProgressBar(float x, float y, float width, float height, bool sixRects, float distBetweenBar, float progress, CRGBA const &color, CRGBA const &colorBack) {
    if (sixRects) {
        if (progress >= 100.0f)
            progress = 100.0f;
        else { // Empty
            DrawSixRects(x, y, width, height, distBetweenBar, CRGBA(colorBack.red, colorBack.green, colorBack.blue, colorBack.alpha));

        }
        if (progress > 0.0f) { // Full            
            DrawSixRects(x, y, width * progress, height, distBetweenBar, CRGBA(color.red, color.green, color.blue, color.alpha));
        }
    }
    else {
        if (progress >= 100.0f)
            progress = 100.0f;
        else // Empty
            CSprite2d::DrawRect(CRect((int)(x)-0.5f, (int)(y)-0.5f, (int)(x + width) - 0.5f, (int)(y + height) - 0.5f), CRGBA(colorBack.red, colorBack.green, colorBack.blue, colorBack.alpha));

        if (progress > 0.0f) // Full
            CSprite2d::DrawRect(CRect((int)(x)-0.5f, (int)(y)-0.5f, (int)(x + width * progress / 100.0f) - 0.5f, (int)(y + height) - 0.5f), CRGBA(color.red, color.green, color.blue, color.alpha));
    }
}

static inline bool ShowFlashingItem(signed int nOnDuration, signed int nOffDuration) {
    return CTimer::m_snTimeInMillisecondsPauseMode % (nOnDuration + nOffDuration) < nOnDuration;
}
