#pragma once
#include "Engine/Scene.h"
#include "Render/Text.h"

class PauseScene : public Scene
{
public:
    void OnInitialize() override;
    void OnUpdate(Clock& time) override;

private:
    void ReturnToGame();
    void ReturnToMainMenu();

    Camera* m_pCamera = nullptr;
    Text* m_pPauseText = nullptr;
    Text* m_pHintText = nullptr;

    UI* m_pBackground = nullptr;
};
