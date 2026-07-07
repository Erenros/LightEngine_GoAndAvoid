#pragma once
#include <cmath>

#include "include.h"
#include "Render/Shape.h"
#include "Transform.h"

#include "Render/Texture.h"
#include "Render/Sprite.h"

class GameObject
{

protected:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual void Update(float32 dt);
	virtual void Initialize(gcle::Shapes shape);
	gcle::Shape* GetBaseShape(gcle::Shapes shape);

public:

	void SetPosition(float32 x, float32 y);
	void SetRenderPosition(Vector2f v, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetRenderPosition(float32 x, float32 y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetRenderSize(int shapeType, std::vector<float32> points);

	void SetScale(Vector2f scale);
	void ScaleBy(Vector2f factor);
	void SetScale(float32 scale) { SetScale({ scale, scale }); }

	void Rotate(Degrees delta);
	void SetRotation(Degrees angle);

	void SetTexture(const std::string& id);

	void AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration = 0.5f);
	void PlayAnimation(const std::string& id, int32 mode = 0);

public:
	Vector2f GetScale();
	Degrees GetRotation();
	Vector2f GetPosition();
	Vector2f GetRenderPosition();
	Transform2D& GetTransform2D() { return m_Transform; }
	gcle::Shape* GetRenderShape() { return mp_RenderShape; };

public:
	bool IsInside(Vector2f position);
	bool ToDestroy() const { return m_ToDestroy; }

public:
	void AddActiveScene(const std::string& sceneTag);
	void RemoveActiveScene(const std::string& sceneTag);
	bool IsActiveIn(const std::string& sceneTag);
	void AddFunctionInFrame(const std::string& animation, int32 frame, std::function<void* ()> function);
	void RemoveFunctionInFrame(const std::string& animation, int32 frame);

protected:
	virtual void OnUpdate() {};
	virtual void OnDestroy() {};
	virtual void OnInitialize() {};

protected:
	void SetDebugLayer(int32 layer) { m_layer = std::clamp(layer, 0, 31); }

protected:
	Transform2D m_Transform;
	gcle::Shape* mp_RenderShape = nullptr;
	std::vector<std::string> m_activeScenes;

protected:
	int64 m_Id = 0;
	bool m_ToDestroy = false;

protected:
	int32 m_Mask = 0;
	int32 m_layer = 0;

private:
	friend class Scene;
	friend class Camera;
	friend class GameManager;
	friend class PhysicsManager;
};

