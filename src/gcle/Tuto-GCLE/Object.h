#pragma once

#include "Engine/Entity.h"

namespace Demo
{
	class Object : public Entity
	{
	public:

		void Interact();
		bool CanBeInteractWith() const;

	protected:
		virtual void OnInteract() {};

	protected:
		bool m_CanInteract = true;
	};
}
