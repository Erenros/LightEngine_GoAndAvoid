#include "GCEnemy.h"

#include "Core/InputManager.h"
#include "Engine/GameManager.h"
#include "Engine/SceneManager.h"
#include "Core/Timer.h"

#include "Tuto-GCLE/Projectile.h"
#include "Tuto-GCLE/Tag.h"

namespace Demo
{
	void GCEnemy::OnUpdate()
	{
		Character::OnUpdate();

		InputManager& im = InputManager::GetInstance();

		/*if (im.IsDown(LeftButton))
		{
			Shoot();
		}*/
	}

	void GCEnemy::OnDestroy()
	{

	}

	void GCEnemy::OnInitialize()
	{
		SetTag(GameTag::Enemy);

		CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0, { 1.0f, 1.0f });

		SetRigidBody(true);
		GetRigidBody().SetGravity(false);

		SetPosition(100.0f, 100.0f);
	}

	void GCEnemy::OnCollision(Entity* collidedWith)
	{

	}

	void GCEnemy::OnCollisionExit(Entity* collidedWith)
	{

	}

	void GCEnemy::OnCollisionEnter(Entity* collidedWith)
	{

	}

	void GCEnemy::Death()
	{
		Character::Death();
		Destroy();
	}

	void GCEnemy::Heal(int amount)
	{
		Character::Heal(amount);
	}

	void GCEnemy::Shoot()
	{
		::Scene* pScene = ::SceneManager::GetInstance().GetCurrentScene();

		Vector2f posToGo = pScene->GetCurrentCamera()->GetMouseScreenToWorldPosition();

		Projectile* pProj = pScene->CreateEntity<Projectile>(gcle::Shapes::Circle);
		pProj->SetOwner(this);

		pProj->SetPosition(GetPosition().x, GetPosition().y);
		pProj->ScaleBy({ 0.3f, 0.3f });
		pProj->SetColor(Color::Red);

		pProj->SetSpeed(100.0f);

		pProj->GoToDirection(posToGo.x, posToGo.y);
	}
}