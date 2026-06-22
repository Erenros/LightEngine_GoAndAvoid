#pragma once
#include "Texture.h"
#include "Core/include.h"
#include <unordered_map>

class Window;

struct Animation
{
	int32 m_firstFrame = 0;
	int32 m_lastFrame = 0;

	int32 m_line = 0;

	int32 m_tileW = 0;
	int32 m_tileH = 0;
	
	float32 m_duration = 0;

	Animation(int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration = 0.5f) :
		m_firstFrame(firstFrame),
		m_lastFrame(lastFrame),
		m_line(line),
		m_tileW(tileWidth),
		m_tileH(tileHeight),
		m_duration(duration)
	{}
};


class Sprite : public Texture
{
private:

	int32 m_width = 0;
	int32 m_height = 0;

	std::unordered_map<std::string, Animation*> m_animationMap;

	Animation* m_currentAnimation = nullptr;
	int32 m_currentFrameX = 0;
	int32 m_currentFrameY = 0;

	float32 m_timer = 0.f;


public:
	
	void UpdateAnimation(float deltatime);

	bool IsAnimationPlaying() { return m_currentAnimation == nullptr ? false : true; };

	Sprite(Window* window, const std::string& path);

	void AddAnimation(const std::string& id, Animation* animation) { m_animationMap[id] = animation; };
	void PlayAnimation(const std::string& id);
};