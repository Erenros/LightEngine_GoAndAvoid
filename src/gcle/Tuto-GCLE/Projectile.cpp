#include "Projectile.h"

#include "Tuto-GCLE/Tag.h"
#include "Tuto-GCLE/DemoPlayer.h"
#include "Tuto-GCLE/GCEnemy.h"

namespace Demo
{
	void Projectile::OnUpdate()
	{
	}

	void Projectile::OnDestroy()
	{
	}

	void Projectile::OnInitialize()
	{
		SetRigidBody(true);
		CreateCollider(gcle::Shapes::Circle, true, { 0.0f, 0.0f }, 0, { 1.0f, 1.0f });
	}

	void Projectile::OnCollision(Entity* collidedWith)
	{
	}

	void Projectile::OnCollisionExit(Entity* collidedWith)
	{
	}

	void Projectile::OnCollisionEnter(Entity* pEntity)
	{
		if (pEntity->IsTag(GameTag::Wall))
		{
			Destroy();
			return;
		}

		if (pEntity->IsTag(GameTag::Player) && m_OwnerTag == GameTag::Enemy)
		{
			static_cast<GCPlayer*>(pEntity)->Damage(m_Damage);
			Destroy();
		}

		if (pEntity->IsTag(GameTag::Enemy) && m_OwnerId != pEntity->GetId())
		{
			static_cast<GCEnemy*>(pEntity)->Damage(m_Damage);
			Destroy();
		}
	}

	void Projectile::SetOwner(Entity* pEntity)
	{
		mp_Owner = pEntity;
		if (pEntity != nullptr)
		{
			m_OwnerTag = pEntity->IsTag(GameTag::Enemy) ? GameTag::Enemy : pEntity->IsTag(GameTag::Player) ? GameTag::Player : -1;
			m_OwnerId = pEntity->GetId();
		}
	}

	Entity* Projectile::GetOwner() const
	{
		return mp_Owner;
	}

	void Projectile::SetDamage(int damage)
	{
		m_Damage = damage;
	}

	int Projectile::GetDamage() const
	{
		return m_Damage;
	}
}
