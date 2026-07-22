#pragma once
#include "KillableEntity.h"
#include "CharacterInput.h"
#include <string>

class Hitbox;

enum class State
{
    Run,
    Jump,
    Fall,
    Attack,
    Dead
};

class Player : public KillableEntity
{
private:
    CharacterInput m_input;
    FrameInputs m_currentInputs;

    Sprite* m_sprite = nullptr;
    State m_state = State::Run;
    std::string m_currentAnim;
    Hitbox* m_attackHitbox = nullptr;

    float32 m_actionTimer = 0.0f;
    float32 m_animDt = 0.1f;
    float32 m_jumpSpeed = 850.0f;

    void UpdateAnimation();
    void Play(const std::string& anim, AnimationMode animMode = AnimationMode::None);
    void StartAction(const std::string& anim, int32 nbFrames, State state, AnimationMode animMode = AnimationMode::None);
    void ReleasePendingAttack();

public:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnDeath() override;

    void SetupPlayer(InputMapping mapping, int8 padIndex);
    void SetCustomKeys(int16 left, int16 right, int16 crouch, int16 jump, int16 skill0, int16 skill1);
    void SetHitbox(Hitbox* hitbox);

    State GetState() const { return m_state; }
    std::string GetStateAsString() const;
};