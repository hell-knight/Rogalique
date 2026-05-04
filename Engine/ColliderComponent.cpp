#include "pch.h"
#include "ColliderComponent.h"

namespace MyEngine {
ColliderComponent::ColliderComponent(GameObject* gameObject)
    : Component(gameObject) {}

void ColliderComponent::SetTrigger(bool newIsTrigger) {
    isTrigger = newIsTrigger;
}

int ColliderComponent::SubscribeCollision(
    std::function<void(const Collision&)> onCollisionAction) {
    int id = nextSubscriptionId++;
    onCollisionActions[id] = std::move(onCollisionAction);
    return id;
}

void ColliderComponent::UnsubscribeCollision(int subscriptionId) {
    onCollisionActions.erase(subscriptionId);
}

int ColliderComponent::SubscribeTriggerEnter(
    std::function<void(const Trigger&)> onTriggerEnterAction) {
    int id = nextSubscriptionId++;
    onTriggerEnterActions[id] = std::move(onTriggerEnterAction);
    return id;
}

void ColliderComponent::UnsubscribeTriggerEnter(int subscriptionId) {
    onTriggerEnterActions.erase(subscriptionId);
}

int ColliderComponent::SubscribeTriggerExit(
    std::function<void(const Trigger&)> onTriggerExitAction) {
    int id = nextSubscriptionId++;
    onTriggerExitActions[id] = std::move(onTriggerExitAction);
    return id;
}

void ColliderComponent::UnsubscribeTriggerExit(int subscriptionId) {
    onTriggerExitActions.erase(subscriptionId);
}

void ColliderComponent::OnCollision(const Collision& collision) {
    for (auto& pair : onCollisionActions) {
        pair.second(collision);
    }
}
void ColliderComponent::OnTriggerEnter(const Trigger& trigger) {
    for (auto& pair : onTriggerEnterActions) {
        pair.second(trigger);
    }
}
void ColliderComponent::OnTriggerExit(const Trigger& trigger) {
    for (auto& pair : onTriggerExitActions) {
        pair.second(trigger);
    }
}
}  // namespace MyEngine