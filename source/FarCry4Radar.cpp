#include "plugin.h"
#include "game_sa\common.h"
#include "game_sa\CMenuManager.h"
#include "FarCry4Radar.h"

float fPosX = 260.0f;
float fPosY = 216.0f;
float fWidth = 115.0f;
float fHeight = 91.0f;
int iRadarAlpha = 150;

void FarCry4Radar::InjectPatches() {
    // Set Radar Range
    static const float fRadarRange = 100.0f;
    plugin::patch::SetFloat(0x586C9B, 100.0f);
    plugin::patch::Set(0x586C66, &fRadarRange);

    // Transparent Radar
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

    // Blips
    plugin::patch::Set(0x5BA85F + 1, PLUGIN_PATH("FarCry4Hud\\blips.txd"));

    // Draw Radar
    plugin::patch::RedirectCall(0x58FC53, DrawStandardRadar);
}

void __declspec(naked) FarCry4Radar::RadarAlpha() {
    RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    _asm
    {
        mov		ecx, 586420h
        mov		al, [esp + 140h - 12Dh]
        test	al, al
        jmp		ecx
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

    // Hud Rectangle
    CSprite2d::DrawRect(CRect(SCREEN_COORD(fPosX - 119.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(112.0f), SCREEN_COORD(fPosX - 119.0f) + SCREEN_MULTIPLIER(fWidth * 2 + 8.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(117.0f) + SCREEN_MULTIPLIER(35.0f)), CRGBA(0, 0, 0, 100));

    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(savedAlpha));
}

void __fastcall FarCry4Radar::DrawStandardRadar() { 
    FarCry4Radar::DrawRadarSprites();
    plugin::CallMethodAndReturn<void __cdecl, 0x58A330>(0);
    plugin::CallMethodAndReturn<char __cdecl, 0x588050>(0);
}