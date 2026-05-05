#pragma once
#include "Component.h"
#include "EngineAPI.h"
#include <functional>
#include <map>

namespace MyEngine {
class ENGINE_API HealthComponent : public Component {
   public:
    HealthComponent(GameObject* gameObject, float newHealth, float newArmor);

    void Update(float deltaTime) override {}
    void Render() override {}

    // void SetMaxHealth(float maxHP);
    // void SetArmor(float value);

    float GetHealth() const;
    float GetMaxHealth() const;
    float GetArmor() const;

    void ApplyDamage(float damage);
    void Heal(float amount);
    bool IsDead() const;

    int SubscribeOnDeath(std::function<void()> callback);
    void UnsubscribeOnDeath(int id);

    int SubscribeOnDamage(std::function<void(float)> callback);
    void UnsubscribeOnDamage(int id);

   private:
    float maxHealth = 100.f;
    float health = 100.f;
    float armor = 0.f;
    bool dead = false;

    int nextCallbackId = 1;
    std::map<int, std::function<void()>> deathCallbacks;

    std::map<int, std::function<void(float)>> damageCallbacks;
    int nextDamageCallbackId = 1;

    void CheckDeath();
};
}  // namespace MyEngine