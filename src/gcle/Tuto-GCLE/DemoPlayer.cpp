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

		UpdateDodgeRoll(dt);

		if (m_IsDodging)
			return;

		if (im.IsHeld('Q') && m_CanMove)
		{
			GetRigidBody().AddForce({ -1, 0 }, 1200, dt);
			SetTextureFlip(TextureFlipMode::FLIP_HORIZONTAL);
			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}
		if (im.IsHeld('D') && m_CanMove)
		{
			GetRigidBody().AddForce({ 1, 0 }, 1200, dt); 
			SetTextureFlip(TextureFlipMode::FLIP_NONE);
			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}
		if (im.IsHeld('S') && m_CanMove)
		{
			GetRigidBody().AddForce({ 0, 1 }, 1200, dt); 
			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}
		if (im.IsHeld('Z') && m_CanMove)
		{
			GetRigidBody().AddForce({ 0, -1 }, 1200, dt); 
			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}

		m_LastMoveDirection = GetRigidBody().GetVelocity();

		if (
			GetRigidBody().GetVelocity().x <= 0.05f && GetRigidBody().GetVelocity().y <= 0.05f &&
			GetRigidBody().GetVelocity().x >= -0.05f && GetRigidBody().GetVelocity().y >= -0.05f && 
			m_CanMove
		){
			PlayAnimation("Idle", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}

		if (im.IsDown(LeftButton))
		{
			Shoot();
		}

		if (im.IsDown(Space) && m_DodgeCooldownTimer <= 0.0f && m_CanMove)
		{
			m_IsDodging = true;
			m_DodgeTimer = m_DodgeDuration;
			m_DodgeCooldownTimer = m_DodgeCooldown;
			 
			SetInvincible(true, m_DodgeDuration);
			SetColor({ 255, 255, 255, 50 }); 

			GetRigidBody().Stop();
			GetRigidBody().AddImpulse(m_LastMoveDirection, m_DodgeForce);

			PlayAnimation("Walk", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
		}

		if (im.IsDown('F') && mp_InteractableObject.size() != 0)
		{
			mp_InteractableObject[0]->Interact(this);
		}

	}

	void GCPlayer::UpdateDodgeRoll(float32 dt)
	{
		if (m_DodgeCooldownTimer > 0.0f)
			m_DodgeCooldownTimer -= dt;

		if (!m_IsDodging)
			return;

		m_DodgeTimer -= dt;

		if (m_DodgeTimer <= 0.0f)
		{
			m_IsDodging = false;
			SetColor(Color::White);
		}
	}

	void GCPlayer::OnDestroy() {}

	void GCPlayer::OnInitialize()
	{
		SetTag(GameTag::Player);

		CreateCollider(gcle::Shapes::Rectangle, true, { -3.0f, 0.0f }, 0, { 0.3f, 0.45f });

		mp_InteractableRange = CreateCollider(gcle::Shapes::Circle, true, { 0.0f, 0.0f }, 0, { 0.6f, 0.6f }, true);

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
		AddAnimation("Appear", 0, 11, 9, 64, 64, 0.095f);
		AddAnimation("Teleport", 0, 11, 21, 64, 64, 0.25f);

		AddFunctionInFrame("Appear", 11, [this]()
			{
				m_CanMove = true;
				PlayAnimation("Idle", AnimationMode::Loop | AnimationMode::IgnoreIfAlreadyPlaying);
				m_CanShoot = true;
			});

		AddFunctionInFrame("Hit", 0, [this]()
			{				
				if (this->GetCurrentLife() <= 0)
				{
					m_CanMove = false;
					PlayAnimation("Death", AnimationMode::Lock, AnimationInterrupt::Force);
					m_CanShoot = false;
				}
			});

		AddFunctionInFrame("Death", 10, [this]()
			{
				Destroy();
			});

		PlayAnimation("Appear", AnimationMode::IgnoreIfAlreadyPlaying | AnimationMode::Lock, AnimationInterrupt::Force);
	}

	void GCPlayer::OnCollision(Entity* collidedWith) {}
	void GCPlayer::OnCollisionExit(Entity* collidedWith) {}
	void GCPlayer::OnCollisionEnter(Entity* collidedWith) {}

	void GCPlayer::OnTrigger(Entity* collidedWith)
	{
	}

	void GCPlayer::OnTriggerExit(Entity* collidedWith)
	{
		bool toClear = false;
		for (auto it = mp_InteractableObject.begin(); it != mp_InteractableObject.end(); )
		{
			Object* obj = *it;

			if (obj == nullptr || obj->GetId() == collidedWith->GetId())
			{ 
				obj->SetInteract(false);
				obj->SetColor(Color{ 255, 255, 255, 255 });
				it = mp_InteractableObject.erase(it);
			} 
			else
			{
				++it;
			}

		}
	}

	void GCPlayer::OnTriggerEnter(Entity* collidedWith)
	{
		if (collidedWith->IsTag(GameTag::Decor))
		{
			Object* pObj = static_cast<Object*>(collidedWith);

			bool alreadyIn = false;
			for (auto& obj : mp_InteractableObject)
			{
				if (collidedWith->GetId() == obj->GetId())
					alreadyIn = true;
			}

			if (!alreadyIn && pObj->CanBeInteractWith())
			{
				mp_InteractableObject.push_back(pObj);
				pObj->SetColor(Color{ 0, 255, 0, 150 });
				pObj->SetInteract(true);
			}
		}
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
		if (!m_CanShoot)
			return;

		::Scene* pScene = ::SceneManager::GetInstance().GetCurrentScene();

		Vector2f posToGo = pScene->GetCurrentCamera()->GetMouseScreenToWorldPosition();

		Projectile* pProj = pScene->CreateEntity<Projectile>(gcle::Shapes::Circle);
		pProj->SetOwner(this);

		pProj->SetPosition(GetPosition().x, GetPosition().y);
		pProj->ScaleBy({ 0.1f, 0.1f });
		pProj->GetTransform2D().UpdateChildPosition();

		pProj->SetColor(Color::Yellow);
		pProj->SetSpeed(400.0f);
		pProj->GoToDirection(posToGo.x, posToGo.y);
	}

	void GCPlayer::Damage(int amount)
	{
		Character::Damage(amount); 
	}
}