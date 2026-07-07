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

	};
}
