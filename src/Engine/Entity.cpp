#include "Entity.h"
#include "Core/InputManager.h"
#include "RessourceManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Collider.h"


static int64 sId = 0;

void Entity::Initialize(gcle::Shapes shape)
{
	m_ToDestroy = false;
	m_Tag = -1;


	mp_RenderShape = GetBaseShape(shape);


	m_RigidBody.Initialize(&m_Transform);
	m_RigidBody.SetActive(true);

	m_Target.isSet = false;

	m_Id = sId++;

	OnInitialize();
}

void Entity::Initialize() {

	m_Direction = { 0.0f, 0.0f };
	m_Speed = 0.f;
	m_ToDestroy = false;
	m_Tag = -1;
	m_Target;


	mp_RenderShape = nullptr;
	m_Transform.Initialize({ 0, 0 }, 0);
	m_RigidBody.Initialize(&m_Transform);
	m_RigidBody.SetActive(true);

	m_Id = sId++;

	OnInitialize();
}

gcle::Shape* Entity::GetBaseShape(gcle::Shapes shape)
{
	switch (shape)
	{
	case gcle::Shapes::Rectangle:
	{
		gcle::Rectangle* pRect = GCLE_NEW gcle::Rectangle(0.0f, 0.0f, 100.0f, 100.0f, Color{ 255, 255, 255, 255 }, this);
		return pRect;
		break;
	}
	case gcle::Shapes::Circle:
	{
		gcle::Circle* pCircle = GCLE_NEW gcle::Circle(0.0f, 0.0f, 100.0f, 32, Color{ 255, 255, 255, 255 }, this);
		return pCircle;
		break;
	}
	case gcle::Shapes::Triangle:
	{
		gcle::Triangle* pTriangle = GCLE_NEW gcle::Triangle(0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 100.0f, Color{ 255, 255, 255, 255 }, this);
		return pTriangle;
		break;
	}
	case gcle::Shapes::Count:
		break;
	default:
		break;
	}

	return nullptr;
}

void Entity::SetDebugLayer(int32 layer)
{
	m_Layer = std::clamp(layer, 0, 31);
}

void Entity::Update(float32 dt)
{
	if (m_RigidBody.IsActive())
		m_RigidBody.Update(dt);

	m_Transform.UpdateChildPosition();

	if (m_IsStatic == false)
	{
		float32 distance = dt * m_Speed;
		Vector2f translation = m_Direction * distance;

		Move(translation);
		if (mp_RenderShape != nullptr) {
			Texture* tex = mp_RenderShape->GetTexture();
			if (tex != nullptr)
			{
				if (tex->IsSprite())
					static_cast<Sprite*>(tex)->UpdateAnimation(dt, mp_RenderShape);
			}
		}
		if (m_Target.isSet)
		{
			float32 x1 = GetPosition().x;
			float32 y1 = GetPosition().y;

			float32 x2 = x1 + m_Direction.x * m_Target.distance;
			float32 y2 = y1 + m_Direction.y * m_Target.distance;

			m_Target.distance -= distance;

			if (m_Target.distance <= 0)
			{
				SetPosition(m_Target.position.x, m_Target.position.y);
				m_Direction = Vector2f({ 0.f, 0.f });
				m_Target.isSet = false;
			}
		}
	}

	OnUpdate();
}

void Entity::AddCollider(Collider* pCollider)
{
	if (pCollider == nullptr)
		return;

	pCollider->SetOwner(this);
	mp_Colliders.insert(pCollider);
}

void Entity::RemoveCollider(Collider* pCollider)
{
	if (pCollider == nullptr)
		return;

	mp_Colliders.erase(pCollider);
}

const std::unordered_set<Collider*>& Entity::GetColliders() const
{
	return mp_Colliders;
}

Collider* Entity::CreateCollider(gcle::Shapes shape, bool isActive, Vector2f relativePosition, float32 rotation, Vector2f scale)
{
	Collider* collider = GCLE_NEW Collider();

	gcle::Shape* colliderShape = GetBaseShape(shape);

	Vector2f entityScale = m_Transform.GetScale();
	colliderShape->SetScale({ scale.x * entityScale.x, scale.y * entityScale.y }); 

	collider->Initialize(colliderShape, m_Transform.GetPosition() + relativePosition, rotation, this);

	AddCollider(collider);
	collider->SetActive(isActive);

	return collider;
}

