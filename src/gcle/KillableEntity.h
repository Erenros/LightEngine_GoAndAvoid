#pragma once
#include "Entity.h"


class KillableEntity: public Entity{
protected:
	int m_maxHp = 0;
	int m_hp = 0;
	int m_dmg = 0;

	virtual void OnDeath() { Destroy(); }

public:
	int GetHp() const { return m_hp; }
	int GetMaxHp() const { return m_maxHp; }
	int GetDamage() const { return m_dmg; }

	void TakeDamage(int dmg);
};

