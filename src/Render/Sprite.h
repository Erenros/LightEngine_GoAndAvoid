#pragma once
#include "Texture.h"
#include "Core/include.h"
#include <unordered_map>
#include <functional>

namespace gcle
{
	class Shape;
}
class Window;

struct Animation
{
	int32 firstFrame = 0;
	int32 lastFrame = 0;

	int32 line = 0;

	int32 tileW = 0;
	int32 tileH = 0;
	
	float32 duration = 0;


	Animation(int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration = 0.5f) :
		firstFrame(firstFrame),
		lastFrame(lastFrame),
		line(line),
		tileW(tileWidth),
		tileH(tileHeight),
		duration(duration)
	{}
	std::unordered_map<int, std::function<void*()>> m_animationFunction;
	int32 frameId = 0;

};


class Sprite : public Texture
{
private:

	int32 m_Width = 0;
	int32 m_Height = 0;

	std::unordered_map<std::string, Animation*> m_animationMap;

	Animation* mp_CurrentAnimation = nullptr;
	int32 m_CurrentFrameX = 0;
	int32 m_CurrentFrameY = 0;

	float32 m_Timer = 0.f;



public:
	
	~Sprite() override;
	void UpdateAnimation(float32 deltatime, gcle::Shape* pShape);

	bool IsAnimationPlaying();

	Sprite(Window* pWindow, const std::string& path);
	Sprite(Window* pWindow, Asset* pAsset);

	void AddAnimation(const std::string& id, 
		int32 firstFrame, 
		int32 lastFrame, 
		int32 line, 
		int32 tileWidth, 
		int32 tileHeight, 
		float32 duration = 0.5f);

	void PlayAnimation(const std::string& id);

	void AddFunctionInFrameSprite(const std::string& animation, int32 frame, std::function<void*()> function);
	void RemoveFunctionInFrame(const std::string& animation, int32 frame);
};