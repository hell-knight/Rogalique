#pragma once
#include "Component.h"
#include "EngineAPI.h"

namespace MyEngine {
class TransformComponent;
class AttackComponent;

class ENGINE_API AIAttackComponent : public Component {
   public:
    AIAttackComponent(GameObject* gameObject);

    void SetTarget(GameObject* target);
    void SetAttackRange(float range);

    void Update(float deltaTime) override;
    void Render() override {}

   private:
    TransformComponent* selfTransform = nullptr;
    TransformComponent* targetTransform = nullptr;
    AttackComponent* attack = nullptr;
    float attackRange = 150.f;
};
}  // namespace MyEngine