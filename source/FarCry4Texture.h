#pragma once
#include <plugin.h>
#include "game_sa\CSprite2d.h"

enum eCrosshairTexture {
    RIFLE,
    DOT
};

enum eHudTexture {
    CROSS
};

class FarCry4TexDictionary {
    friend class FarCry4TxdStorage;
    char m_szPath[MAX_PATH];
    RwTexDictionary *m_pRwTexDictionary;
    std::vector<RwTexture *> m_textures;
    int m_nLastUsedTime;
    bool m_bInitialised;

    void Load();
    void Unload();
    void AddToStorage();
public:
    FarCry4TexDictionary();
    void Init(char *path);
    RwTexture *GetTexture(unsigned int id = 0);
};

class FarCry4TxdStorage {
    std::vector<FarCry4TexDictionary *> m_txds;
    static const int LOAD_UNLOAD_TIME = 3000;
public:
    static FarCry4TxdStorage &Instance();
    void Process();
    void AddTxd(FarCry4TexDictionary *txd);
    void Shutdown();
};

class FarCry4Texture {
public:
    CSprite2d crosshair;
    CSprite2d hud;

    static FarCry4TexDictionary crosshairTxd, hudTxd;

public:
    static void Setup();
};

extern FarCry4Texture farcryTex;