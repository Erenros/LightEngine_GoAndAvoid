#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Hitbox.h"
#include "ScoreManager.h"
#include "PlatformEntry.h"
#include "DecorEntry.h"
#include "SlimeEntry.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <cmath>

#ifdef _DEBUG
#include "DebugPlayer.h"
#include "DebugHitbox.h"
#include "DebugPlatform.h"
#include "DebugKillableEntity.h"
#endif

#include "../lib/nlohmann/json.hpp"
using json = nlohmann::json;

namespace
{
    constexpr float GLOBAL_SCALE = 4.0f;
    constexpr float PLAYER_SCALE = 0.8f;
    constexpr float ENEMY_SCALE = 0.6f;
    constexpr int16 TEXTURE_WIDTH = 400;
    constexpr int16 TEXTURE_HEIGHT = 400;

    constexpr double BG_WIDTH = 1920.0;

    constexpr float STREAM_LOAD_DISTANCE = 2200.0f;
    constexpr float STREAM_UNLOAD_DISTANCE = 2600.0f;

    constexpr const char* PAUSE_SCENE_TAG = "PauseScene";
    constexpr const char* MAIN_MENU_SCENE_TAG = "MainMenu";
}

void TestScene::OnInitialize()
{
    Scene::OnInitialize();

    ScoreManager::GetInstance().Reset();

    m_pCamera = CreateCamera();
    SwitchCamera(m_pCamera);

    AddDrawnTexture("Background_2");
    AddDrawnTexture("Background_1");
    AddDrawnTexture("Assets");
    AddDrawnTexture("slime");

    for (int i = 0; i < 2; ++i)
    {
        m_pBackground2[i] = CreateEntity<Decor>(gcle::Shapes::Rectangle);
        m_pBackground2[i]->SetTexture("Background_2");
        m_pBackground2[i]->SetScale({ 19.2f, 10.8f });

        m_pBackground1[i] = CreateEntity<Decor>(gcle::Shapes::Rectangle);
        m_pBackground1[i]->SetTexture("Background_1");
        m_pBackground1[i]->SetScale({ 19.2f, 10.8f });
    }

    m_LevelStreamer.Initialize(this, STREAM_LOAD_DISTANCE, STREAM_UNLOAD_DISTANCE);

    LoadLevel("../../assets/LDTK/Lv1.ldtk");

    m_pPlayer = CreateEntity<Player>(gcle::Shapes::Rectangle);
    m_pPlayer->SetPosition(0.0f, 800.0f);
    m_pPlayer->SetScale({ PLAYER_SCALE, PLAYER_SCALE });

    m_pPlayer->SetRigidBody(true);

    RigidBody2D* prb = m_pPlayer->GetRigidBody();
    if (prb != nullptr)
    {
        prb->SetFriction({ 0.0f, 0.0f });
        prb->SetActive(true);
        prb->SetGravity(true);
        prb->SetCollisionOnContinuous();
    }

    m_pPlayer->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });

#ifdef _DEBUG
    DebugHitbox* playerHitbox = CreateEntity<DebugHitbox>(gcle::Shapes::Rectangle);
#else
    Hitbox* playerHitbox = CreateEntity<Hitbox>(gcle::Shapes::Rectangle);
#endif
    playerHitbox->SetOwner(m_pPlayer);
    m_pPlayer->SetHitbox(playerHitbox);

#ifdef _DEBUG
    DebugPlayer* debugPlayer = CreateEntity<DebugPlayer>(gcle::Shapes::Rectangle);
    debugPlayer->SetTarget(m_pPlayer);
#endif

    Entity* textEntity = CreateWorldText("Init...", 20, "street-fighter", 255, 255, 0, 255);
    m_pPlayerDebugText = static_cast<WorldText*>(textEntity);

    m_pScoreText = CreateText("Score: 0", { 20.0f, 900.0f }, 28, 255, 255, 0);
    m_pScoreText->SetFont("street-fighter");
    m_pDistanceText = CreateText("Distance: 0m", { 20.0f, 935.0f }, 28, 255, 255, 0);
    m_pDistanceText->SetFont("street-fighter");

    m_LastDisplayedScore = 0;
    m_LastDisplayedDistance = 0;
    m_DebugTextRefreshCounter = 0;

    m_LevelStreamer.Update(m_pPlayer->GetPosition().x);
}

