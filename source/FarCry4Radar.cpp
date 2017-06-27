#include "plugin.h"
#include "game_sa\common.h"
#include "game_sa\CMenuManager.h"
#include "game_sa\CRadar.h"
#include "game_sa\CTimer.h"
#include "game_sa\CFont.h"
#include "FarCry4Radar.h"
#include "FarCry4Texture.h"
#include "Utility.h"

float fPosX = 260.0f;
float fPosY = 216.0f;
float fWidth = 115.0f;
float fHeight = 91.0f;

void FarCry4Radar::InjectPatches() {
    // Set Radar Range
    static const float fRadarRange = 100.0f;
    plugin::patch::SetFloat(0x586C9B, 100.0f);
    plugin::patch::Set(0x586C66, &fRadarRange);

    // Transparent Radar
    static int iRadarAlpha = 30;
    plugin::patch::Set<DWORD>(0x5864BD, iRadarAlpha);
    plugin::patch::RedirectJump(0x58641A, RadarAlpha);

    // Rectangular Radar
    static float rect_offset = 0.0f;
    plugin::patch::SetFloat(0x585719, rect_offset);
    plugin::patch::SetFloat(0x585721, rect_offset);
    plugin::patch::SetFloat(0x585729, rect_offset);
    plugin::patch::SetFloat(0x585731, rect_offset);
    plugin::patch::SetFloat(0x585739, rect_offset);
    plugin::patch::SetFloat(0x585741, rect_offset);
    plugin::patch::SetFloat(0x585749, rect_offset);
    plugin::patch::SetFloat(0x585751, rect_offset);
    plugin::patch::RedirectJump(0x5832F0, LimitRadarPoint);

    plugin::patch::RedirectJump(0x583480, TransformRadarPointToScreenSpace);

    // Disable Radar Circle
    plugin::patch::Nop(0x58A818, 16);
    plugin::patch::Nop(0x58A8C2, 16);
    plugin::patch::Nop(0x58A96C, 16);
    plugin::patch::Nop(0x58AA1A, 16);

    plugin::patch::RedirectCall(0x588722, DrawRadarCentre);

    // Blips
    plugin::patch::Set(0x5BA85F + 1, PLUGIN_PATH("FarCry4Hud\\blips.txd"));

    // Draw Radar
    plugin::patch::RedirectCall(0x58FC53, DrawStandardRadar);
}

void __declspec(naked) FarCry4Radar::RadarAlpha() {
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
    _asm {
        mov     ecx, 586420h
        mov     al, [esp + 140h - 12Dh]
        test    al, al
        jmp     ecx
    }
}

long double __cdecl FarCry4Radar::LimitRadarPoint(CVector2D *pos) {
    long double	result = sqrt(pos->x * pos->x + pos->y * pos->y);

    if (result > 1.41421356237309504880)
    {
        pos->x /= result * 0.707106781186547524401;
        pos->y /= result * 0.707106781186547524401;
    }
    if (pos->x > 1.0)
        pos->x = 1.0;
    else if (-1.0 > pos->x)
        pos->x = -1.0;
    if (pos->y > 1.0)
        pos->y = 1.0;
    else if (-1.0 > pos->y)
        pos->y = -1.0;

    return result;
}

void FarCry4Radar::TransformRadarPointToScreenSpace(CVector2D *out, CVector2D *in) {
    __asm push edx

    if (FrontEndMenuManager.drawRadarOrMap) {
        out->x = FrontEndMenuManager.m_fMapZoom * in->x + FrontEndMenuManager.m_fMapBaseX;
        out->y = FrontEndMenuManager.m_fMapBaseY - FrontEndMenuManager.m_fMapZoom * in->y;
    }
    else {
        out->x = SCREEN_COORD(fPosX) + in->x * SCREEN_COORD(fWidth);
        out->y = SCREEN_COORD_MAX_Y - SCREEN_COORD(fPosY) - in->y * SCREEN_COORD(fHeight);
    }

    __asm pop edx
}

