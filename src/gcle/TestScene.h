#pragma once
#include "Engine/Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Platform.h"
#include "Decor.h"
#include "WorldText.h"
#include "Render/Text.h"
#include "Slime.h"
#include "LevelStreamer.h"
#include <string>

class TestScene : public Scene
{
public:
    void OnInitialize() override;
    void OnUpdate(Clock& time) override;
    void OnExit() override;

private:
    void LoadLevel(const std::string& filepath);
    void UpdateScoreDisplay();
    void UpdatePlayerDebugText(Vector2f playerPosition);
    void UpdateParallaxBackground(Clock& time, Vector2f playerPosition);

    Camera* m_pCamera = nullptr;
    Player* m_pPlayer = nullptr;
    Enemy* m_pEnemy = nullptr;
    WorldText* m_pPlayerDebugText = nullptr;

    Text* m_pScoreText = nullptr;
    Text* m_pDistanceText = nullptr;

    Decor* m_pBackground1[2] = { nullptr, nullptr };
    Decor* m_pBackground2[2] = { nullptr, nullptr };

    LevelStreamer m_LevelStreamer;

    bool m_WasControllerBackHeld = false;

    int32 m_LastDisplayedScore = -1;
    int32 m_LastDisplayedDistance = -1;
    int32 m_DebugTextRefreshCounter = 0;

    static constexpr int32 DEBUG_TEXT_REFRESH_INTERVAL = 6;
};