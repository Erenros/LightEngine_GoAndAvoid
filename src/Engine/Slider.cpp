#include "Slider.h"
#include "GameManager.h"
#include <algorithm>

void Slider::SetRange(float32 minValue, float32 maxValue)
{
    m_MinValue = minValue;
    m_MaxValue = maxValue;

    SetValue(m_Value, false);
}

void Slider::SetHandle(UI* handle)
{
    mp_Handle = handle;

    if (mp_Handle != nullptr && !m_HandleTextureId.empty())
        mp_Handle->SetTexture(m_HandleTextureId);

    UpdateVisual();
}

void Slider::SetHandleSizeRatio(float32 ratio)
{
    m_HandleSizeRatio = ratio;
    UpdateVisual();
}

void Slider::SetHandleTexture(const std::string& id)
{
    m_HandleTextureId = id;

    if (mp_Handle != nullptr && !id.empty())
        mp_Handle->SetTexture(id);
}

void Slider::SetOrientation(SliderOrientation orientation)
{
    m_Orientation = orientation;
    UpdateVisual();
}

void Slider::SetVisualMode(SliderVisualMode mode)
{
    m_VisualMode = mode;
    UpdateVisual();
}

SliderVisualMode Slider::GetVisualMode() const
{
    return m_VisualMode;
}

void Slider::SetOnValueChanged(std::function<void(float32)> callback)
{
    m_OnValueChanged = std::move(callback);
}

float32 Slider::GetValue() const
{
    return m_Value;
}

void Slider::SetValue(float32 value, bool notify)
{
    value = std::clamp(value, m_MinValue, m_MaxValue);

    if (value == m_Value)
        return;

    m_Value = value;

    UpdateVisual();

    if (notify && m_OnValueChanged)
        m_OnValueChanged(m_Value);
}

void Slider::SetPosition(float32 x, float32 y)
{
    m_AnchorPosition = { x, y };

    GameObject::SetPosition(x, y);

    UpdateVisual();
}

void Slider::SetRenderSize(int32 shapeType, const std::vector<float32>& points)
{
    if (shapeType == 0 && points.size() >= 2)
    {
        m_FullSize = { points[0], points[1] };
        m_HasFullSize = true;
    }

    GameObject::SetRenderSize(shapeType, points);

    UpdateVisual();
}

void Slider::Update(float32 dt)
{
    Interactable::Update(dt);

    if (IsPressed())
        UpdateValueFromMouse();
}

void Slider::OnPressed()
{
    UpdateValueFromMouse();
}

void Slider::UpdateValueFromMouse()
{ 
    if (m_VisualMode == SliderVisualMode::Fill)
        return;

    Vector2f mouse = GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget();

    Vector2f pos = GetRenderPosition();

    float length =
        (m_Orientation == SliderOrientation::Horizontal)
        ? GetRenderShape()->GetWidth()
        : GetRenderShape()->GetHeight();

    float axis =
        (m_Orientation == SliderOrientation::Horizontal)
        ? mouse.x - pos.x
        : mouse.y - pos.y;

    float t = std::clamp(axis / length, 0.f, 1.f);

    SetValue(m_MinValue + t * (m_MaxValue - m_MinValue));
}

void Slider::UpdateVisual()
{
    if (m_VisualMode == SliderVisualMode::Fill)
    {
        UpdateVisualFill();
        return;
    } 

    UpdateVisualHandle();
}

void Slider::UpdateVisualHandle()
{
    if (!mp_Handle || !GetRenderShape())
        return;

    float t = (m_MaxValue > m_MinValue) ? (m_Value - m_MinValue) / (m_MaxValue - m_MinValue) : 0.f;

    float width = GetRenderShape()->GetWidth();
    float height = GetRenderShape()->GetHeight();

    if (m_Orientation == SliderOrientation::Horizontal)
    {
        float handleWidth = width * m_HandleSizeRatio;
        float x = t * (width - handleWidth);

        mp_Handle->SetPosition(
            GetPosition().x + x,
            GetPosition().y);
    }
    else
    {
        float handleHeight = height * m_HandleSizeRatio;
        float y = t * (height - handleHeight);

        mp_Handle->SetPosition(
            GetPosition().x,
            GetPosition().y + y);
    }
}

void Slider::UpdateVisualFill()
{
    if (!m_HasFullSize || GetRenderShape() == nullptr)
        return;

    float32 t = (m_MaxValue > m_MinValue) ? (m_Value - m_MinValue) / (m_MaxValue - m_MinValue) : 0.f;
    t = std::clamp(t, 0.f, 1.f);

    if (m_Orientation == SliderOrientation::Horizontal)
    {
        float32 newWidth = m_FullSize.x * t;
         
        float32 leftEdge = m_AnchorPosition.x - m_FullSize.x * 0.5f;

        GameObject::SetRenderSize(0, { newWidth, m_FullSize.y });
        SetRenderPosition({ leftEdge, m_AnchorPosition.y }, 0.0f, 0.5f);
    }
    else
    {
        float32 newHeight = m_FullSize.y * t;
         
        float32 topEdge = m_AnchorPosition.y - m_FullSize.y * 0.5f;

        GameObject::SetRenderSize(0, { m_FullSize.x, newHeight });
        SetRenderPosition({ m_AnchorPosition.x, topEdge }, 0.5f, 0.0f);
    }
}