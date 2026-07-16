#include "Toggle.h"
#include "Slider.h"

void Toggle::SetOn(bool isOn, bool notify)
{
    m_IsOn = isOn;
    if (!m_CheckedTexId.empty() && !m_UncheckedTexId.empty())
        SetTexture(m_IsOn ? m_CheckedTexId : m_UncheckedTexId);

    if (notify && m_OnValueChanged)
        m_OnValueChanged(m_IsOn);
}

bool Toggle::IsOn() const
{
    return m_IsOn;
}

void Toggle::SetOnCheckedTexture(const std::string& id)
{
    bool tex = false;
    if (!m_CheckedTexId.empty() && !m_UncheckedTexId.empty())
        tex = true;

    m_CheckedTexId = id;


    if (!tex && !m_CheckedTexId.empty())
        SetTexture(m_CheckedTexId);
}

void Toggle::SetOnUncheckedTexture(const std::string& id)
{
    bool tex = false;
    if (!m_CheckedTexId.empty() && !m_UncheckedTexId.empty())
        tex = true;

    m_UncheckedTexId = id;


    if (!tex && !m_UncheckedTexId.empty())
        SetTexture(m_UncheckedTexId);
}


void Toggle::SetOnValueChanged(std::function<void(bool)> callback)
{
    m_OnValueChanged = std::move(callback);
}

void Toggle::OnReleased(bool insideOnRelease)
{
    if (insideOnRelease)
        SetOn(!m_IsOn);
}
