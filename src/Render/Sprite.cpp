#include "Sprite.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Utils.h"
#include "Shape.h"

Sprite::Sprite(Window* pWindow, const std::string& path)
{
	m_isSprite = true;

	InitTexture(pWindow, path);

	if (!IsTextureInit())
		return;

	float32 texW = 0.f, texH = 0.f;
	SDL_GetTextureSize(GetSDLTexture(), &texW, &texH); 
	m_Width = static_cast<int32>(texW);
	m_Height = static_cast<int32>(texH);
}

Sprite::Sprite(Window* pWindow, Asset* pAsset)
{
	InitTextureWithBuffer(pWindow, pAsset);
}

Sprite::Sprite(Window* window, Surface* surface)
{
	InitTextureWithSurface(window, surface);
	//SDL_QueryTexture(GetSDLTexture(), NULL, NULL, &m_width, &m_height);
	m_Width = surface->GetSDL_Surface()->w;
	m_Height = surface->GetSDL_Surface()->h;
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
	m_animationMap[id] = GCLE_NEW Animation(firstFrame, lastFrame, line, tileWidth, tileHeight, duration);
}


void Sprite::PlayAnimation(const std::string& id, AnimationMode mode)
{
	if (!m_animationMap.contains(id))
	{
		GCLE_WARN << "No animation with id : " << id << ENDL;
		return;
	}

	if (mode == AnimationMode::KeepPlaying && m_currentAnimationId == id)
		return;

	m_currentAnimationId = id;

	mp_currentAnimation = m_animationMap[id];
	mp_currentAnimation->m_frameId = 0;

	m_currentFrameX = mp_currentAnimation->m_firstFrame;
	m_currentFrameY = mp_currentAnimation->m_line;
	m_timer = mp_CurrentAnimation->duration; 
	m_Mode = mode;
}

void Sprite::StopAnimation()
{
	mp_CurrentAnimation = nullptr;
	m_CurrentFrameX = 0;
	m_CurrentFrameY = 0;
	m_Timer = 0.f;
	m_Mode = 0;
}

void Sprite::UpdateAnimation(float32 deltatime, gcle::Shape* pShape)
{
	Animation* anim = mp_CurrentAnimation;

	if (anim == nullptr)
		return;

	m_Timer += deltatime;
	if (m_Timer < mp_CurrentAnimation->duration)
		return;

	m_Timer = 0.f;
	anim->frameId++;

	m_CurrentFrameX++;
	if (m_CurrentFrameX > anim->lastFrame) 
	{
		if (m_Mode == 1)
		{
			StopAnimation();
			return;
		}

		m_CurrentFrameX = anim->firstFrame;
		anim->frameId = 0;
	}


	int32 x = m_CurrentFrameX * anim->tileW;
	int32 y = m_CurrentFrameY * anim->tileH;

	if (anim->m_animationFunction.find(anim->frameId) != anim->m_animationFunction.end()) {
		anim->m_animationFunction[anim->frameId]();
	}

	pShape->SetTextureRect(x, y, anim->tileW, anim->tileH, m_Width, m_Height);
}

bool Sprite::IsAnimationPlaying()
{
	return mp_CurrentAnimation == nullptr ? false : true;
}


void Sprite::AddFunctionInFrame(const std::string& animation, int32 frame, std::function<void()> function) {
	auto it = m_animationMap.find(animation);
	if (it == m_animationMap.end())
		return;

	Animation* anim = m_animationMap[animation];
	if (anim->m_animationFunction.find(frame) != anim->m_animationFunction.end())
		return;
	anim->m_animationFunction[frame] = function;
}

void Sprite::RemoveFunctionInFrame(const std::string& animation, int32 frame) {
	auto it = m_animationMap.find(animation);
	if (it == m_animationMap.end())
		return;

	Animation* anim = it->second;

	auto itFunction = anim->m_animationFunction.find(frame);
	if (itFunction != anim->m_animationFunction.end()) {
		anim->m_animationFunction.erase(itFunction);
	}
}