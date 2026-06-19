#include "Scene.h"



void Scene::Draw(Window* window) {
	for (Entity* e : GameManager::GetInstance().m_entities) {
		if(e->IsActiveIn(m_tag)){
			GameManager::GetInstance().GetWindow()->Draw(e->GetRenderShape());
		}
	}

	for (Text* t : m_texts)
		GameManager::GetInstance().GetWindow()->DrawTextOnRenderer(t);
}

Scene::~Scene()
{
	for (Text* t : m_texts)
		delete t;
	m_texts.clear();
}

Text* Scene::CreateText(Font* font, std::string& text, int x, int y, int w, int h, byte r, byte g, byte b)
{
	Text* new_text = new Text(font, text, x, y, w, h, r, g, b);
	m_texts.push_back(new_text);
	return new_text;
}

void Scene::DestroyText(Text* text)
{
	std::erase(m_texts, [text](Text* t) { return t == text; });
	delete text;
}

void Scene::Update(Timer& time){
	for (Entity* e : GameManager::GetInstance().m_entities) {
		if (e->IsActiveIn(m_tag)) {
			e->Update(time);
		}
	}
} 