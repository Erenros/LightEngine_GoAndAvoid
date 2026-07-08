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

		CreateCollider(gcle::Shapes::Rectangle, true, { -3.0f, 0.0f }, 0, { 0.3f, 0.45f });


		SetRigidBody(true);
		GetRigidBody().SetGravity(false);
		GetRigidBody().ActivateDamping(true);
		GetRigidBody().SetDampingStrenght(0.99f);
		GetRigidBody().SetCollisionOnContinuous();

		SetTexture("enemy");

		AddAnimation("Idle", 0, 3, 0, 64, 64, 0.5f);
		AddAnimation("Walk", 0, 5, 3, 64, 64, 0.2f);
		AddAnimation("Hit", 0, 3, 5, 64, 64, 0.2f);
		AddAnimation("Death", 0, 10, 6, 64, 64, 0.25f);
		AddAnimation("Appear", 0, 11, 9, 64, 64, 0.25f);
		AddAnimation("Teleport", 0, 11, 21, 64, 64, 0.25f);

		AddFunctionInFrame("Appear", 11, [this]()
			{
				PlayAnimation("Idle", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
			}); 
		
		AddFunctionInFrame("Hit", 0, [this]()
			{
				if (this->GetCurrentLife() <= 0)
				{
					PlayAnimation("Death");
				}
			});
		
		AddFunctionInFrame("Death", 10, [this]()
			{
				Destroy();
			});

		PlayAnimation("Appear", AnimationMode::IgnoreIfAlreadyPlaying);

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