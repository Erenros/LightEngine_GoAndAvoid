#include "MainMenu.h"
#include "Engine/SceneManager.h"
#include "ScoreManager.h"
#include <algorithm>
#include <cstdlib>

namespace
{
    constexpr const char* TEST_SCENE_TAG = "TestScene";
    constexpr const char* BUTTON_TEXTURE_ID = "Button";
    constexpr const char* BUTTON_PRESSED_TEXTURE_ID = "PressedButton";
    constexpr float32 SCREEN_WIDTH = 1920.0f;
    constexpr float32 SCREEN_HEIGHT = 1080.0f;
    constexpr float32 OFF_SCREEN_POSITION = -6000.0f;
    constexpr int32 MENU_BUTTON_FONT_SIZE = 28;
}

void MainMenu::OnInitialize()
{
    Scene::OnInitialize();

    m_pCamera = CreateCamera();
    SwitchCamera(m_pCamera);

    AddDrawnTexture("MenuBg");

    m_pBackground = CreateUI<UI>(gcle::Shapes::Rectangle);
    m_pBackground->SetTexture("MenuBg");
    m_pBackground->SetScale({ 19.2f, 10.8f });
    m_pBackground->SetPosition(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

    CreateMenuButtons();
    CreateScorePanel();
}

void MainMenu::OnUpdate(Clock& time)
{
    Scene::OnUpdate(time);

    if (m_pCamera == nullptr)
        return;

    Vector2f mousePosition = m_pCamera->GetScreenMousePosition();

    if (m_ShowScorePanel)
    {
        if (InputManager::GetInstance().IsDown(Escape))
            CloseScorePanel();

        return;
    }

    float32 dt = static_cast<float32>(time.GetDeltaTime());

    UpdateMenuNavigation(mousePosition);

    UpdateButtonHoverScale(m_pPlayButton, m_SelectedMenuIndex == 0, dt);
    UpdateButtonHoverScale(m_pScoreButton, m_SelectedMenuIndex == 1, dt);
    UpdateButtonHoverScale(m_pExitButton, m_SelectedMenuIndex == 2, dt);

    UpdateButtonPressState(mousePosition);
    UpdateSelectionConfirm();
}

Button* MainMenu::CreateMenuButton(const std::string& label, Vector2f position, Vector2f scale)
{
    Button* pButton = CreateUI<Button>(gcle::Shapes::Rectangle);

    pButton->SetTexture(BUTTON_TEXTURE_ID);

    Text* pButtonText = CreateText(label, { 0.0f, 0.0f }, MENU_BUTTON_FONT_SIZE, 255, 255, 255);
    pButtonText->SetFont("street-fighter");
    pButtonText->SetText(label);

    pButton->SetTextObject(pButtonText);

    pButton->SetScale(scale);
    pButton->SetPosition(position.x, position.y);

    return pButton;
}

void MainMenu::CreateMenuButtons()
{
    constexpr float32 BUTTON_WIDTH = 320.0f;
    constexpr float32 BUTTON_HEIGHT = 100.0f;
    constexpr float32 BUTTON_SPACING = 50.0f;

    m_ButtonBaseScale = { BUTTON_WIDTH / 100.0f, BUTTON_HEIGHT / 100.0f };

    m_PlayButtonPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - (BUTTON_HEIGHT + BUTTON_SPACING) };
    m_ScoreButtonPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
    m_ExitButtonPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + (BUTTON_HEIGHT + BUTTON_SPACING) };

    m_pPlayButton = CreateMenuButton("JOUER", m_PlayButtonPosition, m_ButtonBaseScale);
    m_pScoreButton = CreateMenuButton("SCORES", m_ScoreButtonPosition, m_ButtonBaseScale);
    m_pExitButton = CreateMenuButton("QUITTER", m_ExitButtonPosition, m_ButtonBaseScale);

    m_pPlayButton->AddFunction([this]() { OnPlayClicked(); });
    m_pScoreButton->AddFunction([this]() { OnScoreClicked(); });
    m_pExitButton->AddFunction([this]() { OnExitClicked(); });
}