void FarCry4Radar::DrawRadarSprites() {
    int savedAlpha;
    RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &savedAlpha);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));

    // Radar Rectangle
    CSprite2d::DrawRect(CRect(SCREEN_COORD(fPosX - 115.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(fPosY - 91.0f), SCREEN_COORD(fPosX - 115.0f) + SCREEN_MULTIPLIER(fWidth * 2), SCREEN_COORD_MAX_Y - SCREEN_COORD(fPosY - 91.0f) - SCREEN_MULTIPLIER(fHeight * 2)), CRGBA(0, 0, 0, 50));
    CSprite2d::DrawRect(CRect(SCREEN_COORD(fPosX - 119.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(fPosY - 96.0f), SCREEN_COORD(fPosX - 119.0f) + SCREEN_MULTIPLIER(fWidth * 2 + 8.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(fPosY - 96.0f) - SCREEN_MULTIPLIER(fHeight * 2 + 10.0f)), CRGBA(0, 0, 0, 50));

    // Left side
    // Hud Rectangle
    // Up left
    CSprite2d::DrawRect(CRect(SCREEN_COORD(fPosX - 115.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(358.0f), SCREEN_COORD(fPosX - 115.0f) + SCREEN_MULTIPLIER(40.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(358.0f) + SCREEN_MULTIPLIER(34.0f)), CRGBA(0, 0, 0, 50));
    // Up right
    CSprite2d::DrawRect(CRect(SCREEN_COORD(fPosX - 115.0f + 44.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(358.0f), SCREEN_COORD(fPosX - 115.0f + 42.0f) + SCREEN_MULTIPLIER(40.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(358.0f) + SCREEN_MULTIPLIER(34.0f)), CRGBA(0, 0, 0, 50));

    farcryTex.hud.m_pTexture = farcryTex.hudTxd.GetTexture(CROSS);
    farcryTex.hud.Draw(CRect(SCREEN_COORD(fPosX - 113.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(358.0f), SCREEN_COORD(fPosX - 114.0f) + SCREEN_MULTIPLIER(34.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(358.0f) + SCREEN_MULTIPLIER(34.0f)), CRGBA(110, 220, 110, 255));
    farcryTex.hud.m_pTexture = nullptr;

    //
    CFont::SetBackground(0, 0);
    CFont::SetAlignment(ALIGN_CENTER);
    CFont::SetProp(true);
    CFont::SetJustify(false);
    CFont::SetColor(CRGBA(255, 255, 255, 255));
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetOutlinePosition(0);
    CFont::SetCentreSize(SCREEN_WIDTH - SCREEN_COORD(350));
    CFont::SetScale(SCREEN_MULTIPLIER(0.6f), SCREEN_MULTIPLIER(1.5f));
    CFont::PrintStringFromBottom(SCREEN_COORD(fPosX - 115.0f + 62.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(329.0f), "0");

    // Bottom
    CPed *player = FindPlayerPed(0);

    CRGBA color;

    if (player->m_fHealth <= 40) {
        color.red = 150;
        color.green = 0;
        color.blue = 0;
        color.alpha = 100;
    }
    else {
        color.red = 0;
        color.green = 0;
        color.blue = 0;
        color.alpha = 100;
    }

    CSprite2d::DrawRect(CRect(SCREEN_COORD(fPosX - 119.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(112.0f), SCREEN_COORD(fPosX - 119.0f) + SCREEN_MULTIPLIER(fWidth * 2 + 8.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(117.0f) + SCREEN_MULTIPLIER(35.0f)), CRGBA(color));

    // Right side
    // Up Up :D
    CSprite2d::DrawRect(CRect(SCREEN_COORD_RIGHT(187.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(194.5f), SCREEN_COORD_RIGHT(187.5f) + SCREEN_MULTIPLIER(44.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(194.5f) + SCREEN_MULTIPLIER(40.0f)), CRGBA(0, 0, 0, 50));

    // Up left
    CSprite2d::DrawRect(CRect(SCREEN_COORD_RIGHT(179.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(152.0f), SCREEN_COORD_RIGHT(179.5f) + SCREEN_MULTIPLIER(36.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(152.0f) + SCREEN_MULTIPLIER(35.0f)), CRGBA(0, 0, 0, 50));
    // Up right
    CSprite2d::DrawRect(CRect(SCREEN_COORD_RIGHT(175.5f + 44.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(152.0f), SCREEN_COORD_RIGHT(175.5f + 44.0f) + SCREEN_MULTIPLIER(36.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(152.0f) + SCREEN_MULTIPLIER(35.0f)), CRGBA(0, 0, 0, 50));

    // Bottom right
    CSprite2d::DrawRect(CRect(SCREEN_COORD_RIGHT(195.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(112.0f), SCREEN_COORD_RIGHT(195.0f) + SCREEN_MULTIPLIER(52.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(112.0f) + SCREEN_MULTIPLIER(33.0f)), CRGBA(0, 0, 0, 50));

    // Bottom left
    CSprite2d::DrawRect(CRect(SCREEN_COORD_RIGHT(307.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(112.0f), SCREEN_COORD_RIGHT(307.5f) + SCREEN_MULTIPLIER(108.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(112.0f) + SCREEN_MULTIPLIER(33.0f)), CRGBA(0, 0, 0, 50));

    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));
}

void FarCry4Radar::DrawRadarCentre(CSprite2d* sprite, float x, float y, float angle, unsigned int width, unsigned int height, CRGBA color) {
    CRadar::DrawRotatingRadarSprite(sprite, x, y, -3.15, SCREEN_MULTIPLIER(21.5f), SCREEN_MULTIPLIER(21.5f), color);
}

void __fastcall FarCry4Radar::DrawStandardRadar() { 
    FarCry4Radar::DrawRadarSprites();
    plugin::CallMethodAndReturn<void __cdecl, 0x58A330>(0);
    plugin::CallMethodAndReturn<char __cdecl, 0x588050>(0);
}