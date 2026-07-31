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

    void UpdateMenuNavigation(Vector2f mousePosition);
    void UpdateButtonHoverScale(Button* pButton, bool isSelected, float32 dt);
    void UpdateButtonPressState(Vector2f mousePosition);
    void UpdateSelectionConfirm();
    Button* GetButtonUnderMouse(Vector2f mousePosition) const;
    Button* GetMenuButtonAt(int32 index) const;
    int32 GetMenuButtonIndex(Button* pButton) const;
    Button* GetSelectedButton() const;

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
    UI* m_pBackground = nullptr;

    Button* m_pPlayButton = nullptr;
    Button* m_pScoreButton = nullptr;
    Button* m_pExitButton = nullptr;
    Button* m_pPressedButton = nullptr;

    Vector2f m_PlayButtonPosition = { -1000.0f, 0.0f };
    Vector2f m_ScoreButtonPosition = { -1000.0f, 0.0f };
    Vector2f m_ExitButtonPosition = { -1000.0f, 0.0f };
    Vector2f m_ButtonBaseScale = { 1.0f, 1.0f };

    int32 m_SelectedMenuIndex = 0;

    bool m_ShowScorePanel = false;
    bool m_WasLeftButtonHeld = false;
    bool m_WasControllerConfirmHeld = false;

    UI* m_pScorePanelBackground = nullptr;
    Vector2f m_ScorePanelBackgroundOnPosition = { 0.0f, 0.0f };

    Text* m_pScoreTitleText = nullptr;
    Text* m_pBestScoreText = nullptr;
    Text* m_pBestDistanceText = nullptr;
    std::vector<Text*> m_HistoryTexts;
    std::vector<ScorePanelLine> m_ScorePanelLines;

    static constexpr int32 MAX_HISTORY_DISPLAY = 5;
    static constexpr int32 MENU_BUTTON_COUNT = 3;
    static constexpr float32 HOVER_SCALE_MULTIPLIER = 1.15f;
    static constexpr float32 HOVER_LERP_SPEED = 12.0f;
};