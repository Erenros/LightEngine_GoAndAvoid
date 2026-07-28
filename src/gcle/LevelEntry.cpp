#include "LevelEntry.h"
#include "Entity.h"

LevelEntry::LevelEntry(Vector2f position, Vector2f scale) :
    m_Position(position),
    m_Scale(scale)
{
}

bool LevelEntry::IsSpawned() const
{
    return mp_Entity != nullptr;
}

float32 LevelEntry::GetX() const
{
    return m_Position.x;
}