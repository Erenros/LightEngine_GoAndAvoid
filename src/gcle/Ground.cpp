//#include "Enemy.h"
//#include "KillableEntity.h"
//
//void Enemy::OnInitialize()
//{
//    SetRigidBody(true);
//    if (GetRigidBody() != nullptr)
//    {
//        GetRigidBody()->SetActive(false);
//    }
//}
//
//void Enemy::OnCollisionEnter(Entity* other)
//{
//    KillableEntity* victim = dynamic_cast<KillableEntity*>(other);
//
//    if (victim != nullptr)
//    {
//        victim->TakeDamage(1);
//    }
//}