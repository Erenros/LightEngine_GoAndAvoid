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


		if (im.IsHeld('Q')) {
			GetRigidBody().AddForce({ -1, 0 }, 1200, dt);
		}
		if (im.IsHeld('D')) {
			GetRigidBody().AddForce({ 1, 0 }, 1200, dt);
		}
		if (im.IsHeld('S')) {
			GetRigidBody().AddForce({ 0, 1 }, 1200, dt);
		}
		if (im.IsHeld('Z')) {
			GetRigidBody().AddForce({ 0, -1 }, 1200, dt);
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

		CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0, { 1.0f, 1.0f });

		SetRigidBody(true);
		GetRigidBody().SetGravity(false);
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