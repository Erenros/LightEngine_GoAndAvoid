#include "Character.h"

#include "Engine/GameManager.h"

namespace Demo
{
	void Character::OnUpdate()
	{
		if (m_damageDuration > 0.0f)
		{
			m_damageDuration -= ::GameManager::GetInstance().GetTime()->GetDeltaTime();
			SetColor(Color::Red);
		}
		else
		{
			SetColor(Color::White);
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

	int Character::GetCurrentLife(int currentLife) const
	{
		return m_CurrentLife;
	}

	void Character::Damage(int amount)
	{
		m_CurrentLife -= amount;
		m_damageDuration = m_baseDamageDuration;

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
}
