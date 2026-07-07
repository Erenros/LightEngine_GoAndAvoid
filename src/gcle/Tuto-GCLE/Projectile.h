#pragma once

#include "Engine/Entity.h"

namespace Demo
{
	class Projectile : public Entity
	{
	public:
		void OnUpdate()								override;
		void OnDestroy()							override;
		void OnInitialize()							override;
		void OnCollision(Entity* collidedWith)		override;
		void OnCollisionExit(Entity* collidedWith)	override;
		void OnCollisionEnter(Entity* collidedWith) override;

		void SetOwner(Entity* pEntity);
		Entity* GetOwner() const;

		void SetDamage(int damage);
		int GetDamage() const;

	private:
		Entity* mp_Owner = nullptr;

		int m_Damage = 10;
	};

}
