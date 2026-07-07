#pragma once

#include "GameObject.h"

class UI : public GameObject
{
public:
    UI() = default;
    virtual ~UI() = default;

protected:
    virtual void OnActivate() {}
    virtual void OnUpdate() override {}
    virtual void OnInitialize() override {}

private:
    void Initialize(gcle::Shapes shape) override;
    void Initialize();
    void Update(float32 dt) override;

private:
    friend class Scene;
    friend class GameManager;
};