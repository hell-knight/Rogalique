#include "pch.h"
#include "CameraShakeComponent.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include <cstdlib>

namespace MyEngine {
CameraShakeComponent::CameraShakeComponent(GameObject* gameObject,
                                           CameraComponent* camera,
                                           HealthComponent* health,
                                           AttackComponent* attack)
    : Component(gameObject), camera(camera), health(health), attack(attack) {
    if (!camera || !health) {
        LOG_ERROR(
            "CameraShakeComponent: requires CameraComponent and "
            "HealthComponent. Removing.");
        gameObject->RemoveComponent(this);
        return;
    }
    previousHealth = health->GetHealth();

    // Subscribe to receive updates
    damageSubId = health->SubscribeOnDamage([this](float damage) {
        if (damage > 0.f) {
            StartShake(16.f, 0.2f);  // intensity and duration to deal damage
        }
    });

    // Handler for a successful player attack (if an AttackComponent is passed)
    if (attack) {
        attackSubId =
            attack->SubscribeOnAttack([this]() { StartShake(8.f, 0.1f); });
    }
}

CameraShakeComponent::~CameraShakeComponent() {
    // Safely unsubscribe from events
    if (auto* health = gameObject->GetComponent<HealthComponent>()) {
        health->UnsubscribeOnDamage(damageSubId);
        // If there were a “unsubscribe” option for death, we'd unsubscribe from that too, but there isn't one
    }
    if (auto* attack = gameObject->GetComponent<AttackComponent>()) {
        attack->UnsubscribeOnAttack(attackSubId);
    }
}

void CameraShakeComponent::Update(float deltaTime) {
    if (isShaking) {
        shakeTimer -= deltaTime;
        if (shakeTimer <= 0.f) {
            isShaking = false;
            camera->SetShakeOffset(Vector2Df(0.f, 0.f));
            return;
        }
        // Generate a random offset that decays toward the end
        float ratio = shakeTimer / shakeDuration;
        float intensity = shakeIntensity * ratio;
        float offsetX =
            (static_cast<float>(std::rand()) / RAND_MAX * 2.f - 1.f) *
            intensity;
        float offsetY =
            (static_cast<float>(std::rand()) / RAND_MAX * 2.f - 1.f) *
            intensity;
        camera->SetShakeOffset(Vector2Df(offsetX, offsetY));
    }
}

void CameraShakeComponent::StartShake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeDuration = duration;
    shakeTimer = duration;
    isShaking = true;
}


}  // namespace MyEngine
