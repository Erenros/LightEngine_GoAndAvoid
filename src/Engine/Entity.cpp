#include "Entity.h"
#include "Core/InputManager.h"
#include "RessourceManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Collider.h"


static int64 sId = 0;

void Entity::Initialize(gcle::Shapes shape)
{
	m_Direction = { 0.0f, 0.0f };
	m_Speed = 0.f;
	m_ToDestroy = false;
	m_Tag = -1;
	m_Target;


	mp_Shape = GetBaseShape(shape);
	mp_RenderShape = mp_Shape->Clone();


	m_RigidBody.Initialize(mp_Shape->GetTransform());
	m_RigidBody.SetActive(true);

	m_Target.isSet = false;

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

void Entity::Update(float32 dt)
{
	if (IsRigidBody())
		m_RigidBody.Update(dt);

	float32 distance = dt * m_Speed;
	Vector2f translation = m_Direction * distance;

	mp_Shape->Move(translation);
	Texture* tex = mp_RenderShape->GetTexture();
	if (tex != nullptr)
	{
		if (tex->IsSprite())
			static_cast<Sprite*>(tex)->UpdateAnimation(dt, mp_RenderShape);
	}
	if (m_Target.isSet)
	{
		float32 x1 = GetPosition(0.5f, 0.5f).x;
		float32 y1 = GetPosition(0.5f, 0.5f).y;

		float32 x2 = x1 + m_Direction.x * m_Target.distance;
		float32 y2 = y1 + m_Direction.y * m_Target.distance;

		m_Target.distance -= distance;

		if (m_Target.distance <= 0)
		{
			SetPosition(m_Target.position.x, m_Target.position.y, 0.5f, 0.5f);
			m_Direction = Vector2f({ 0.f, 0.f });
			m_Target.isSet = false;
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

Collider* Entity::CreateCollider(gcle::Shapes shape, bool isActive, Vector2f position, Vector2f scale)
{
	Collider* collider = new Collider();
	collider->Initialize(GetBaseShape(shape), position, this);
	AddCollider(collider);
	collider->SetActive(isActive);
	collider->GetShape()->SetScale(scale);
	return collider;
}


void Entity::Destroy()
{
	m_ToDestroy = true;
	PhysicsManager::GetInstance().RemoveEntity(this);
	OnDestroy();
}

bool Entity::GoToPosition(float32 x, float32 y, float32 speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	Vector2f position = mp_Shape->GetPosition(0.5f, 0.5f);

	m_Target.position = { x, y };
	m_Target.distance = position.GetDistance({ x, y });
	m_Target.isSet = true;

	return true;
}

bool Entity::GoToDirection(float32 x, float32 y, float32 speed)
{
	Vector2f position = mp_Shape->GetPosition(0.5f, 0.5f);
	Vector2f direction = Vector2f({ x - position.x, y - position.y });

	direction = direction.Normalized();

	SetDirection(direction.x, direction.y, speed);

	return true;
}

Vector2f Entity::GetPosition(float32 ratioX, float32 ratioY)
{
	return mp_Shape->GetPosition(ratioX, ratioY);
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
		PhysicsManager::GetInstance().AddEntity(this);

	if (!isRigidBody)
		PhysicsManager::GetInstance().RemoveEntity(this);
}

void Entity::SetPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
{
	mp_Shape->SetPosition(x, y, ratioX, ratioY);
}

Vector2f Entity::GetScale()
{
	return mp_Shape->GetScale();
}

Degrees Entity::GetRotation()
{
	return mp_Shape->GetRotation();
}

void Entity::SetScale(Vector2f scale)
{
	mp_Shape->SetScale(scale);
}

void Entity::ScaleBy(Vector2f factor)
{
	mp_Shape->ScaleBy(factor);
}

void Entity::SetRotation(Degrees angle)
{
	mp_Shape->SetRotation(angle);
}

void Entity::Rotate(Degrees delta)
{
	mp_Shape->Rotate(delta);
}

void Entity::SetTexture(const std::string& id) {
	mp_RenderShape->SetTexture(RessourceManager::GetInstance().GetTexture(id));
	if (SceneManager::GetInstance().GetCurrentSceneTag() != "") {
		for (auto& sId : m_activeScenes)
			SceneManager::GetInstance().GetSceneWithTag(sId)->AddDrawnTexture(id);
		if (RessourceManager::GetInstance().GetTexture(id)->mp_texture == nullptr) {
			std::string path = "../../assets/textures/" + id + ".png";
			RessourceManager::GetInstance().LoadTexture(GameManager::GetInstance().GetWindow(), path, id);
		}
	}
}

void Entity::SetRenderPosition(float32 x, float32 y, float ratioX, float ratioY)
{
	mp_RenderShape->SetPosition(x, y, ratioX, ratioY);
}

void Entity::SetRenderPosition(Vector2f v, float ratioX, float ratioY)
{
	mp_RenderShape->SetPosition(v.x, v.y, ratioX, ratioY);
}

void Entity::SetRenderSize(int shapeType, std::vector<float32> points)
{
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

Vector2f Entity::GetRenderPosition()
{
	return mp_RenderShape->GetPosition();
}

bool Entity::IsColliding(Entity* other)
{
	for (Collider* pCollider : mp_Colliders)
	{
		if (!pCollider->IsActive())
			continue;

		for (Collider* pOtherCollider : other->mp_Colliders)
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

Entity::~Entity() {
	delete mp_Shape;
	delete mp_RenderShape;
}

void Entity::AddActiveScene(const std::string& sceneTag) {
	if (std::find(m_activeScenes.begin(), m_activeScenes.end(), sceneTag) != m_activeScenes.end()) {
		std::cerr << sceneTag << "exists" << std::endl;
		return;
	}

	m_activeScenes.push_back(sceneTag);
	if (mp_RenderShape->GetTexture() != nullptr) {
		SceneManager::GetInstance().GetSceneWithTag(sceneTag)->AddDrawnTexture(mp_RenderShape->GetTexture()->id);
	}
}

void Entity::RemoveActiveScene(const std::string& sceneTag) {
	std::vector<std::string>::iterator it = std::find(m_activeScenes.begin(), m_activeScenes.end(), sceneTag);
	if (it == m_activeScenes.end()) {
		std::cerr << sceneTag << " doesn't exists " << std::endl;
		return;
	}
	m_activeScenes.erase(it);
}

bool Entity::IsActiveIn(const std::string& sceneTag) {
	return (std::find(m_activeScenes.begin(), m_activeScenes.end(), sceneTag) != m_activeScenes.end());
}

bool Entity::GetInQuerySeen(){
	return m_inQuerySeen;
}

void Entity::SetInQuerySeen(bool seen){
	m_inQuerySeen = seen;
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