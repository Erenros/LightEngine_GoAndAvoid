#include "PauseScene.h"
#include "Engine/SceneManager.h"

namespace
{
    constexpr const char* TEST_SCENE_TAG = "TestScene";
    constexpr const char* MAIN_MENU_SCENE_TAG = "MainMenu";
    constexpr float32 SCREEN_WIDTH = 1920.0f;
    constexpr float32 SCREEN_HEIGHT = 1080.0f;
    constexpr int32 PAUSE_FONT_SIZE = 140;
    constexpr int32 HINT_FONT_SIZE = 26;
    constexpr float32 HINT_SPACING = 40.0f;
}

void PauseScene::OnInitialize()
{
    Scene::OnInitialize();

    m_pCamera = CreateCamera();
    SwitchCamera(m_pCamera);

    AddDrawnTexture("PauseBg");

    m_pBackground = CreateUI<UI>(gcle::Shapes::Rectangle);
    m_pBackground->SetTexture("PauseBg");
    m_pBackground->SetScale({ 19.2f, 10.8f });
    m_pBackground->SetPosition(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

    m_pPauseText = CreateText("PAUSE", { 0.0f, 0.0f }, PAUSE_FONT_SIZE, 255, 255, 255);
    m_pPauseText->SetFont("street-fighter");
    m_pPauseText->SetText("PAUSE");

    Vector2f pauseSize = m_pPauseText->GetSizes();
    m_pPauseText->SetPosition(
        static_cast<int32>(SCREEN_WIDTH * 0.5f - pauseSize.x * 0.5f),
        static_cast<int32>(SCREEN_HEIGHT * 0.5f - pauseSize.y * 0.5f));
}

void PauseScene::OnUpdate(Clock& time)
{
    Scene::OnUpdate(time);

    InputManager& input = InputManager::GetInstance();

    if (input.IsDown('P'))
    {
        ReturnToGame();
        return;
    }

    if (input.IsDown(Escape) || input.IsDown(Backspace) || input.IsDown('M'))
    {
        ReturnToMainMenu();
        return;
    }
}

void PauseScene::ReturnToGame()
{
    SceneManager::GetInstance().SetCurrentSceneWithTag(TEST_SCENE_TAG, true);
}

void PauseScene::ReturnToMainMenu()
{
    SceneManager::GetInstance().DeleteScene(TEST_SCENE_TAG);
    SceneManager::GetInstance().SetCurrentSceneWithTag(MAIN_MENU_SCENE_TAG, false);
}