#include "GameObject.h"
#include "SceneManager.h"
#include "PhysicsManager.h"


void GameObject::Initialize(gcle::Shapes shape)
{
    m_ToDestroy = false;

    mp_RenderShape = GetBaseShape(shape);
}

void GameObject::Update(float32 dt)
{
    m_Transform.UpdateChildPosition();

    if (mp_RenderShape != nullptr) {
        Texture* tex = mp_RenderShape->GetTexture();
        if (tex != nullptr)
        {
            if (tex->IsSprite())
                static_cast<Sprite*>(tex)->UpdateAnimation(dt, mp_RenderShape);
        }
    }
}

void GameObject::SetPosition(float32 x, float32 y)
{
    m_Transform.SetPosition({ x, y });
    OnSetPosition(x, y);
}

void GameObject::SetRenderPosition(Vector2f v, float ratioX, float ratioY)
{
    mp_RenderShape->SetPosition(v.x, v.y, ratioX, ratioY);
}

void GameObject::SetRenderPosition(float32 x, float32 y, float ratioX, float ratioY)
{
    mp_RenderShape->SetPosition(x, y, ratioX, ratioY);
}

void GameObject::SetRenderSize(int shapeType, std::vector<float32> points)
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

void GameObject::SetScale(Vector2f scale)
{
    m_Transform.SetScale(scale);
}

void GameObject::ScaleBy(Vector2f factor)
{
    Vector2f current = m_Transform.GetScale();
    m_Transform.SetScale({ current.x * factor.x, current.y * factor.y });
}

void GameObject::Rotate(Degrees delta)
{
    Degrees newAngle = m_Transform.GetDegAngle() + delta;
    newAngle = std::fmod(newAngle, 360.0f);
    if (newAngle < 0.0f)
        newAngle += 360.0f;

    m_Transform.SetDegAngle(newAngle);
}

void GameObject::SetRotation(Degrees angle)
{
    m_Transform.SetDegAngle(angle);
}

void GameObject::SetDebugLayer(int32 layer)
{
    m_Layer = std::clamp(layer, 0, 31);
}

void GameObject::SetTexture(const std::string& id)
{
    if (mp_RenderShape == nullptr)
        return;

    RessourceManager& RM = RessourceManager::GetInstance();

    if (SceneManager::GetInstance().GetCurrentSceneTag() != "") {
        for (auto& sId : m_ActiveScenes)
            SceneManager::GetInstance().GetSceneWithTag(sId)->AddDrawnTexture(id);
        if (RM.GetSurface(id)->mp_surface == nullptr) {
            std::string path = "../../assets/textures/" + id + ".png";
            RM.LoadSurface(GameManager::GetInstance().GetWindow(), path, id);
        }
    }

    mp_RenderShape->SetTexture(GameManager::GetInstance().GetWindow(), RM.GetSurface(id));
    RM.AddTexture(id, mp_RenderShape->GetTexture());
}

void GameObject::AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration)
{
    Sprite* sprite = mp_RenderShape->GetTexture();
    if (!sprite)
    {
        GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
        return;
    }

    sprite->AddAnimation(id, firstFrame, lastFrame, line, tileWidth, tileHeight, duration);
}

void GameObject::PlayAnimation(const std::string& id, int32 mode)
{
    Sprite* sprite = mp_RenderShape->GetTexture();
    if (!sprite)
    {
        GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
        return;
    }

    sprite->PlayAnimation(id, mode);
}

Vector2f GameObject::GetScale()
{
    return m_Transform.GetScale();
}

Degrees GameObject::GetRotation()
{
    return m_Transform.GetDegAngle();
}

Vector2f GameObject::GetPosition()
{
    return m_Transform.GetPosition();
}

Vector2f GameObject::GetRenderPosition()
{
    return mp_RenderShape->GetPosition();
}

bool GameObject::IsInside(Vector2f position)
{
    return PhysicsManager::GetInstance().IsInside(mp_RenderShape, position);
}

void GameObject::AddActiveScene(const std::string& sceneTag)
{
    if (std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), sceneTag) != m_ActiveScenes.end()) {
        std::cerr << sceneTag << "exists" << std::endl;
        return;
    }

    m_ActiveScenes.push_back(sceneTag);
    if (mp_RenderShape->GetTexture() != nullptr) {
        SceneManager::GetInstance().GetSceneWithTag(sceneTag)->AddDrawnTexture(mp_RenderShape->GetTexture()->GetId());
    }
}

void GameObject::RemoveActiveScene(const std::string& sceneTag)
{
    std::vector<std::string>::iterator it = std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), sceneTag);
    if (it == m_ActiveScenes.end()) {
        std::cerr << sceneTag << " doesn't exists " << std::endl;
        return;
    }
    m_ActiveScenes.erase(it);
}

bool GameObject::IsActiveIn(const std::string& sceneTag)
{
    return (std::find(m_ActiveScenes.begin(), m_ActiveScenes.end(), sceneTag) != m_ActiveScenes.end());
}

gcle::Shape* GameObject::GetBaseShape(gcle::Shapes shape)
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

void GameObject::AddFunctionInFrame(const std::string& animation, int32 frame, std::function<void* ()> function) {
    Sprite* sprite = mp_RenderShape->GetTexture();
    if (!sprite)
    {
        GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
        return;
    }
    sprite->AddFunctionInFrameSprite(animation, frame, function);
}

void GameObject::RemoveFunctionInFrame(const std::string& animation, int32 frame) {
    Sprite* sprite = mp_RenderShape->GetTexture();
    if (!sprite)
    {
        GCLE_WARN << "Entity don't have texture, add one before use this function" << ENDL;
        return;
    }
    sprite->RemoveFunctionInFrame(animation, frame);
}