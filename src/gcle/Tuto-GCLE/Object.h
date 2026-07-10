#pragma once

#include "Engine/Entity.h"

namespace Demo
{
	class Object : public Entity
	{
	public:

		void Interact(Entity* pEntity);
		bool CanBeInteractWith() const;
		void SetInteract(bool interact);

	protected:
		virtual void OnInteract(Entity* pEntity) {};

	protected:
		bool m_CanInteract = false;
	};
}
