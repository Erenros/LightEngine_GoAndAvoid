#pragma once
#include "Engine/Scene.h"
#include "Render/Text.h"
#include <vector>
#include <string>

class MainMenu : public Scene
{
public:
    void OnInitialize() override;
    void OnUpdate(Clock& time) override;

private:
    struct ScorePanelLine
    {
        Text* pText = nullptr;
        Vector2f onScreenPosition;
    };

    void CreateMenuButtons();
    void CreateScorePanel();

    Button* CreateMenuButton(const std::string& label, Vector2f position, Vector2f scale);

    void UpdateButtonHoverScale(Button* pButton, bool& isHovered, Vector2f mousePosition, float32 dt);
    void UpdateButtonPressState(Vector2f mousePosition);
    Button* GetButtonUnderMouse(Vector2f mousePosition) const;

    void OnPlayClicked();
    void OnScoreClicked();
    void OnExitClicked();

    void OpenScorePanel();
    void CloseScorePanel();
    void RefreshScorePanel();
    void SetScorePanelVisible(bool visible);
    void SetMenuButtonsVisible(bool visible);

private:
    Camera* m_pCamera = nullptr;

    Button* m_pPlayButton = nullptr;
    Button* m_pScoreButton = nullptr;
    Button* m_pExitButton = nullptr;
    Button* m_pPressedButton = nullptr;

    Vector2f m_PlayButtonPosition = { 0.0f, 0.0f };
    Vector2f m_ScoreButtonPosition = { 0.0f, 0.0f };
    Vector2f m_ExitButtonPosition = { 0.0f, 0.0f };
    Vector2f m_ButtonBaseScale = { 1.0f, 1.0f };

    bool m_PlayHovered = false;
    bool m_ScoreHovered = false;
    bool m_ExitHovered = false;

    bool m_ShowScorePanel = false;
    bool m_WasLeftButtonHeld = false;

    UI* m_pScorePanelBackground = nullptr;
    Vector2f m_ScorePanelBackgroundOnPosition = { 0.0f, 0.0f };

    Text* m_pScoreTitleText = nullptr;
    Text* m_pBestScoreText = nullptr;
    Text* m_pBestDistanceText = nullptr;
    std::vector<Text*> m_HistoryTexts;
    std::vector<ScorePanelLine> m_ScorePanelLines;

    static constexpr int32 MAX_HISTORY_DISPLAY = 5;
    static constexpr float32 HOVER_SCALE_MULTIPLIER = 1.15f;
    static constexpr float32 HOVER_LERP_SPEED = 12.0f;
};