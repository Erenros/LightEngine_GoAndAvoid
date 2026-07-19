#include "TestScene.h"
#include "Engine/SceneManager.h"
#include <fstream>
#include <iostream>

// OBLIGATOIRE : Tu dois ajouter cette bibliothèque à ton projet pour que ça compile
#include "../lib/nlohmann/json.hpp"
using json = nlohmann::json;

void TestScene::OnInitialize()
{
    Scene::OnInitialize();

    m_pCamera = CreateCamera();
    SwitchCamera(m_pCamera);

    // Chargement du vrai niveau
    LoadLevel("C:\\Users\\X1605\\Downloads\\gcle_Gameplay-main\\gcle_Gameplay-main\\assets\\LDTK\\Lv1.ldtk");
    AddDrawnTexture("Assets");
    m_pPlayer = CreateEntity<Player>(gcle::Shapes::Rectangle);
    m_pPlayer->SetPosition(0.0f, 200.0f);
    m_pPlayer->SetScale({ 1.0f, 1.0f });
    m_pPlayer->SetRigidBody(true);
    m_pPlayer->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });

    m_pEnemy = CreateEntity<Enemy>(gcle::Shapes::Rectangle);
    m_pEnemy->SetPosition(800.0f, 350.0f);
    m_pEnemy->SetScale({ 1.0f, 1.0f });
    m_pEnemy->CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
    if (m_pEnemy->GetRigidBody() != nullptr)
    {
        m_pEnemy->GetRigidBody()->SetActive(true);
    }

    Entity* textEntity = CreateWorldText("Init...", 20, "Hack-Regular", 255, 255, 0, 255);
    m_pPlayerDebugText = static_cast<WorldText*>(textEntity);
}

void TestScene::LoadLevel(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Erreur critique : Impossible de trouver le niveau " << filepath << std::endl;
        return;
    }

    json data;
    file >> data;

    auto levels = data["levels"];
    if (levels.empty()) return;

    // RAPPEL : Remplace 512 par la vraie taille en pixels de ton fichier "Assets.png"
    int16 textureLargeurTotale = 512;
    int16 textureHauteurTotale = 512;

    for (const auto& layer : levels[0]["layerInstances"])
    {
        std::string layerName = layer["__identifier"];
        float gridSize = layer["__gridSize"];

        // 1. Calque des Plateformes
        if (layerName == "Platformes")
        {
            for (const auto& tile : layer["gridTiles"])
            {
                float posX = tile["px"][0];
                float posY = tile["px"][1];

                int16 srcX = tile["src"][0];
                int16 srcY = tile["src"][1];

                Platform* p = CreateEntity<Platform>(gcle::Shapes::Rectangle);
                p->SetPosition(posX, posY);
                p->SetScale({ gridSize, gridSize });

                p->SetTexture("Assets");

                // CORRECTION : Utilisation de GetRenderShape()
                if (p->GetRenderShape() != nullptr)
                {
                    p->GetRenderShape()->SetTextureRect(srcX, srcY, static_cast<int16>(gridSize), static_cast<int16>(gridSize), textureLargeurTotale, textureHauteurTotale);
                }
            }
        }
        // 2. Calque du Décor
        else if (layerName == "Decor")
        {
            for (const auto& tile : layer["gridTiles"])
            {
                float posX = tile["px"][0];
                float posY = tile["px"][1];

                int16 srcX = tile["src"][0];
                int16 srcY = tile["src"][1];

                Entity* decor = CreateEntity<Entity>(gcle::Shapes::Rectangle);
                decor->SetPosition(posX, posY);
                decor->SetScale({ gridSize, gridSize });

                decor->SetTexture("Assets");

                // CORRECTION : Utilisation de GetRenderShape()
                if (decor->GetRenderShape() != nullptr)
                {
                    decor->GetRenderShape()->SetTextureRect(srcX, srcY, static_cast<int16>(gridSize), static_cast<int16>(gridSize), textureLargeurTotale, textureHauteurTotale);
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
        Vector2f playerPos = m_pPlayer->GetPosition();

        if (m_pCamera != nullptr)
        {
            m_pCamera->SetPosition({ playerPos.x + 250.0f, 240.0f });
        }

        if (m_pPlayerDebugText != nullptr && m_pPlayerDebugText->GetText() != nullptr)
        {
            m_pPlayerDebugText->SetRenderPosition({ playerPos.x, playerPos.y - 60.0f });
            std::string debugStr = "HP: " + std::to_string(m_pPlayer->GetHp()) + " | X: " + std::to_string(static_cast<int32>(playerPos.x));
            m_pPlayerDebugText->GetText()->SetText(debugStr);
        }
    }
}