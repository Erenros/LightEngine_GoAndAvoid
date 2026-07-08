#pragma once

#include "Engine/Entity.h"

namespace Demo
{
	class Character : public Entity
	{
	public:

		void OnUpdate() override;

		void SetMaxLife(int maxLife);
		void SetCurrentLife(int currentLife);

		int GetMaxLife(int maxLife) const;
		int GetCurrentLife() const;

		void Damage(int amount);
		virtual void Death();
		virtual void Heal(int amount);

		virtual void Shoot() {};

	private:
		int m_MaxLife = 20.0f;
		int m_CurrentLife = 20.0f;

		float m_baseDamageDuration = 1.0f;
		float m_damageDuration = 0.0f; 
		bool m_wasHit = false;
	};

}
