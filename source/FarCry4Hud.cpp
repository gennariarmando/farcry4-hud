#include <plugin.h>
#include "FarCry4PlayerInfo.h"
#include "FarCry4Radar.h"
#include "FarCry4Subs.h"

class FarCryHud {
public:
    FarCryHud() {
        FarCry4PlayerInfo::InjectPatches();
        FarCry4Radar::InjectPatches();
        FarCry4Subs::InjectPatches();
    };
} FaryCryHud;