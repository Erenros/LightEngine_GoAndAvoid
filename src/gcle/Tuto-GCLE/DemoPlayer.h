#pragma once

#include "Tuto-GCLE/Character.h"

namespace Demo
{
	class GCPlayer : public Character
	{
	public:

		void OnUpdate()								override;
		void OnDestroy()							override;
		void OnInitialize()							override;
		void OnCollision(Entity* collidedWith)		override;
		void OnCollisionExit(Entity* collidedWith)	override;
		void OnCollisionEnter(Entity* collidedWith) override;

		void Death()								override;
		void Heal(int amount)						override;
		void Shoot()								override;
		void Damage(int amount)						override;

	private:
		void UpdateDodgeRoll(float32 dt);


	private:
		bool	m_CanMove = true;
		bool	m_IsDodging = false;
		float32 m_DodgeDuration = 0.25f;
		float32 m_DodgeTimer = 0.0f;

		float32 m_DodgeCooldown = 0.6f;
		float32 m_DodgeCooldownTimer = 0.0f;

		float32 m_DodgeForce = 15000.0f;

		Vector2f m_LastMoveDirection = { 0.0f, -1.0f };
	}; 
}
