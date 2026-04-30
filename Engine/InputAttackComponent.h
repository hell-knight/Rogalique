#pragma once
#include "Component.h"
#include "EngineAPI.h"

namespace MyEngine
{
    class InputComponent;
    class AttackComponent;

    class ENGINE_API InputAttackComponent : public Component
    {
    public:
        InputAttackComponent(GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override {}

    private:
        InputComponent* input = nullptr;
        AttackComponent* attack = nullptr;
    };
}