void Entity::Destroy()
{
	if (this != nullptr)
	{
		m_ToDestroy = true;
		PhysicsManager::GetInstance().RemoveEntity(this);
		OnDestroy();
	}
}

bool Entity::GoToPosition(float32 x, float32 y, float32 speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	Vector2f position = m_Transform.GetPosition();

	m_Target.position = { x, y };
	m_Target.distance = position.GetDistance({ x, y });
	m_Target.isSet = true;

	return true;
}

void Entity::Move(Vector2f translation) {
	Vector2f pivot = m_Transform.GetPosition();
	m_Transform.SetPosition({ pivot.x + translation.x, pivot.y + translation.y });
}

bool Entity::GoToDirection(float32 x, float32 y, float32 speed)
{
	Vector2f position = m_Transform.GetPosition();
	Vector2f direction = Vector2f({ x - position.x, y - position.y });

	direction = direction.Normalized();

	SetDirection(direction.x, direction.y, speed);

	return true;
}

void Entity::SetTag(int32 tag)
{
	m_Tag = tag;
}

Vector2f Entity::GetPosition()
{
	return m_Transform.GetPosition();
}

void Entity::SetSpeed(float32 speed)
{
	m_Speed = speed;
}

void Entity::SetDirection(float32 x, float32 y, float32 speed)
{
	if (speed > 0)
	{
		m_Speed = speed;
	}

	m_Direction = { x, y };
}

void Entity::SetRigidBody(bool isRigidBody)
{
	m_RigidBody.SetActive(isRigidBody);

	if (isRigidBody)
	{
		PhysicsManager::GetInstance().AddEntity(this);
		m_IsHighlighted = true;
	}

	if (!isRigidBody)
	{
		PhysicsManager::GetInstance().RemoveEntity(this);
		m_IsHighlighted = false;
	}
}

void Entity::SetPosition(float32 x, float32 y)
{
	m_Transform.SetPosition({ x, y });
}

Vector2f Entity::GetScale()
{
	return m_Transform.GetScale();
}

Degrees Entity::GetRotation()
{
	return m_Transform.GetDegAngle();
}

void Entity::SetScale(Vector2f scale)
{
	m_Transform.SetScale(scale);
}

void Entity::ScaleBy(Vector2f factor)
{
	Vector2f current = m_Transform.GetScale();
	m_Transform.SetScale({ current.x * factor.x, current.y * factor.y });
}

void Entity::SetRotation(Degrees angle)
{
	m_Transform.SetDegAngle(angle);
}

void Entity::Rotate(Degrees delta)
{
	Degrees newAngle = m_Transform.GetDegAngle() + delta;
	newAngle = std::fmod(newAngle, 360.0f);
	if (newAngle < 0.0f)
		newAngle += 360.0f;

	m_Transform.SetDegAngle(newAngle);
}

void Entity::SetTexture(const std::string& id) {
	if (mp_RenderShape == nullptr)
		return;
	mp_RenderShape->SetTexture(RessourceManager::GetInstance().GetTexture(id));
	if (SceneManager::GetInstance().GetCurrentSceneTag() != "") {
		for (auto& sId : m_ActiveScenes)
			SceneManager::GetInstance().GetSceneWithTag(sId)->AddDrawnTexture(id);
		if (RessourceManager::GetInstance().GetTexture(id)->texture == nullptr) {
			std::string path = "../../assets/textures/" + id + ".png";
			RessourceManager::GetInstance().LoadTexture(GameManager::GetInstance().GetWindow(), path, id);
		}
	}
}

void Entity::SetRenderPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
{
	if(mp_RenderShape != nullptr)
		mp_RenderShape->SetPosition(x, y, ratioX, ratioY);
}

void Entity::SetRenderPosition(Vector2f v, float32 ratioX, float32 ratioY)
{
	if(mp_RenderShape != nullptr)
		mp_RenderShape->SetPosition(v.x, v.y, ratioX, ratioY);
}

void Entity::SetRenderSize(int32 shapeType, std::vector<float32> points)
{
	if (mp_RenderShape == nullptr)
		return;
	if (shapeType == 0)
	{


		static_cast<gcle::Rectangle*>(mp_RenderShape)->SetWidth(points[0]);


		float a = static_cast<gcle::Rectangle*>(mp_RenderShape)->GetWidth();



		mp_RenderShape->SetHeight(points[1]);
	}

	else if (shapeType == 1)
	{
		mp_RenderShape->SetRadius(points[0]);
	}

	else if (shapeType == 2)
	{
		std::vector<Vector2f> newTrianglePoints;
		newTrianglePoints.push_back({ points[0], points[1] });
		newTrianglePoints.push_back({ points[2], points[3] });
		newTrianglePoints.push_back({ points[4], points[5] });

		mp_RenderShape->SetTrianglePoints(newTrianglePoints);
	}
}