void TestScene::LoadLevel(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        return;
    }

    json data;
    file >> data;

    std::unordered_map<int, std::string> tileCustomData;
    if (data.contains("defs") && data["defs"].contains("tilesets"))
    {
        for (const auto& tileset : data["defs"]["tilesets"])
        {
            if (tileset.contains("customData"))
            {
                for (const auto& customData : tileset["customData"])
                {
                    tileCustomData[customData["tileId"].get<int>()] = customData["data"].get<std::string>();
                }
            }
        }
    }

    auto levels = data["levels"];
    if (levels.empty()) return;

    const std::vector<std::string> targetOrder = { "Decor", "Platformes", "Entities" };

    for (const auto& targetLayerName : targetOrder)
    {
        for (const auto& layer : levels[0]["layerInstances"])
        {
            std::string layerName = layer["__identifier"];

            if (layerName != targetLayerName)
            {
                continue;
            }

            float gridSize = layer["__gridSize"];
            float targetSize = gridSize * GLOBAL_SCALE;
            float engineScale = targetSize / 100.0f;

            if (layerName == "Platformes")
            {
                for (const auto& tile : layer["gridTiles"])
                {
                    float posX = tile["px"][0] * GLOBAL_SCALE;
                    float posY = tile["px"][1] * GLOBAL_SCALE;

                    int16 srcX = tile["src"][0];
                    int16 srcY = tile["src"][1];
                    int tileId = tile["t"];

                    std::string slopeType = "";
                    if (tileCustomData.find(tileId) != tileCustomData.end())
                    {
                        slopeType = tileCustomData[tileId];
                    }

                    Vector2f position{ posX, posY };
                    Vector2f scale{ engineScale, engineScale };
                    int16 gridSize16 = static_cast<int16>(gridSize);

                    if (slopeType == "Water")
                    {
                        m_LevelStreamer.AddEntry(std::make_unique<DecorEntry>(
                            position, scale, srcX, srcY, gridSize16, TEXTURE_WIDTH, TEXTURE_HEIGHT, true));
                        continue;
                    }

                    m_LevelStreamer.AddEntry(std::make_unique<PlatformEntry>(
                        position, scale, srcX, srcY, gridSize16, TEXTURE_WIDTH, TEXTURE_HEIGHT, engineScale, slopeType));
                }
            }
            else if (layerName == "Decor")
            {
                for (const auto& tile : layer["gridTiles"])
                {
                    float posX = tile["px"][0] * GLOBAL_SCALE;
                    float posY = tile["px"][1] * GLOBAL_SCALE;

                    int16 srcX = tile["src"][0];
                    int16 srcY = tile["src"][1];

                    int tileId = tile["t"];

                    std::string customType = "";
                    if (tileCustomData.find(tileId) != tileCustomData.end())
                    {
                        customType = tileCustomData[tileId];
                    }

                    Vector2f position{ posX, posY };
                    Vector2f scale{ engineScale, engineScale };
                    int16 gridSize16 = static_cast<int16>(gridSize);

                    m_LevelStreamer.AddEntry(std::make_unique<DecorEntry>(
                        position, scale, srcX, srcY, gridSize16, TEXTURE_WIDTH, TEXTURE_HEIGHT, customType == "Water"));
                }
            }
            else if (layerName == "Entities")
            {
                for (const auto& entityInstance : layer["entityInstances"])
                {
                    std::string entityIdentifier = entityInstance["__identifier"];

                    float posX = entityInstance["px"][0] * GLOBAL_SCALE;
                    float posY = entityInstance["px"][1] * GLOBAL_SCALE;

                    if (entityIdentifier == "Slime")
                    {
                        Vector2f position{ posX, posY };
                        Vector2f scale{ GLOBAL_SCALE / 5.0f, GLOBAL_SCALE / 5.0f };

                        m_LevelStreamer.AddEntry(std::make_unique<SlimeEntry>(position, scale));
                    }
                }
            }
        }
    }

    m_LevelStreamer.FinalizeLoading();
}

