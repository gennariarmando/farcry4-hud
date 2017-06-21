#pragma once
#include "game_sa\CSprite2d.h"
#include "game_sa\CVector2D.h"

class FarCry4Radar {
public:
    static void InjectPatches();
    static void RadarAlpha();
    static long double __cdecl LimitRadarPoint(CVector2D *pos);
    static void TransformRadarPointToScreenSpace(CVector2D * out, CVector2D * in);
    static void DrawRadarSprites();
    static void __fastcall DrawStandardRadar();
};