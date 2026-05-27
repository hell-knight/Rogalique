#include "pch.h"
#include "FollowDirectionComponent.h"

namespace MyEngine {
FollowDirectionComponent::FollowDirectionComponent(GameObject* gameObject) : Component(gameObject) {
    follow = gameObject->GetComponent<FollowComponent>();
    dirAnim = gameObject->GetComponent<DirectionalAnimationComponent>();
    selfTransform = gameObject->GetComponent<TransformComponent>();

    if (!follow || !dirAnim) {
        LOG_ERROR(
            "FollowDirectionComponent requires FollowComponent and "
            "DirectionalAnimationComponent. Removing.");
        gameObject->RemoveComponent(this);
    }
}

void FollowDirectionComponent::Update(float deltaTime) {
    if (!follow || !dirAnim || !selfTransform) return;

    GameObject* target = follow->GetTarget();
    if (!target) return;

    auto* targetTransform = target->GetComponent<TransformComponent>();
    if (!targetTransform) return;

    Vector2Df dir =
        targetTransform->GetWorldPosition() - selfTransform->GetWorldPosition();
    dirAnim->SetMoveDirection(dir);
}


}  // namespace MyEngine