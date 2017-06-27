#include "FarCry4Texture.h"
#include "game_sa\CTxdStore.h"
#include "game_sa\CFileLoader.h"
#include "game_sa\CTimer.h"

FarCry4Texture farcryTex;
FarCry4TexDictionary FarCry4Texture::crosshairTxd, FarCry4Texture::hudTxd;

void FarCry4TexDictionary::Load() {
    if (!m_pRwTexDictionary) {
        m_pRwTexDictionary = CFileLoader::LoadTexDictionary(m_szPath);
        if (m_pRwTexDictionary) {
            RwTexDictionaryForAllTextures(m_pRwTexDictionary, [](RwTexture *tex, void *data) {
                reinterpret_cast<std::vector<RwTexture *> *>(data)->push_back(tex);
                return tex;
            }, &m_textures);
        }
        m_nLastUsedTime = CTimer::m_snTimeInMillisecondsNonClipped;
    }
}

void FarCry4TexDictionary::Unload() {
    if (m_pRwTexDictionary) {
        RwTexDictionaryDestroy(m_pRwTexDictionary);
        m_pRwTexDictionary = nullptr;
        m_textures.clear();
    }
}

void FarCry4TexDictionary::AddToStorage() {
    FarCry4TxdStorage::Instance().AddTxd(this);
}

FarCry4TexDictionary::FarCry4TexDictionary() {
    m_szPath[0] = '\0';
    m_bInitialised = false;
    m_nLastUsedTime = 0;
}

void FarCry4TexDictionary::Init(char *path) {
    strcpy(m_szPath, path);
    m_pRwTexDictionary = nullptr;
    m_bInitialised = true;
    AddToStorage();
}

RwTexture *FarCry4TexDictionary::GetTexture(unsigned int id) {
    if (m_bInitialised) {
        Load();
        if (m_textures.size() > id) {
            m_nLastUsedTime = CTimer::m_snTimeInMillisecondsNonClipped;
            return m_textures[id];
        }
    }
    return nullptr;
}

void FarCry4TxdStorage::Process() {
    for (auto txd : m_txds) {
        if (CTimer::m_snTimeInMilliseconds - txd->m_nLastUsedTime > LOAD_UNLOAD_TIME)
            txd->Unload();
    }
}

void FarCry4TxdStorage::AddTxd(FarCry4TexDictionary *txd) {
    m_txds.push_back(txd);
}

void FarCry4TxdStorage::Shutdown() {
    for (auto txd : m_txds) {
        txd->Unload();
    }
}

FarCry4TxdStorage &FarCry4TxdStorage::Instance() {
    static FarCry4TxdStorage storage;
    return storage;
}

void FarCry4Texture::Setup() {
    crosshairTxd.Init(PLUGIN_PATH("FarCry4Hud\\crosshair.txd"));
    hudTxd.Init(PLUGIN_PATH("FarCry4Hud\\hud.txd"));
}