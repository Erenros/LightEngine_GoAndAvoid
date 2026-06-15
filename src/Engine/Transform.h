#include <vector>

#include"Entity.h"

namespace Transform {

    using Radiant = float;

    struct Transform2D
    {
        //vector2D position
        //vector2D direction
        Radiant angle;
        Transform2D* mp_Parent;
        std::vector<Transform2D*> mp_Childs;

        void SetParent(Transform2D* pParent);
        void UpdateLocalWithWorldParentAddition();

        Transform2D* GetParent();
        Transform2D* GetChild(uint32_t index);
        const Transform2D* GetParent() const;
        const Transform2D* GetChild(uint32_t index) const;
        uint32_t GetChildCount() const;
    };


    Transform2D::~Transform2D() {

        if (mp_Parent != nullptr) {
            for (auto it = mp_Parent->mp_Childs.begin(); it != mp_Parent->mp_Childs.end(); ++it) {
                if (*it == this) {
                    mp_Parent->mp_Childs.erase(it);
                    break;
                }
            }
        }

        for (auto pChild : mp_Childs) {
            pChild->SetParent(nullptr);
        }
    }

#pragma region Parent/Childs

    Transform2D* Transform2D::GetParent() {
        return mp_Parent;
    }

    Transform2D* Transform2D::GetChild(uint32_t index) {
        return mp_Childs[index];
    }

    const Transform2D* Transform2D::GetParent() const {
        return mp_Parent;
    }

    const Transform2D* Transform2D::GetChild(uint32_t index) const {
        return mp_Childs[index];
    }

    uint32_t Transform2D::GetChildCount() const {
        return mp_Childs.size();
    }

    void Transform2D::SetParent(Transform2D* pParent) {
        if (pParent == this) {
            //GPC_WARNING << "You are trying to create a loop in transform parent" << ENDL;
            return;
        }
        if (mp_Parent != nullptr) {
            RemoveParent();
        }
        mp_Parent = pParent;
        if (mp_Parent != nullptr) {
            mp_Parent->mp_Childs.push_back(this);
        }
        UpdateLocalWithWorldParentAddition();
    }

    void Transform2D::AddChild(Transform2D* pChild) {
        if (pChild == this) {
            GPC_WARNING << "You are trying to create a loop in transform parent" << ENDL;
            return;
        }
        for (auto& child : mp_Childs) {
            if (child == pChild) return;
        }

        if (pChild->mp_Parent != nullptr) {
            pChild->RemoveParent();
        }
        mp_Childs.push_back(pChild);
        pChild->mp_Parent = this;
        pChild->UpdateLocalWithWorldParentAddition();
    }

    void Transform2D::RemoveParent() {
        if (mp_Parent != nullptr) {
            for (auto it = mp_Parent->mp_Childs.begin(); it != mp_Parent->mp_Childs.end(); ++it) {
                if (*it == this) {
                    mp_Parent->mp_Childs.erase(it);
                    break;
                }
            }
        }
        mp_Parent = nullptr;
    }

    void Transform3D::RemoveChild(uint32_t index) {
        if (index >= GetChildCount()) return;
        auto it = mp_Childs.begin();
        std::advance(it, index);
        mp_Childs[index]->UpdateLocalWithWorldParentRemoval();
        mp_Childs[index]->mp_Parent = nullptr;
        mp_Childs.erase(it);
    }

#pragma endregion


    void Transform2D::SetDirty() {
        if (m_IsDirty) return;
        m_IsDirty = true;
        for (auto pChild : mp_Childs) {
            pChild->SetDirty();
        }
    }
} // GPC