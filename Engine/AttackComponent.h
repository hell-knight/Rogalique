#pragma once
#include "Component.h"
#include "EngineAPI.h"

namespace MyEngine {
class HealthComponent;

class ENGINE_API AttackComponent : public Component {
   public:
    AttackComponent(GameObject* gameObject, float newDamage, float newRadius,
                    float newCooldown);

    void SetDamage(float dmg);
    void SetRadius(float rad);
    void SetCooldown(float cd);

    void Update(float deltaTime) override;
    void Render() override {}

    bool Attack();

   private:
    float damage = 10.f;
    float radius = 100.f;
    float cooldown = 1.f;
    float timeSinceLastAttack = 0.f;

    HealthComponent* FindTargetInRange() const;
};
}  // namespace MyEngine