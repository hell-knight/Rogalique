#pragma once
#include "Component.h"
#include "EngineAPI.h"
#include <memory>

namespace MyEngine {
class TransformComponent;
class AttackComponent;
class GameObject;

class ENGINE_API AIAttackComponent : public Component {
   public:
    AIAttackComponent(GameObject* gameObject);

    void SetTarget(GameObject* target);
    void SetAttackRange(float range);

    void Update(float deltaTime) override;
    void Render() override {}

   private:
    TransformComponent* selfTransform = nullptr;
    std::weak_ptr<GameObject> targetGameObject;
    AttackComponent* attack = nullptr;
    float attackRange = 150.f;
};
}  // namespace MyEngine