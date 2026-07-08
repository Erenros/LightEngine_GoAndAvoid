#include "Character.h"

#include "Engine/GameManager.h"

namespace Demo
{
	void Character::OnUpdate()
	{
		float32 dt = static_cast<float32>(::GameManager::GetInstance().GetTime()->GetDeltaTime());

		if (m_damageDuration > 0.0f)
		{
			m_damageDuration -= dt;
			m_wasHit = true;
		}
		else if (m_CurrentLife > 0 && m_wasHit)
		{
			PlayAnimation("Idle", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
			m_wasHit = false;
		}

		if (m_IsInvincible)
		{
			m_InvincibilityTimer -= dt;
			if (m_InvincibilityTimer <= 0.0f)
			{
				m_IsInvincible = false;
				m_InvincibilityTimer = 0.0f;
			}
		}
	}

	void Character::SetMaxLife(int maxLife)
	{
		m_MaxLife = maxLife;
	}

	void Character::SetCurrentLife(int currentLife)
	{
		m_CurrentLife = currentLife;
	}

	int Character::GetMaxLife(int maxLife) const
	{
		return m_MaxLife;
	}

	int Character::GetCurrentLife() const
	{
		return m_CurrentLife;
	}

	void Character::Damage(int amount)
	{
		if (m_IsInvincible)
			return;

		m_CurrentLife -= amount;
		m_damageDuration = m_baseDamageDuration;

		PlayAnimation("Hit", AnimationMode::Reverse);

		if (m_CurrentLife <= 0)
			Death();
	}

	void Character::Death()
	{
		m_CurrentLife = 0;
	}

	void Character::Heal(int amount)
	{
		m_CurrentLife += amount;

		if (m_CurrentLife > m_MaxLife)
			m_CurrentLife = m_MaxLife;
	}

	void Character::SetInvincible(bool isInvincible, float duration)
	{
		m_IsInvincible = isInvincible;
		m_InvincibilityTimer = duration;
	}

	bool Character::IsInvincible() const
	{
		return m_IsInvincible;
	}
}
