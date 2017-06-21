#pragma once

class FarCry4PlayerInfo {
public:
    static void InjectPatches();
    static void __cdecl DrawHealthAndBreathBar(float posX, float posY, unsigned __int16 width, unsigned int height, float progress, int progressAdd, bool drawPercentage, bool drawBlackBorder, CRGBA color, CRGBA progressAddColor);
    static void __cdecl DrawArmourBar(float posX, float posY, unsigned __int16 width, unsigned int height, float progress, int progressAdd, bool drawPercentage, bool drawBlackBorder, CRGBA color, CRGBA progressAddColor);
};