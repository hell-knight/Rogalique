#include "pch.h"
#include "FollowComponent.h"
#include "iostream"

namespace MyEngine {
FollowComponent::FollowComponent(GameObject* gameObject)
    : Component(gameObject) {
    transform = gameObject->GetComponent<MyEngine::TransformComponent>();
    spriteRenderer =
        gameObject->GetComponent<MyEngine::SpriteRendererComponent>();
    if (transform == nullptr) {
        LOG_ERROR("FollowComponent requires TransformComponent. Removing.");
        gameObject->RemoveComponent(this);
    }
}

void FollowComponent::Update(float deltaTime) {
    if (transform == nullptr) {
        return;
    }

    if (skipFrames > 0) {
        --skipFrames;
        return;
    }

    auto target = targetGameObject.lock();
    if (!target) return;

    auto* targetTransform = target->GetComponent<TransformComponent>();
    if (!targetTransform) return;

    Vector2Df currentPos = transform->GetWorldPosition();
    Vector2Df targetPos = targetTransform->GetWorldPosition();
    Vector2Df direction = targetPos - currentPos;

    float length = direction.GetLength();

    if (length > 0.001f) {
        Vector2Df normalized =
            Vector2Df(direction.x / length, direction.y / length);
        transform->MoveBy(normalized * speed * deltaTime);

        if (spriteRenderer) {
            spriteRenderer->FlipX(direction.x < 0);
        }
    }
}

void FollowComponent::Render() {}

void FollowComponent::SetTarget(GameObject* targetObject) {
    if (!targetObject) {
        LOG_WARN("FollowComponent: SetTarget called with nullptr.");
        targetGameObject.reset();
        return;
    }
    targetGameObject = targetObject->GetWeakPtr();
    auto locked = targetGameObject.lock();
    if (locked) {
        LOG_INFO("FollowComponent: target set to " + targetObject->GetName());
    } else {
        LOG_ERROR("FollowComponent: failed to lock target weak_ptr.");
    }
}

void FollowComponent::SetSpeed(float newSpeed) { speed = newSpeed; }

float FollowComponent::GetSpeed() const { return speed; }

MyEngine::GameObject* FollowComponent::GetTarget() const { 
    auto locked = targetGameObject.lock();
    return locked ? locked.get() : nullptr; 
}

}  // namespace MyEngine