#pragma once
#include "Engine/Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Platform.h"
#include "Decor.h"
#include "WorldText.h"
#include "Render/Text.h"
#include "Slime.h"
#include <string>

class TestScene : public Scene
{
public:
    void OnInitialize() override;
    void OnUpdate(Clock& time) override;

private:
    void LoadLevel(const std::string& filepath);

    Camera* m_pCamera = nullptr;
    Player* m_pPlayer = nullptr;
    Enemy* m_pEnemy = nullptr;
    WorldText* m_pPlayerDebugText = nullptr;

    Text* m_pScoreText = nullptr;
    Text* m_pDistanceText = nullptr;
};