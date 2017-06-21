#include <plugin.h>
#include "game_sa\common.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CWorld.h"
#include "game_sa\CStats.h"
#include "FarCry4PlayerInfo.h"

void FarCry4PlayerInfo::InjectPatches() {
    plugin::patch::RedirectCall(0x589395, DrawHealthAndBreathBar);
    plugin::patch::RedirectCall(0x58917A, DrawArmourBar);
    plugin::patch::Set(0x589252, 5);
}

void __cdecl FarCry4PlayerInfo::DrawHealthAndBreathBar(float posX, float posY, unsigned __int16 width, unsigned int height, float progress, int progressAdd, bool drawPercentage, bool drawBlackBorder, CRGBA color, CRGBA progressAddColor) {
    CSprite2d::DrawBarChart(SCREEN_COORD(148.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(99.0f), SCREEN_MULTIPLIER(109.0f), SCREEN_MULTIPLIER(10.0f), progress, progressAdd, drawPercentage, 0, CRGBA(125, 220, 125, 255), CRGBA(0, 0, 0, 0));

    float fBreath = CWorld::Players[0].m_pPed->m_pPlayerData->m_fBreath / CStats::GetFatAndMuscleModifier(STAT_MOD_AIR_IN_LUNG);
    float fBreathProgress = fBreath * 100.0f;

    CSprite2d::DrawBarChart(SCREEN_COORD(148.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(106.0f), SCREEN_MULTIPLIER(224.5f), SCREEN_MULTIPLIER(3.5f), fBreathProgress, 0, 0, 0, CRGBA(255, 255, 255, 255), CRGBA(0, 0, 0, 0));
}

void __cdecl FarCry4PlayerInfo::DrawArmourBar(float posX, float posY, unsigned __int16 width, unsigned int height, float progress, int progressAdd, bool drawPercentage, bool drawBlackBorder, CRGBA color, CRGBA progressAddColor) {
    CSprite2d::DrawBarChart(SCREEN_COORD(150.0f + 113.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(99.0f), SCREEN_MULTIPLIER(109.0f), SCREEN_MULTIPLIER(10.0f), progress, progressAdd, drawPercentage, 0, CRGBA(155, 205, 255, 255), CRGBA(0, 0, 0, 0));
}