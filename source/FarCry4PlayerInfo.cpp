#include <plugin.h>
#include "game_sa\common.h"
#include "game_sa\CFont.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CWorld.h"
#include "game_sa\CStats.h"
#include "game_sa\CCamera.h"
#include "game_sa\CTheScripts.h"
#include "FarCry4PlayerInfo.h"
#include "FarCry4Texture.h"
#include "Utility.h"

void FarCry4PlayerInfo::InjectPatches() {
    plugin::patch::Set(0x58EC21, 5); // Disable default clock
    plugin::patch::Set(0x58F607, 5); // Disable default money counter
    plugin::patch::Set(0x58917A, 5); // Disable default armour
    plugin::patch::Set(0x589252, 5); // Disable default breath
    plugin::patch::Set<BYTE>(0x58FC2C, 0xEB); // Disable default statsbox

    plugin::patch::RedirectCall(0x589395, RenderHealthAndArmourBar);
    plugin::patch::RedirectCall(0x58962A, DrawWeaponAmmo);
    plugin::patch::RedirectJump(0x58D7D0, DrawWeaponIcon);
    plugin::patch::RedirectJump(0x58E020, RenderCrosshair);
}

void __cdecl FarCry4PlayerInfo::RenderHealthAndArmourBar(float posX, float posY, unsigned __int16 width, unsigned int height, float progress, int progressAdd, bool drawPercentage, bool drawBlackBorder, CRGBA color, CRGBA progressAddColor) {
    CPed *player = FindPlayerPed(0);

    if (player->m_fHealth <= 30) {
        if (ShowFlashingItem(250, 250)) {
            // Health
            DrawProgressBar(SCREEN_COORD(148.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(99.0f), SCREEN_MULTIPLIER(35.8f), SCREEN_MULTIPLIER(10.0f), 1, 2.0f, player->m_fHealth / 100, CRGBA(140, 0, 0, 255), CRGBA(0, 0, 0, 255 / 2));

            // Armour
            DrawProgressBar(SCREEN_COORD(148.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(106.0f), SCREEN_MULTIPLIER(224.5f), SCREEN_MULTIPLIER(3.0f), 0, 0.0f, player->m_fArmour / 100, CRGBA(255, 255, 255, 255), CRGBA(125, 125, 125, 255 / 2));
        }
    }
    else {
            // Health
            DrawProgressBar(SCREEN_COORD(148.5f), SCREEN_COORD_MAX_Y - SCREEN_COORD(99.0f), SCREEN_MULTIPLIER(35.8f), SCREEN_MULTIPLIER(10.0f), 1, 2.0f, player->m_fHealth / 100, CRGBA(125, 220, 125, 255), CRGBA(0, 0, 0, 255 / 2));

            // Armour
            DrawProgressBar(SCREEN_COORD(148.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(106.0f), SCREEN_MULTIPLIER(224.5f), SCREEN_MULTIPLIER(3.0f), 0, 0.0f, player->m_fArmour / 100, CRGBA(255, 255, 255, 255), CRGBA(125, 125, 125, 255 / 2));
    }
}

void FarCry4PlayerInfo::DrawWeaponAmmo(float x, float y, char *str) {
    CFont::SetBackground(0, 0);
    CFont::SetAlignment(ALIGN_RIGHT);
    CFont::SetProp(true);
    CFont::SetJustify(false);
    CFont::SetColor(CRGBA(255, 255, 255, 255));
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetOutlinePosition(0);
    CFont::SetScale(SCREEN_MULTIPLIER(0.6f), SCREEN_MULTIPLIER(1.5f));
    CFont::PrintStringFromBottom(SCREEN_COORD_RIGHT(205.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(85.0f), str); 
    CFont::PrintStringFromBottom(SCREEN_COORD_RIGHT(160.0f), SCREEN_COORD_MAX_Y - SCREEN_COORD(85.0f), "0");
}

void FarCry4PlayerInfo::DrawWeaponIcon(CPed *player) {

}

void __cdecl FarCry4PlayerInfo::RenderCrosshair() {
    CPed *player = FindPlayerPed(-1);

    WORD mode = TheCamera.m_aCams[TheCamera.m_nActiveCam].m_eMode; 

    float fDotSize = 12.0f;
    float fDotX = SCREEN_COORD_CENTER_X - SCREEN_COORD(fDotSize / 2);
    float fDotY = SCREEN_COORD_CENTER_Y - SCREEN_COORD(fDotSize / 2);
    float fRifleSize = 176.0f;
    float fRifleX = SCREEN_COORD_CENTER_X - SCREEN_COORD(fRifleSize / 2);
    float fRifleY = SCREEN_COORD_CENTER_Y - SCREEN_COORD(fRifleSize / 2);

    // If FirstPerson Mod is loaded.
    if (GetModuleHandle("Hooks.asi")) {
        if (mode == 53 || mode == 55 || mode == 65 || mode == 34 || mode == 42 || mode == 41 || mode == 45) {
            if (FindPlayerVehicle(-1, 0)) {}
            else {
                switch (CWeaponInfo::GetWeaponInfo((eWeaponType)player->m_aWeapons[player->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1) {
                case MODEL_AK47:
                    farcryTex.crosshair.m_pTexture = farcryTex.crosshairTxd.GetTexture(RIFLE);
                    farcryTex.crosshair.Draw(CRect(fRifleX, fRifleY, fRifleX + SCREEN_MULTIPLIER(fRifleSize), fRifleY + SCREEN_MULTIPLIER(fRifleSize)), CRGBA(255, 255, 255, 255));
                    break;
                case MODEL_M4:
                    farcryTex.crosshair.m_pTexture = farcryTex.crosshairTxd.GetTexture(RIFLE);
                    farcryTex.crosshair.Draw(CRect(fRifleX, fRifleY, fRifleX + SCREEN_MULTIPLIER(fRifleSize), fRifleY + SCREEN_MULTIPLIER(fRifleSize)), CRGBA(255, 255, 255, 255));
                    break;
                default:
                    farcryTex.crosshair.m_pTexture = farcryTex.crosshairTxd.GetTexture(DOT);
                    farcryTex.crosshair.Draw(CRect(fDotX, fDotY, fDotX + SCREEN_MULTIPLIER(fDotSize), fDotY + SCREEN_MULTIPLIER(fDotSize)), CRGBA(255, 255, 255, 255));
                    break;
                }
                farcryTex.crosshair.m_pTexture = nullptr;
            }
        }
    }
}