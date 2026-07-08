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
	std::unordered_map<int, std::function<void()>> m_animationFunction;
	int m_frameId = 0;

};

enum AnimationMode
{
	Restart,
	KeepPlaying
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

	std::string m_currentAnimationId;
	float32 m_Timer = 0.f;

	int8 m_Mode = 0;

public:
	
	~Sprite() override;
	void UpdateAnimation(float32 deltatime, gcle::Shape* pShape);

	bool IsAnimationPlaying();

	Sprite(Window* pWindow, const std::string& path);
	Sprite(Window* pWindow, Asset* pAsset);
	Sprite(Window* pWindow, Surface* pSurface);

	void AddAnimation(const std::string& id, 
		int32 firstFrame, 
		int32 lastFrame, 
		int32 line, 
		int32 tileWidth, 
		int32 tileHeight, 
		float32 duration = 0.5f);
 
	void PlayAnimation(const std::string& id, AnimationMode mode);

	void AddFunctionInFrame(const std::string& animation, int32 frame, std::function<void()> function); 
	void StopAnimation();  
	void RemoveFunctionInFrame(const std::string& animation, int32 frame);
};