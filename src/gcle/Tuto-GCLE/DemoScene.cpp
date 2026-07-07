#include "DemoScene.h"

#include "Tuto-GCLE/DemoPlayer.h"
#include "Tuto-GCLE/GCEnemy.h"

void DemoScene::OnInitialize()
{
	Scene::OnInitialize();

	pSceneCamera = CreateCamera();
	SwitchCamera(pSceneCamera);

	mp_Player = CreateEntity<Demo::GCPlayer>(gcle::Shapes::Rectangle);
	mp_Enemy  = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);

}

void DemoScene::OnUpdate(Clock& time)
{
	Scene::OnUpdate(time);
}
