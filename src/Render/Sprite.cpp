#include "Sprite.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Utils.h"
#include "Shape.h"

Sprite::Sprite(Window* window, const std::string& path)
{
	m_isSprite = true;

	InitTexture(window, path);

	if (!IsTextureInit())
		return;

	float32 texW = 0.f, texH = 0.f;
	SDL_GetTextureSize(GetSDLTexture(), &texW, &texH); 
	m_width = static_cast<int32>(texW);
	m_height = static_cast<int32>(texH);
}

Sprite::Sprite(Window* window, Asset* asset)
{
	InitTextureWithBuffer(window, asset);
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
	m_timer = 0.0f;
}


Sprite::~Sprite()
{
	for (auto& pair : m_animationMap)
		delete pair.second;
	m_animationMap.clear();
}

void Sprite::UpdateAnimation(float32 deltatime, gcle::Shape* shape)
{
	Animation* anim = mp_currentAnimation;

	if (anim == nullptr)
		return;

	m_timer += deltatime;
	if (m_timer < mp_currentAnimation->m_duration)
		return;

	m_timer = 0;
	anim->m_frameId++;

	m_currentFrameX++;
	if (m_currentFrameX > anim->m_lastFrame) {
		m_currentFrameX = anim->m_firstFrame;
		anim->m_frameId = 0;
	}


	int32 x = m_currentFrameX * anim->m_tileW;
	int32 y = m_currentFrameY * anim->m_tileH;

	if (anim->m_animationFunction.find(anim->m_frameId) != anim->m_animationFunction.end()) {
		anim->m_animationFunction[anim->m_frameId]();
	}

	shape->SetTextureRect(x, y, anim->m_tileW, anim->m_tileH, m_width, m_height);
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