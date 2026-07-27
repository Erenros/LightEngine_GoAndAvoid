#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Hitbox.h"
#include "ScoreManager.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

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
    constexpr float PLAYER_SCALE = 0.6f;
    constexpr float ENEMY_SCALE = 0.6f;
    constexpr int16 TEXTURE_WIDTH = 400;
    constexpr int16 TEXTURE_HEIGHT = 400;
}

void TestScene::OnInitialize()
{
    Scene::OnInitialize();

    ScoreManager::GetInstance().Reset();

    m_pCamera = CreateCamera();
    SwitchCamera(m_pCamera);

    LoadLevel("../../assets/LDTK/Lv1.ldtk");
    AddDrawnTexture("Assets");
    AddDrawnTexture("slime");

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

    Entity* textEntity = CreateWorldText("Init...", 20, "Hack-Regular", 255, 255, 0, 255);
    m_pPlayerDebugText = static_cast<WorldText*>(textEntity);

    m_pScoreText = CreateText("Score: 0", { 20.0f, 900.0f }, 28, 255, 255, 0);
    m_pDistanceText = CreateText("Distance: 0m", { 20.0f, 935.0f }, 28, 255, 255, 0);
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

    for (const auto& layer : levels[0]["layerInstances"])
    {
        std::string layerName = layer["__identifier"];
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

                Platform* p = CreateEntity<Platform>(gcle::Shapes::Rectangle);
                p->SetPosition(posX, posY);
                p->SetScale({ engineScale, engineScale });
                p->SetTexture("Assets");

                p->SetTag(1);
                p->SetRigidBody(true);
                p->GetRigidBody()->SetGravity(false);
                p->SetStatic(false);

                p->SetTag(1);
                p->SetRigidBody(true);
                p->GetRigidBody()->SetGravity(false);
                p->SetStatic(false);

                if (slopeType == "Spikes")
                {
                    p->SetType(PlatformType::Spikes);
                }

                p->BuildColliders(slopeType, engineScale);

                if (p->GetRenderShape() != nullptr)
                {
                    p->GetRenderShape()->SetTextureRect(srcX, srcY, static_cast<int16>(gridSize), static_cast<int16>(gridSize), TEXTURE_WIDTH, TEXTURE_HEIGHT);
                }

#ifdef _DEBUG
                float yOffsetDbg = 25.0f;

                if (slopeType == "SlopeRight")
                {
                    auto dp = CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
                    dp->SetTarget(p, { 0.0f, 0.0f }, { -1.0f, 1.0f });
                }
                else if (slopeType == "SlopeLeft")
                {
                    auto dp = CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
                    dp->SetTarget(p, { 0.0f, 0.0f }, { 1.0f, 1.0f });
                }
                else if (slopeType == "SlopeRight_Low")
                {
                    auto dp = CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
                    dp->SetTarget(p, { 0.0f, yOffsetDbg }, { -1.0f, 0.5f });
                }
                else if (slopeType == "SlopeLeft_Low")
                {
                    auto dp = CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
                    dp->SetTarget(p, { 0.0f, yOffsetDbg }, { 1.0f, 0.5f });
                }
                else if (slopeType == "SlopeRight_High")
                {
                    auto dp1 = CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
                    dp1->SetTarget(p, { 0.0f, yOffsetDbg }, { 1.0f, 0.5f });
                    auto dp2 = CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
                    dp2->SetTarget(p, { 0.0f, -yOffsetDbg }, { -1.0f, 0.5f });
                }
                else if (slopeType == "SlopeLeft_High")
                {
                    auto dp1 = CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
                    dp1->SetTarget(p, { 0.0f, yOffsetDbg }, { 1.0f, 0.5f });
                    auto dp2 = CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
                    dp2->SetTarget(p, { 0.0f, -yOffsetDbg }, { 1.0f, 0.5f });
                }
                else
                {
                    auto dp = CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
                    dp->SetTarget(p, { 0.0f, 0.0f }, { 1.0f, 1.0f });
                }
#endif
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

                Decor* decor = CreateEntity<Decor>(gcle::Shapes::Rectangle);
                decor->SetPosition(posX, posY);
                decor->SetScale({ engineScale, engineScale });
                decor->SetTexture("Assets");

                if (decor->GetRenderShape() != nullptr)
                {
                    decor->GetRenderShape()->SetTextureRect(srcX, srcY, static_cast<int16>(gridSize), static_cast<int16>(gridSize), TEXTURE_WIDTH, TEXTURE_HEIGHT);
                }
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
                    Slime* slime = CreateEntity<Slime>(gcle::Shapes::Rectangle);
                    slime->SetPosition(posX, posY);
                    slime->SetScale({ GLOBAL_SCALE / 5.0f, GLOBAL_SCALE / 5.0f });

#ifdef _DEBUG
                    DebugKillableEntity* debugEnemy = CreateEntity<DebugKillableEntity>(gcle::Shapes::Rectangle);
                    debugEnemy->SetTarget(slime);
#endif
                }
            }
        }
    }
}

void TestScene::OnUpdate(Clock& time)
{
    Scene::OnUpdate(time);

    if (m_pScoreText != nullptr)
    {
        m_pScoreText->SetText("Score: " + std::to_string(ScoreManager::GetInstance().GetScore()));
    }

    if (m_pDistanceText != nullptr)
    {
        m_pDistanceText->SetText("Distance: " + std::to_string(ScoreManager::GetInstance().GetDistance()) + "m");
    }

    if (m_pPlayer != nullptr)
    {
        if (m_pPlayer->ToDestroy())
        {
            m_pPlayer = nullptr;
            return;
        }

        Vector2f playerPos = m_pPlayer->GetPosition();

        if (m_pCamera != nullptr)
        {
            Vector2f currentPos = m_pCamera->GetPosition();
            Vector2f targetPos = { playerPos.x + 250.0f, playerPos.y - 150.0f };
            float dt = static_cast<float>(time.GetDeltaTime());

            float newX = currentPos.x + (targetPos.x - currentPos.x) * 10.0f * dt;
            float newY = currentPos.y + (targetPos.y - currentPos.y) * 10.0f * dt;

            m_pCamera->SetPosition({ newX, newY });
        }

        if (m_pPlayerDebugText != nullptr && m_pPlayerDebugText->GetText() != nullptr)
        {
            m_pPlayerDebugText->SetRenderPosition({ playerPos.x, playerPos.y - 60.0f });
            std::string debugStr = "HP: " + std::to_string(m_pPlayer->GetHp()) +
                " | X: " + std::to_string(static_cast<int32>(playerPos.x)) +
                " | Y: " + std::to_string(static_cast<int32>(playerPos.y));
            m_pPlayerDebugText->GetText()->SetText(debugStr);
        }
    }
}