void MainMenu::CreateScorePanel()
{
    constexpr float32 PANEL_WIDTH = 760.0f;
    constexpr float32 PANEL_HEIGHT = 640.0f;
    constexpr float32 LINE_HEIGHT = 46.0f;
    constexpr int32 FONT_SIZE = 26;

    m_ScorePanelBackgroundOnPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

    m_pScorePanelBackground = CreateUI<UI>(gcle::Shapes::Rectangle);
    m_pScorePanelBackground->SetScale({ PANEL_WIDTH / 100.0f, PANEL_HEIGHT / 100.0f });
    m_pScorePanelBackground->SetColor({ 15, 15, 25, 235 });
    m_pScorePanelBackground->SetPosition(m_ScorePanelBackgroundOnPosition.x, m_ScorePanelBackgroundOnPosition.y);

    float32 textX = SCREEN_WIDTH * 0.5f - PANEL_WIDTH * 0.5f + 40.0f;
    float32 currentY = SCREEN_HEIGHT * 0.5f - PANEL_HEIGHT * 0.5f + 40.0f;

    auto addLine = [&](const std::string& initialText) -> Text*
        {
            Vector2f onScreenPos = { textX, currentY };
            Text* pText = CreateText(initialText, onScreenPos, FONT_SIZE, 255, 255, 255);
            pText->SetFont("street-fighter");
            m_ScorePanelLines.push_back({ pText, onScreenPos });
            currentY += LINE_HEIGHT;
            return pText;
        };

    m_pScoreTitleText = addLine("MEILLEURS SCORES");
    currentY += LINE_HEIGHT * 0.5f;

    m_pBestScoreText = addLine("Meilleur score : 0");
    m_pBestDistanceText = addLine("Meilleure distance : 0m");
    currentY += LINE_HEIGHT * 0.5f;

    m_HistoryTexts.resize(MAX_HISTORY_DISPLAY);
    for (int32 i = 0; i < MAX_HISTORY_DISPLAY; i++)
        m_HistoryTexts[i] = addLine("");

    currentY += LINE_HEIGHT * 0.5f;
    addLine("ECHAP TO RETURN");

    SetScorePanelVisible(false);
}

void MainMenu::UpdateMenuNavigation(Vector2f mousePosition)
{
    InputManager& input = InputManager::GetInstance();

    Button* pButtonUnderMouse = GetButtonUnderMouse(mousePosition);
    if (pButtonUnderMouse != nullptr)
    {
        m_SelectedMenuIndex = GetMenuButtonIndex(pButtonUnderMouse);
    }

    if (input.IsDown(UpArrow))
    {
        m_SelectedMenuIndex = (m_SelectedMenuIndex + MENU_BUTTON_COUNT - 1) % MENU_BUTTON_COUNT;
    }
    else if (input.IsDown(DownArrow))
    {
        m_SelectedMenuIndex = (m_SelectedMenuIndex + 1) % MENU_BUTTON_COUNT;
    }
}

void MainMenu::UpdateButtonHoverScale(Button* pButton, bool isSelected, float32 dt)
{
    if (pButton == nullptr)
        return;

    Vector2f targetScale = isSelected
        ? Vector2f{ m_ButtonBaseScale.x * HOVER_SCALE_MULTIPLIER, m_ButtonBaseScale.y * HOVER_SCALE_MULTIPLIER }
    : m_ButtonBaseScale;

    Vector2f currentScale = pButton->GetScale();
    float32 t = std::clamp(HOVER_LERP_SPEED * dt, 0.0f, 1.0f);

    Vector2f newScale
    {
        currentScale.x + (targetScale.x - currentScale.x) * t,
        currentScale.y + (targetScale.y - currentScale.y) * t
    };

    pButton->SetScale(newScale);
}

void MainMenu::UpdateButtonPressState(Vector2f mousePosition)
{
    InputManager& input = InputManager::GetInstance();
    bool isLeftButtonHeld = input.IsHeld(LeftButton);

    if (m_pPressedButton == nullptr)
    {
        if (isLeftButtonHeld && !m_WasLeftButtonHeld)
        {
            m_pPressedButton = GetButtonUnderMouse(mousePosition);
            if (m_pPressedButton != nullptr)
                m_pPressedButton->SetTexture(BUTTON_PRESSED_TEXTURE_ID);
        }
    }
    else
    {
        if (!isLeftButtonHeld)
        {
            bool validated = m_pPressedButton->IsInside(mousePosition);
            m_pPressedButton->SetTexture(BUTTON_TEXTURE_ID);

            if (validated)
                m_pPressedButton->Click();

            m_pPressedButton = nullptr;
        }
    }

    m_WasLeftButtonHeld = isLeftButtonHeld;
}

void MainMenu::UpdateSelectionConfirm()
{
    InputManager& input = InputManager::GetInstance();

    bool keyboardConfirmPressed = input.IsDown(Space);

    bool isControllerConfirmHeld = input.IsControllerDown(0, XBOX_A);
    bool controllerConfirmPressed = isControllerConfirmHeld && !m_WasControllerConfirmHeld;
    m_WasControllerConfirmHeld = isControllerConfirmHeld;

    if (!keyboardConfirmPressed && !controllerConfirmPressed)
        return;

    Button* pSelectedButton = GetSelectedButton();
    if (pSelectedButton != nullptr)
        pSelectedButton->Click();
}

