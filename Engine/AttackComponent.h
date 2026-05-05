#pragma once
#include "Component.h"
#include "EngineAPI.h"
#include <functional>
#include <map>

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

    int SubscribeOnAttack(std::function<void()> callback);
    void UnsubscribeOnAttack(int id);

   private:
    float damage = 10.f;
    float radius = 100.f;
    float cooldown = 1.f;
    float timeSinceLastAttack = 0.f;

    int nextAttackCallbackId = 1;
    std::map<int, std::function<void()>> attackCallbacks;

    HealthComponent* FindTargetInRange() const;
};
}  // namespace MyEngine