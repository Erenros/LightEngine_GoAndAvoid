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
		}

		if (pEntity->IsTag(GameTag::Player))
		{
			if (mp_Owner->IsTag(GameTag::Enemy))
			{
				GCPlayer* pPlayer = static_cast<GCPlayer*>(pEntity);
				pPlayer->Damage(m_Damage);
				Destroy();
			}
		}

		if (pEntity->IsTag(GameTag::Enemy))
		{
			if (mp_Owner->GetId() != pEntity->GetId())
			{
				GCEnemy* pEnemy = static_cast<GCEnemy*>(pEntity);
				pEnemy->Damage(m_Damage);
				Destroy();
			}
		}
	}

	void Projectile::SetOwner(Entity* pEntity)
	{
		mp_Owner = pEntity;
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
