#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Component.h"
#include "Collision.h"
#include "Trigger.h"
#include "PhysicsSystem.h"
#include "EngineAPI.h"
#include <map>

namespace MyEngine {
class ENGINE_API ColliderComponent : public Component {
   public:
    ColliderComponent(GameObject* gameObject);

    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    void SetTrigger(bool newIsTrigger);

    int SubscribeCollision(
        std::function<void(const Collision&)> onCollisionAction);
    void UnsubscribeCollision(int subscriptionId);

    int SubscribeTriggerEnter(
        std::function<void(const Trigger&)> onTriggerEnterAction);
    void UnsubscribeTriggerEnter(int subscriptionId);

    int SubscribeTriggerExit(
        std::function<void(const Trigger&)> onTriggerExitAction);
    void UnsubscribeTriggerExit(int subscriptionId);

    friend class PhysicsSystem;

   protected:
    sf::FloatRect bounds;
    bool isTrigger = false;

    void OnCollision(const Collision& collision);
    void OnTriggerEnter(const Trigger& trigger);
    void OnTriggerExit(const Trigger& trigger);

   private:
    int nextSubscriptionId = 1;  // Simple ID Generator

    std::map<int, std::function<void(const Collision&)>> onCollisionActions;
    std::map<int, std::function<void(const Trigger&)>> onTriggerEnterActions;
    std::map<int, std::function<void(const Trigger&)>> onTriggerExitActions;
};
}  // namespace MyEngine