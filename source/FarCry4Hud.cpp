#include <plugin.h>
#include "FarCry4PlayerInfo.h"
#include "FarCry4Radar.h"
#include "FarCry4Subs.h"
#include "FarCry4Texture.h"

class FarCryHud {
public:
    FarCryHud() {
        FarCry4PlayerInfo::InjectPatches();
        FarCry4Radar::InjectPatches();
        FarCry4Subs::InjectPatches();
        plugin::Events::initRwEvent += [] {
            FarCry4Texture::Setup();

        };
    };
} FaryCryHud;