void Entity::SetStatic(bool isStatic)
{
	m_IsStatic = isStatic;
}

Vector2f Entity::GetRenderPosition()
{
	if (mp_RenderShape != nullptr)
		return mp_RenderShape->GetPosition();
	else
		return Vector2f();
}

bool Entity::IsStatic() const
{
	return !m_IsStatic;
}

bool Entity::IsColliding(Entity* pOther)
{
	for (Collider* pCollider : mp_Colliders)
	{
		if (!pCollider->IsActive())
			continue;

		for (Collider* pOtherCollider : pOther->mp_Colliders)
		{
			if (!pOtherCollider->IsActive())
				continue;

			if (PhysicsManager::GetInstance().IsColliding(pCollider, pOtherCollider))
				return true;
		}
	}

	return false;
}

bool Entity::IsInside(Vector2f position)
{
	return PhysicsManager::GetInstance().IsInside(this, position);
}

bool Entity::ToDestroy() const
{
	return m_ToDestroy;
}

bool Entity::IsTag(int32 tag) const
{
	return m_Tag == tag;
}

bool Entity::IsRigidBody() const
{
	return m_RigidBody.IsActive();
}

Entity::~Entity() {
	if (mp_RenderShape != nullptr) {
		delete mp_RenderShape;
		mp_RenderShape = nullptr;
	}

	for (auto& collider : mp_Colliders)
	{
		delete collider;
	}
	mp_Colliders.clear();

}

void Entity::AddActiveScene(const std::string& sceneTag) {
	if (std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), sceneTag) != m_ActiveScenes.end()) {
		std::cerr << sceneTag << "exists" << std::endl;
		return;
	}

	m_ActiveScenes.push_back(sceneTag);
	if (mp_RenderShape->GetTexture() != nullptr) {
		SceneManager::GetInstance().GetSceneWithTag(sceneTag)->AddDrawnTexture(mp_RenderShape->GetTexture()->GetId());
	}
}

void Entity::RemoveActiveScene(const std::string& sceneTag) {
	std::vector<std::string>::iterator it = std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), sceneTag);
	if (it == m_ActiveScenes.end()) {
		std::cerr << sceneTag << " doesn't exists " << std::endl;
		return;
	}
	m_ActiveScenes.erase(it);
}

bool Entity::IsActiveIn(const std::string& sceneTag) {
	return (std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), sceneTag) != m_ActiveScenes.end());
}

bool Entity::IsWorldText() const
{
	return m_IsWorldText;
}

void Entity::AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration)
{
	Sprite* sprite = mp_RenderShape->GetTexture();
	if (!sprite)
	{
		GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
		return;
	}

	sprite->AddAnimation(id, firstFrame, lastFrame, line, tileWidth, tileHeight, duration);
}

void Entity::PlayAnimation(const std::string& id, int32 mode)
{
	Sprite* sprite = mp_RenderShape->GetTexture();
	if (!sprite)
	{
		GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
		return;
	}

	sprite->PlayAnimation(id);
}

void Entity::AddFunctionInFrame(const std::string& animation, int32 frame, std::function<void*()> function) {
	Sprite* sprite = mp_RenderShape->GetTexture();
	if (!sprite)
	{
		GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
		return;
	}
	sprite->AddFunctionInFrameSprite(animation, frame, function);
}

void Entity::RemoveFunctionInFrame(const std::string& animation, int32 frame) {
	Sprite* sprite = mp_RenderShape->GetTexture();
	if (!sprite)
	{
		GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
		return;
	}
	sprite->RemoveFunctionInFrame(animation, frame);
}

void Entity::SetLayer(int32 layer)
{
	m_Layer = std::clamp(layer, 0, 15);
}

void Entity::SetScale(float32 scale) { 
	SetScale({ scale, scale }); 
}


int64			Entity::GetId() const		{ return m_Id; }
int32			Entity::GetLayer() const	{ return m_Layer; };
RigidBody2D&	Entity::GetRigidBody()		{ return m_RigidBody; }
Transform2D&	Entity::GetTransform2D()	{ return m_Transform; }
gcle::Shape*	Entity::GetRenderShape()	{ return mp_RenderShape; };