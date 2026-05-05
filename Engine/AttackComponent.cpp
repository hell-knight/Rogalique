#include "pch.h"
#include "GameObject.h"
#include "AttackComponent.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "Logger.h"

namespace MyEngine {
AttackComponent::AttackComponent(GameObject* gameObject, float newDamage,
                                 float newRadius, float newCooldown)
    : Component(gameObject),
      damage(newDamage),
      radius(newRadius),
      cooldown(newCooldown) {
    LOG_INFO(gameObject->GetName() + " AttackComponent created (default dmg=" +
             std::to_string(damage) + " radius=" + std::to_string(radius) +
             " cd=" + std::to_string(cooldown) + ")");
}

void AttackComponent::SetDamage(float dmg) { damage = dmg; }
void AttackComponent::SetRadius(float rad) { radius = rad; }
void AttackComponent::SetCooldown(float cd) { cooldown = cd; }

void AttackComponent::Update(float deltaTime) {
    if (timeSinceLastAttack < cooldown) timeSinceLastAttack += deltaTime;
}

bool AttackComponent::Attack() {
    if (timeSinceLastAttack < cooldown) return false;

    auto* target = FindTargetInRange();
    if (target) {
        target->ApplyDamage(damage);
        LOG_INFO(gameObject->GetName() + " attacks " +
                 target->GetGameObject()->GetName() + " for " +
                 std::to_string(damage) + " damage.");
        timeSinceLastAttack = 0.f;
        for (const auto& pair : attackCallbacks) {
            pair.second();
        }
        return true;
    }
    return false;
}

int AttackComponent::SubscribeOnAttack(std::function<void()> callback) {
    int id = nextAttackCallbackId++;
    attackCallbacks[id] = callback;
    return id;
}

void AttackComponent::UnsubscribeOnAttack(int id) { attackCallbacks.erase(id); }

HealthComponent* AttackComponent::FindTargetInRange() const {
    auto selfPos =
        gameObject->GetComponent<TransformComponent>()->GetWorldPosition();
    HealthComponent* closest = nullptr;
    float closestDist = radius;

    for (auto obj : GameWorld::Instance()->GetAllGameObjects()) {
        if (obj == gameObject) continue;
        auto health = obj->GetComponent<HealthComponent>();
        if (!health || health->IsDead()) continue;

        auto targetPos =
            obj->GetComponent<TransformComponent>()->GetWorldPosition();
        float dist = (targetPos - selfPos).GetLength();
        if (dist < closestDist) {
            closestDist = dist;
            closest = health;
        }
    }
    return closest;
}
}  // namespace MyEngine