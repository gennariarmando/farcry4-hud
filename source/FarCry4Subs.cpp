#include <plugin.h>
#include "game_sa\CFont.h"
#include "FarCry4Subs.h"

void FarCry4Subs::InjectPatches() {
    plugin::patch::RedirectCall(0x58C68A, DrawSubtitles);
}

void FarCry4Subs::DrawSubtitles(float x, float y, char *str) {
    CFont::SetBackground(0, 0);
    CFont::SetAlignment(ALIGN_CENTER);
    CFont::SetProp(true);
    CFont::SetJustify(false);
    CFont::SetColor(CRGBA(255, 255, 255, 255));
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetOutlinePosition(0);
    CFont::SetCentreSize(SCREEN_WIDTH - SCREEN_COORD(350));
    CFont::SetScale(SCREEN_MULTIPLIER(0.9f), SCREEN_MULTIPLIER(1.9f));
    CFont::PrintStringFromBottom(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(100.0f), str);
}