#include "Player.h"
#include "GameManager.h"
#include "Hitbox.h"
#include "ScoreManager.h"
#include "SceneManager.h"
#include "Collider.h"
#include "Platform.h"
#include <algorithm>

#undef min
#undef max

namespace
{
    constexpr int32 PLATFORM_TAG = 1;
    constexpr const char* MAIN_MENU_SCENE_TAG = "MainMenu";
}

void Player::OnInitialize()
{
    KillableEntity::OnInitialize();

    RigidBody2D* rb = GetRigidBody();
    if (rb != nullptr)
    {
        rb->SetFriction({ 0.0f, 0.0f });
    }

    m_input.SetMapping(InputMapping::Keyboard, 0);
    m_hp = 1;
    m_maxHp = 1;

    SetTexture("male_hero2");
    Window* window = GameManager::GetInstance().GetWindow();
    m_sprite = new Sprite(window, "../../assets/textures/male_hero2.png");

    m_sprite->AddAnimation("Idle", 0, 9, 0, 32, 50, m_animDt);
    m_sprite->AddAnimation("Walk", 0, 9, 1, 35, 50, m_animDt);
    m_sprite->AddAnimation("Run", 0, 9, 2, 40, 50, m_animDt);
    m_sprite->AddAnimation("Jump", 0, 5, 3, 31, 50, m_animDt);
    m_sprite->AddAnimation("Fall", 0, 2, 4, 29, 50, m_animDt);
    m_sprite->AddAnimation("Attack", 0, 2, 5, 61, 50, m_animDt);
    m_sprite->AddAnimation("AttackEnd", 0, 3, 6, 46, 50, m_animDt);

    m_sprite->AddFunctionInFrame("Attack", 2, [this]() { ReleasePendingAttack(); });

    m_state = State::Run;
    Play("Run", AnimationMode::Loop);
}

void Player::SetupPlayer(InputMapping mapping, int8 padIndex)
{
    m_input.SetMapping(mapping, padIndex);
}

void Player::SetCustomKeys(int16 left, int16 right, int16 crouch, int16 jump, int16 skill0, int16 skill1)
{
    m_input.m_keyLeft = left;
    m_input.m_keyRight = right;
    m_input.m_keyCrouch = crouch;
    m_input.m_keyJump = jump;
    m_input.m_keySkill0 = skill0;
    m_input.m_keySkill1 = skill1;
}

void Player::SetHitbox(Hitbox* hitbox)
{
    m_attackHitbox = hitbox;
}

void Player::Play(const std::string& anim, AnimationMode animMode)
{
    if (anim == m_currentAnim)
    {
        return;
    }
    m_sprite->PlayAnimation(anim, animMode, AnimationInterrupt::Force);
    m_currentAnim = anim;
}

void Player::StartAction(const std::string& anim, int32 nbFrames, State state, AnimationMode animMode)
{
    Play(anim, animMode);
    m_currentAnim = anim;
    m_actionTimer = nbFrames * m_animDt;
    m_state = state;
}

void Player::ReleasePendingAttack()
{
    m_actionTimer = 0.0f;
    m_attackCooldownTimer = 0.2f;
    m_state = State::Run;
}

void Player::UpdateAnimation()
{
    if (m_state == State::Dead || m_actionTimer > 0.0f)
    {
        return;
    }

    switch (m_state)
    {
    case State::Run:
        Play("Run", AnimationMode::Loop);
        break;
    case State::Jump:
        Play("Jump", AnimationMode::None);
        break;
    case State::Fall:
        Play("Fall", AnimationMode::Loop);
        break;
    case State::Attack:
        break;
    }
}

void Player::OnUpdate()
{
    float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());

    m_currentInputs = m_input.Read();

    if (m_attackCooldownTimer > 0.0f)
    {
        m_attackCooldownTimer -= dt;
    }

    m_velX = 400.0f;

    if (m_actionTimer > 0.0f)
    {
        m_actionTimer -= dt;
    }

    if (!m_groundContacts.empty() && m_currentInputs.jump)
    {
        m_velY = -m_jumpSpeed;
        m_groundContacts.clear();

        if (m_actionTimer <= 0.0f)
        {
            m_state = State::Jump;
        }
    }
    else if (m_actionTimer <= 0.0f)
    {
        if (m_currentInputs.skill0 && m_attackCooldownTimer <= 0.0f)
        {
            StartAction("Attack", 3, State::Attack, AnimationMode::Lock);

            if (m_attackHitbox != nullptr)
            {
                m_attackHitbox->Activate({ 50.0f, 0.0f }, { 1.0f, 1.0f }, m_dmg, 0.2f);
            }
        }
        else if (m_groundContacts.empty() && m_velY < 0.0f)
        {
            m_state = State::Jump;
        }
        else if (m_groundContacts.empty() && m_velY > 0.0f)
        {
            m_state = State::Fall;
        }
        else if (!m_groundContacts.empty())
        {
            m_state = State::Run;
        }
    }

    KillableEntity::OnUpdate();
    UpdateAnimation();

    ScoreManager::GetInstance().UpdateDistanceFromPosition(GetPosition());

    if (m_sprite != nullptr && GetRenderShape() != nullptr)
    {
        m_sprite->UpdateAnimation(dt, GetRenderShape());
    }
}

void Player::OnDeath()
{
    if (m_state == State::Dead)
    {
        return;
    }

    m_state = State::Dead;

    ScoreManager::GetInstance().RegisterRunEnd();

    if (m_sprite != nullptr)
    {
        delete m_sprite;
        m_sprite = nullptr;
    }
    KillableEntity::OnDeath();

    SceneManager::GetInstance().SetCurrentSceneWithTag(MAIN_MENU_SCENE_TAG, false);
}

std::string Player::GetStateAsString() const
{
    switch (m_state)
    {
    case State::Run: return "Run";
    case State::Jump: return "Jump";
    case State::Fall: return "Fall";
    case State::Attack: return "Attack";
    case State::Dead: return "Dead";
    default: return "Unknown";
    }
}

bool Player::HasCollidedOnRightSide(Entity* other)
{
    bool hitOnRight = false;

    for (Collider* collider : GetColliders())
    {
        if (collider == nullptr)
        {
            continue;
        }

        if (collider->GetCollisionDirection().isCollidingOnRight)
        {
            hitOnRight = true;
        }

        collider->StoppedColliding();
    }

    if (!hitOnRight || other == nullptr)
    {
        return false;
    }

    gcle::Shape* pSelfShape = GetRenderShape();
    gcle::Shape* pOtherShape = other->GetRenderShape();

    if (pSelfShape == nullptr || pOtherShape == nullptr)
    {
        return hitOnRight;
    }

    float32 tolerance = pSelfShape->GetHeight() * 0.25f;

    float32 selfBottom = GetPosition().y + pSelfShape->GetHeight() * 0.5f;
    float32 otherTop = other->GetPosition().y - pOtherShape->GetHeight() * 0.5f;

    return selfBottom > otherTop + tolerance;
}

void Player::OnCollisionEnter(Entity* other)
{
    KillableEntity::OnCollisionEnter(other);

    if (other == nullptr || m_state == State::Dead || !other->IsTag(PLATFORM_TAG))
    {
        return;
    }

    bool hitOnRight = HasCollidedOnRightSide(other);

    Platform* platform = dynamic_cast<Platform*>(other);
    bool isSpikes = (platform != nullptr && platform->IsSpikes());

    if (isSpikes || hitOnRight)
    {
        TakeDamage(m_hp);
    }
}