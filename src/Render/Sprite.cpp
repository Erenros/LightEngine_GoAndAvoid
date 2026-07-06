#include "Sprite.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Utils.h"
#include "Shape.h"

Sprite::Sprite(Window* window, const std::string& path)
{
	m_isSprite = true;

	InitTexture(window, path);
	
	if (!IsTextureInit())
		return;

	SDL_QueryTexture(GetSDLTexture(), NULL, NULL, &m_width, &m_height);
	DEBUG_INFO << "width : " << m_width << " / Height : " << m_height << ENDL; //A virer 
}

Sprite::Sprite(Window* window, Asset* asset)
{
	InitTextureWithBuffer(window, asset);
}

Sprite::Sprite(Window* window, Surface* surface)
{
	InitTextureWithSurface(window, surface);
	SDL_QueryTexture(GetSDLTexture(), NULL, NULL, &m_width, &m_height);
	m_isSprite = true;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(mp_texture);

	for (auto& anim : m_animationMap)
		delete anim.second;

	m_animationMap.clear();
}

void Sprite::AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration)
{
	m_animationMap[id] = new Animation(firstFrame, lastFrame, line, tileWidth, tileHeight, duration);
}


void Sprite::PlayAnimation(const std::string& id)
{
	if (!m_animationMap.contains(id))
	{
		DEBUG_WARN << "No animation with id : " << id << ENDL;
		return;
	}


	//Yea it sucks but for now i don't want to think of how to do smooth transition
	mp_currentAnimation = m_animationMap[id];
	m_currentFrameX = mp_currentAnimation->m_firstFrame;
	m_currentFrameY = mp_currentAnimation->m_line;
	m_timer = mp_currentAnimation->m_duration;
}


void Sprite::UpdateAnimation(float32 deltatime, gcle::Shape* shape)
{
	Animation* anim = mp_currentAnimation;

	if (anim == nullptr)
		return;

	m_timer += deltatime;
	if(m_timer < mp_currentAnimation->m_duration)
		return;

	m_timer = 0.f;

	m_currentFrameX++;
	if (m_currentFrameX > anim->m_lastFrame)
		m_currentFrameX = anim->m_firstFrame;

	int x = m_currentFrameX * anim->m_tileW;
	int y = m_currentFrameY * anim->m_tileH;

	shape->SetTextureRect(x, y, anim->m_tileW, anim->m_tileH, m_width, m_height);
}
