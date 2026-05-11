#include "pch.h"
#include "GameObject.h"
#include "AIAttackComponent.h"
#include "TransformComponent.h"
#include "AttackComponent.h"
#include "Logger.h"

namespace MyEngine {
AIAttackComponent::AIAttackComponent(GameObject* gameObject)
    : Component(gameObject) {
    selfTransform = gameObject->GetComponent<TransformComponent>();
    attack = gameObject->GetComponent<AttackComponent>();
    if (!selfTransform || !attack) {
        LOG_ERROR(
            "AIAttackComponent: missing Transform or Attack component. "
            "Removing.");
        gameObject->RemoveComponent(this);
    }
}

void AIAttackComponent::SetTarget(GameObject* target) {
    if (target) {
        targetGameObject = target->GetWeakPtr();
        auto t = targetGameObject.lock();
        if (t)
            LOG_INFO("AIAttackComponent target set to " + t->GetName());
    } else {
        targetGameObject.reset();
        LOG_WARN("AIAttackComponent: target cleared.");
    }
}

void AIAttackComponent::SetAttackRange(float range) { attackRange = range; }

void AIAttackComponent::Update(float) {
    if (!selfTransform || !attack) return;

    auto target = targetGameObject.lock();
    if (!target) return;

    auto* targetTransform = target->GetComponent<TransformComponent>();
    if (!targetTransform) return;

    Vector2Df diff =
        targetTransform->GetWorldPosition() - selfTransform->GetWorldPosition();
    if (diff.GetLength() <= attackRange) {
        attack->Attack();
    }
}
}  // namespace MyEngine