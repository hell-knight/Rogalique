#include "pch.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Logger.h"

namespace MyEngine {
HealthComponent::HealthComponent(GameObject* gameObject, float newHealth,
                                 float newArmor)
    : Component(gameObject),
      maxHealth(newHealth),
      health(newHealth),
      armor(newArmor) {
    LOG_INFO(gameObject->GetName() + " HealthComponent created (default HP: " +
             std::to_string(maxHealth) + ", armor: " + std::to_string(armor) +
             ")");
}

/*void HealthComponent::SetMaxHealth(float maxHP)
{
    maxHealth = maxHP;
    health = maxHP;
    LOG_INFO(gameObject->GetName() + " max health set to " +
std::to_string(maxHealth));
}

void HealthComponent::SetArmor(float value)
{
    armor = value;
    LOG_INFO(gameObject->GetName() + " armor set to " + std::to_string(armor));
}*/

float HealthComponent::GetHealth() const { return health; }
float HealthComponent::GetMaxHealth() const { return maxHealth; }
float HealthComponent::GetArmor() const { return armor; }
bool HealthComponent::IsDead() const { return dead; }

void HealthComponent::ApplyDamage(float damage) {
    if (dead) return;
    float effective = std::max(0.f, damage - armor);
    health -= effective;
    LOG_INFO(gameObject->GetName() + " took " + std::to_string(effective) +
             " damage, HP: " + std::to_string(health));
    for (auto& pair : damageCallbacks){
        pair.second(effective);
    }
    if (health <= 0.f) {
        health = 0.f;
        CheckDeath();
    }
}

void HealthComponent::Heal(float amount) {
    if (dead) return;
    health = std::min(maxHealth, health + amount);
    LOG_INFO(gameObject->GetName() + " healed by " + std::to_string(amount) +
             ", HP: " + std::to_string(health));
}

void HealthComponent::CheckDeath() {
    if (!dead) {
        dead = true;
        LOG_INFO(gameObject->GetName() + " has died.");
        for (auto& pair : deathCallbacks) pair.second();
    }
}

int HealthComponent::SubscribeOnDeath(std::function<void()> callback) {
    int id = nextCallbackId++;
    deathCallbacks[id] = callback;
    return id;
}

void HealthComponent::UnsubscribeOnDeath(int id) { deathCallbacks.erase(id); }

int HealthComponent::SubscribeOnDamage(std::function<void(float)> callback) {
    int id = nextDamageCallbackId++;
    damageCallbacks[id] = callback;
    return id;
}

void HealthComponent::UnsubscribeOnDamage(int id) { damageCallbacks.erase(id); }

}  // namespace MyEngine