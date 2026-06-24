#include "Scene.h"
#include "Render/Text.h"


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

Text* Scene::CreateText(const std::string& text, int x, int y, int w, int h, byte r, byte g, byte b)
{
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	if (font == nullptr)
	{
		DEBUG_WARN << "Couldn't find the default font" << ENDL;
		return nullptr;
	}

	Text* new_text = new Text(font,text, x, y, w, h, r, g, b);
	m_texts.push_back(new_text);
	return new_text;
}

void Scene::DestroyText(Text* text)
{
	std::erase_if(m_texts, [text](Text* t) { return t == text; });
	delete text;
}

void Scene::AddDrawnTexture(const std::string& textureName){
	if (std::find(m_activeTextures.begin(), m_activeTextures.end(), textureName) != m_activeTextures.end()) {
		return;
	}
	m_activeTextures.push_back(textureName);
}

bool Scene::isDrawn(const std::string& tag){
	return (std::find(m_activeTextures.begin(), m_activeTextures.end(), tag) != m_activeTextures.end());
}

void Scene::Update(Clock& time){
	for (Entity* e : GameManager::GetInstance().m_entities) {
		if (e->IsActiveIn(m_tag)) {
			e->Update(time);
		}
	}
} 