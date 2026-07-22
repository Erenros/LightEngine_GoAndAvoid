#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Hitbox.h"
#include <fstream>
#include <iostream>

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
        prb->SetGravity(false);
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

                Platform* p = CreateEntity<Platform>(gcle::Shapes::Rectangle);
                p->SetPosition(posX, posY);
                p->SetScale({ engineScale, engineScale });
                p->SetTexture("Assets");

                p->SetTag(1);
                p->SetRigidBody(true);
                p->GetRigidBody()->SetGravity(false);
                p->SetStatic(false);

                p->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });

                if (p->GetRenderShape() != nullptr)
                {
                    p->GetRenderShape()->SetTextureRect(srcX, srcY, static_cast<int16>(gridSize), static_cast<int16>(gridSize), TEXTURE_WIDTH, TEXTURE_HEIGHT);
                }

#ifdef _DEBUG
                DebugPlatform* debugPlat = CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
                debugPlat->SetTarget(p);
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
                    slime->SetScale({ GLOBAL_SCALE / 5, GLOBAL_SCALE / 5 });

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
            m_pCamera->SetPosition({ playerPos.x + 250.0f, playerPos.y - 150.0f });
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