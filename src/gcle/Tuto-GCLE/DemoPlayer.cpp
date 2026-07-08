#include "DemoPlayer.h"

#include "Core/InputManager.h"
#include "Engine/GameManager.h"
#include "Engine/SceneManager.h"
#include "Core/Timer.h"

#include "Tuto-GCLE/Projectile.h"
#include "Tuto-GCLE/Tag.h"

namespace Demo
{
	void GCPlayer::OnUpdate()
	{
		Character::OnUpdate();

		InputManager& im = InputManager::GetInstance(); 
		float32 dt = static_cast<float32>(::GameManager::GetInstance().GetTime()->GetDeltaTime());


		if (im.IsHeld('Q')) 
		{
			GetRigidBody().AddForce({ -1, 0 }, 600, dt); 

			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);

		}
		if (im.IsHeld('D'))
		{
			GetRigidBody().AddForce({ 1, 0 }, 600, dt);

			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}
		if (im.IsHeld('S'))
		{
			GetRigidBody().AddForce({ 0, 1 }, 600, dt);

			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}
		if (im.IsHeld('Z'))
		{
			GetRigidBody().AddForce({ 0, -1 }, 600, dt);

			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}

		if (GetRigidBody().GetVelocity().x <= 0.05f && GetRigidBody().GetVelocity().y <= 0.05f &&
			GetRigidBody().GetVelocity().x >= -0.05f && GetRigidBody().GetVelocity().y >= -0.05f)
		{
			PlayAnimation("Idle", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}

		if (im.IsDown(LeftButton))
		{
			Shoot();
		}
	}

	void GCPlayer::OnDestroy()
	{
	}

	void GCPlayer::OnInitialize()
	{
		SetTag(GameTag::Player);

		CreateCollider(gcle::Shapes::Rectangle, true, { -3.0f, 0.0f }, 0, { 0.3f, 0.45f });
		

		SetRigidBody(true);
		GetRigidBody().SetGravity(false);
		GetRigidBody().ActivateDamping(true);
		GetRigidBody().SetDampingStrenght(0.99f);
		GetRigidBody().SetCollisionOnContinuous();

		SetTexture("player");

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

		PlayAnimation("Appear", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
	}

	void GCPlayer::OnCollision(Entity* collidedWith)
	{
	}

	void GCPlayer::OnCollisionExit(Entity* collidedWith)
	{
	}

	void GCPlayer::OnCollisionEnter(Entity* collidedWith)
	{
	}

	void GCPlayer::Death()
	{
		Character::Death();

		Destroy();
	}

	void GCPlayer::Heal(int amount)
	{
		Character::Heal(amount);
	}

	void GCPlayer::Shoot()
	{ 
		::Scene* pScene = ::SceneManager::GetInstance().GetCurrentScene();

		Vector2f posToGo = pScene->GetCurrentCamera()->GetMouseScreenToWorldPosition();

		Projectile* pProj = pScene->CreateEntity<Projectile>(gcle::Shapes::Circle);
		pProj->SetOwner(this);

		pProj->SetPosition(GetPosition().x, GetPosition().y);
		pProj->ScaleBy({ 0.1f, 0.1f });
		pProj->SetColor(Color::Yellow);

		pProj->SetSpeed(100.0f);

		pProj->GoToDirection(posToGo.x, posToGo.y);
	}
}