void TestScene::UpdateScoreDisplay()
{
    int32 currentScore = ScoreManager::GetInstance().GetScore();
    if (m_pScoreText != nullptr && currentScore != m_LastDisplayedScore)
    {
        m_pScoreText->SetText("Score: " + std::to_string(currentScore));
        m_LastDisplayedScore = currentScore;
    }

    int32 currentDistance = ScoreManager::GetInstance().GetDistance();
    if (m_pDistanceText != nullptr && currentDistance != m_LastDisplayedDistance)
    {
        m_pDistanceText->SetText("Distance: " + std::to_string(currentDistance) + "m");
        m_LastDisplayedDistance = currentDistance;
    }
}

void TestScene::UpdatePlayerDebugText(Vector2f playerPosition)
{
    if (m_pPlayerDebugText == nullptr || m_pPlayerDebugText->GetText() == nullptr)
    {
        return;
    }

    m_pPlayerDebugText->SetRenderPosition({ playerPosition.x, playerPosition.y - 60.0f });

    ++m_DebugTextRefreshCounter;
    if (m_DebugTextRefreshCounter < DEBUG_TEXT_REFRESH_INTERVAL)
    {
        return;
    }
    m_DebugTextRefreshCounter = 0;

    std::string debugStr = "HP: " + std::to_string(m_pPlayer->GetHp()) +
        " | X: " + std::to_string(static_cast<int32>(playerPosition.x)) +
        " | Y: " + std::to_string(static_cast<int32>(playerPosition.y));
    m_pPlayerDebugText->GetText()->SetText(debugStr);
}

void TestScene::UpdateParallaxBackground(Clock& time, Vector2f playerPosition)
{
    if (m_pCamera == nullptr)
    {
        return;
    }

    Vector2f currentPos = m_pCamera->GetPosition();
    Vector2f targetPos = { playerPosition.x + 250.0f, playerPosition.y - 150.0f };
    float dt = static_cast<float>(time.GetDeltaTime());

    float newX = currentPos.x + (targetPos.x - currentPos.x) * 10.0f * dt;
    float newY = currentPos.y + (targetPos.y - currentPos.y) * 10.0f * dt;

    m_pCamera->SetPosition({ newX, newY });

    Vector2f camPos = m_pCamera->GetPosition();

    double camX = static_cast<double>(camPos.x);
    double parallax1 = 0.7;
    double parallax2 = 0.9;

    double offset1 = std::fmod(camX * (1.0 - parallax1), BG_WIDTH);
    if (offset1 < 0.0) offset1 += BG_WIDTH;

    double offset2 = std::fmod(camX * (1.0 - parallax2), BG_WIDTH);
    if (offset2 < 0.0) offset2 += BG_WIDTH;

    for (int i = 0; i < 2; ++i)
    {
        if (m_pBackground1[i] != nullptr)
        {
            m_pBackground1[i]->SetPosition(static_cast<float>(camX - offset1 + (i * BG_WIDTH)), camPos.y);
        }
        if (m_pBackground2[i] != nullptr)
        {
            m_pBackground2[i]->SetPosition(static_cast<float>(camX - offset2 + (i * BG_WIDTH)), camPos.y);
        }
    }
}

void TestScene::OnUpdate(Clock& time)
{
    Scene::OnUpdate(time);

    InputManager& input = InputManager::GetInstance();

    bool isControllerBackHeld = input.IsControllerDown(0, XBOX_BACK);
    bool controllerBackPressed = isControllerBackHeld && !m_WasControllerBackHeld;
    m_WasControllerBackHeld = isControllerBackHeld;

    if (input.IsDown('P'))
    {
        SceneManager::GetInstance().SetCurrentSceneWithTag(PAUSE_SCENE_TAG, true);
        return;
    }

    if (input.IsDown(Escape) || input.IsDown(Backspace) || input.IsDown('M') || controllerBackPressed)
    {
        SceneManager::GetInstance().SetCurrentSceneWithTag(MAIN_MENU_SCENE_TAG, false);
        return;
    }

    UpdateScoreDisplay();

    if (m_pPlayer != nullptr)
    {
        if (m_pPlayer->ToDestroy())
        {
            m_pPlayer = nullptr;
            return;
        }

        Vector2f playerPos = m_pPlayer->GetPosition();

        m_LevelStreamer.Update(playerPos.x);

        UpdateParallaxBackground(time, playerPos);
        UpdatePlayerDebugText(playerPos);
    }
}

void TestScene::OnExit()
{
    m_LevelStreamer.Shutdown();
    Scene::OnExit();
}