Button* MainMenu::GetButtonUnderMouse(Vector2f mousePosition) const
{
    if (m_pPlayButton != nullptr && m_pPlayButton->IsInside(mousePosition))
        return m_pPlayButton;

    if (m_pScoreButton != nullptr && m_pScoreButton->IsInside(mousePosition))
        return m_pScoreButton;

    if (m_pExitButton != nullptr && m_pExitButton->IsInside(mousePosition))
        return m_pExitButton;

    return nullptr;
}

Button* MainMenu::GetMenuButtonAt(int32 index) const
{
    switch (index)
    {
    case 0: return m_pPlayButton;
    case 1: return m_pScoreButton;
    case 2: return m_pExitButton;
    default: return nullptr;
    }
}

int32 MainMenu::GetMenuButtonIndex(Button* pButton) const
{
    if (pButton == m_pPlayButton) return 0;
    if (pButton == m_pScoreButton) return 1;
    if (pButton == m_pExitButton) return 2;
    return -1;
}

Button* MainMenu::GetSelectedButton() const
{
    return GetMenuButtonAt(m_SelectedMenuIndex);
}

void MainMenu::OnPlayClicked()
{
    SceneManager::GetInstance().SetCurrentSceneWithTag(TEST_SCENE_TAG, true);
}

void MainMenu::OnScoreClicked()
{
    if (m_ShowScorePanel)
        CloseScorePanel();
    else
        OpenScorePanel();
}

void MainMenu::OnExitClicked()
{
    std::exit(0);
}

void MainMenu::OpenScorePanel()
{
    RefreshScorePanel();
    SetScorePanelVisible(true);
    SetMenuButtonsVisible(false);
    m_ShowScorePanel = true;
}

void MainMenu::CloseScorePanel()
{
    SetScorePanelVisible(false);
    SetMenuButtonsVisible(true);
    m_ShowScorePanel = false;
}

void MainMenu::RefreshScorePanel()
{
    ScoreManager& scoreManager = ScoreManager::GetInstance();

    if (m_pBestScoreText != nullptr)
        m_pBestScoreText->SetText("Meilleur score : " + std::to_string(scoreManager.GetBestScore()));

    if (m_pBestDistanceText != nullptr)
        m_pBestDistanceText->SetText("Meilleure distance : " + std::to_string(scoreManager.GetBestDistance()) + "m");

    const std::vector<RunResult>& history = scoreManager.GetRunHistory();
    int32 runCount = static_cast<int32>(history.size());

    for (int32 i = 0; i < static_cast<int32>(m_HistoryTexts.size()); i++)
    {
        if (m_HistoryTexts[i] == nullptr)
            continue;

        int32 runIndex = runCount - 1 - i;

        if (runIndex >= 0)
        {
            const RunResult& run = history[runIndex];
            m_HistoryTexts[i]->SetText("Partie " + std::to_string(runIndex + 1) + " : Score " + std::to_string(run.score) + " - Distance " + std::to_string(run.distance) + "m");
        }
        else
        {
            m_HistoryTexts[i]->SetText("");
        }
    }
}

void MainMenu::SetScorePanelVisible(bool visible)
{
    if (m_pScorePanelBackground != nullptr)
    {
        Vector2f pos = visible ? m_ScorePanelBackgroundOnPosition : Vector2f{ OFF_SCREEN_POSITION, OFF_SCREEN_POSITION };
        m_pScorePanelBackground->SetPosition(pos.x, pos.y);
    }

    for (ScorePanelLine& line : m_ScorePanelLines)
    {
        if (line.pText == nullptr)
            continue;

        Vector2f pos = visible ? line.onScreenPosition : Vector2f{ OFF_SCREEN_POSITION, OFF_SCREEN_POSITION };
        line.pText->SetPosition(static_cast<int32>(pos.x), static_cast<int32>(pos.y));
    }
}

void MainMenu::SetMenuButtonsVisible(bool visible)
{
    if (m_pPlayButton != nullptr)
    {
        Vector2f pos = visible ? m_PlayButtonPosition : Vector2f{ OFF_SCREEN_POSITION, OFF_SCREEN_POSITION };
        m_pPlayButton->SetPosition(pos.x, pos.y);
    }

    if (m_pScoreButton != nullptr)
    {
        Vector2f pos = visible ? m_ScoreButtonPosition : Vector2f{ OFF_SCREEN_POSITION, OFF_SCREEN_POSITION };
        m_pScoreButton->SetPosition(pos.x, pos.y);
    }

    if (m_pExitButton != nullptr)
    {
        Vector2f pos = visible ? m_ExitButtonPosition : Vector2f{ OFF_SCREEN_POSITION, OFF_SCREEN_POSITION };
        m_pExitButton->SetPosition(pos.x, pos.y);
    }
}