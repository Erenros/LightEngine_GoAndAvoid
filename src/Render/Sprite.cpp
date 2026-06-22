#include "Sprite.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Utils.h"

Sprite::Sprite(Window* window, const std::string& path)
{
	m_isSprite = true;

	InitTexture(window, path);
	
	if (!IsTextureInit())
		return;

	SDL_QueryTexture(GetSDLTexture(), NULL, NULL, &m_width, &m_height);
	DEBUG_INFO << "width : " << m_width << " / Height : " << m_height << ENDL; //A virer 
}

void Sprite::PlayAnimation(const std::string& id)
{
	if (!m_animationMap.contains(id))
	{
		DEBUG_WARN << "No animation with id : " << id << ENDL;
		return;
	}


	//Yea it sucks but for now i don't want to think of how to do smooth transition
	m_currentAnimation = m_animationMap[id];
	m_currentFrameX = m_currentAnimation->m_firstFrame;
	m_currentFrameY = m_currentAnimation->m_line;
	m_timer = m_currentAnimation->m_duration;
}


void Sprite::UpdateAnimation(float deltatime)
{
	m_timer -= deltatime;
	if(m_timer > 0)
		return;

	m_timer = m_currentAnimation->m_duration;

	m_currentFrameX++;
	if (m_currentFrameX > m_currentAnimation->m_lastFrame)
		m_currentFrameX = m_currentAnimation->m_firstFrame;
}
