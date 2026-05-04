#include "pch.h"
#include "StaminaComponent.h"
#include "Logger.h"
#include <algorithm>

namespace MyEngine {
StaminaComponent::StaminaComponent(GameObject* gameObject, float maxStamina)
    : Component(gameObject), maxStamina(maxStamina), stamina(maxStamina) {
    LOG_INFO(gameObject->GetName() + " StaminaComponent created (max: " +
             std::to_string(maxStamina) + ")");
}

float StaminaComponent::GetStamina() const { return stamina; }

float StaminaComponent::GetMaxStamina() const { return maxStamina; }

void StaminaComponent::Restore(float amount) {
    if (amount <= 0) return;
    stamina = std::min(maxStamina, stamina + amount);
}

void StaminaComponent::Use(float amount) {
    if (amount <= 0) return;
    stamina = std::max(0.f, stamina - amount);
}

bool StaminaComponent::HasEnough(float amount) const {
    return stamina >= amount;
}

}  // namespace MyEngine