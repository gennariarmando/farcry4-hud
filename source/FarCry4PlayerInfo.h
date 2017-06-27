#pragma once

class FarCry4PlayerInfo {
public:
    static void InjectPatches();
    static void __cdecl RenderHealthAndArmourBar(float posX, float posY, unsigned __int16 width, unsigned int height, float progress, int progressAdd, bool drawPercentage, bool drawBlackBorder, CRGBA color, CRGBA progressAddColor);
    static void DrawWeaponAmmo(float x, float y, char *str);
    static void DrawWeaponIcon(CPed *player);
    static void RenderCrosshair();
};