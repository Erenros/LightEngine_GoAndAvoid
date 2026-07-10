#include "Object.h"

void Demo::Object::Interact(Entity* pEntity)
{
	if (m_CanInteract)
		OnInteract(pEntity);
}

bool Demo::Object::CanBeInteractWith() const
{
	return m_CanInteract;
}

void Demo::Object::SetInteract(bool interact)
{
	m_CanInteract = true;
}
