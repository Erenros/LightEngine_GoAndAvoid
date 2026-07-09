#include "Object.h"

void Demo::Object::Interact()
{
	if (m_CanInteract)
		OnInteract();
}

bool Demo::Object::CanBeInteractWith() const
{
	return m_CanInteract;
}
