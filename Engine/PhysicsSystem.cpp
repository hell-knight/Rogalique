#include "pch.h"
#include "PhysicsSystem.h"

namespace MyEngine {

PhysicsSystem* PhysicsSystem::Instance() {
    static PhysicsSystem physicsSystem;
    return &physicsSystem;
}

float PhysicsSystem::GetFixedDeltaTime() const { return fixedDeltaTime; }

void PhysicsSystem::Update() {
    ProcessColliders();
    ProcessTriggerExits();
}

void PhysicsSystem::ProcessColliders() {
    for (size_t colliderIndex = 0; colliderIndex < colliders.size();
         ++colliderIndex) {
        auto* rigidbody = colliders[colliderIndex]
                              ->GetGameObject()
                              ->GetComponent<RigidbodyComponent>();

        // Ignore colliders without a rigid body or those set to kinematic
        if (!rigidbody || rigidbody->GetKinematic()) continue;

        for (size_t otherIndex = 0; otherIndex < colliders.size();
             ++otherIndex) {
            if (otherIndex == colliderIndex) continue;

            sf::FloatRect intersection;
            if (colliders[colliderIndex]->bounds.intersects(
                    colliders[otherIndex]->bounds, intersection)) {
                ProcessCollisionPair(colliders[colliderIndex],
                                     colliders[otherIndex], intersection);
            }
        }
    }
}

void PhysicsSystem::ProcessCollisionPair(ColliderComponent* colliderA,
                                         ColliderComponent* colliderB,
                                         const sf::FloatRect& intersection) {
    // One is a trigger and the other is not → trigger enter logic
    if (colliderA->isTrigger != colliderB->isTrigger) {
        auto triggerPair = std::make_pair(colliderA, colliderB);
        if (triggersEnteredSet.find(triggerPair) == triggersEnteredSet.end()) {
            Trigger triggerEnter(colliderA, colliderB);
            colliderA->OnTriggerEnter(triggerEnter);
            colliderB->OnTriggerEnter(triggerEnter);

            triggersEnteredSet.insert(triggerPair);
            LOG_INFO("Trigger entered between '" +
                     colliderA->GetGameObject()->GetName() + "' and '" +
                     colliderB->GetGameObject()->GetName() + "'");
        }
        return;
    }

    // Both are non-trigger colliders → resolve physical collision
    if (!colliderA->isTrigger) {
        ResolveCollision(colliderA, colliderB, intersection);
    }
}

void PhysicsSystem::ResolveCollision(ColliderComponent* colliderA,
                                     ColliderComponent* colliderB,
                                     const sf::FloatRect& intersection) {
    float intersectionWidth = intersection.width;
    float intersectionHeight = intersection.height;
    Vector2Df intersectionPosition = {
        intersection.left - 0.5f * intersectionWidth,
        intersection.top - 0.5f * intersectionHeight};

    Vector2Df colliderAPosition = {colliderA->bounds.left,
                                   colliderA->bounds.top};
    auto colliderATransform =
        colliderA->GetGameObject()->GetComponent<TransformComponent>();

    // Determine collision direction and push colliderA out of colliderB
    std::string collisionDir;
    if (intersectionWidth > intersectionHeight) {
        if (intersectionPosition.y > colliderAPosition.y) {
            colliderATransform->MoveBy({0, -intersectionHeight});
            collisionDir = "Top";
        } else {
            colliderATransform->MoveBy({0, intersectionHeight});
            collisionDir = "Down";
        }
    } else {
        if (intersectionPosition.x > colliderAPosition.x) {
            colliderATransform->MoveBy({-intersectionWidth, 0.f});
            collisionDir = "Right";
        } else {
            colliderATransform->MoveBy({intersectionWidth, 0.f});
            collisionDir = "Left";
        }
    }

    Collision collision(colliderA, colliderB, intersection);
    colliderA->OnCollision(collision);
    colliderB->OnCollision(collision);

    LOG_INFO("Collision (" + collisionDir + ") between '" +
             colliderA->GetGameObject()->GetName() + "' and '" +
             colliderB->GetGameObject()->GetName() + "'");
}

void PhysicsSystem::ProcessTriggerExits() {
    std::vector<std::pair<ColliderComponent*, ColliderComponent*>> exitedPairs;
    for (const auto& triggerPair : triggersEnteredSet) {
        if (!triggerPair.first->bounds.intersects(triggerPair.second->bounds)) {
            Trigger triggerExit(triggerPair.first, triggerPair.second);
            triggerPair.first->OnTriggerExit(triggerExit);
            triggerPair.second->OnTriggerExit(triggerExit);

            exitedPairs.push_back(triggerPair);
            LOG_INFO("Trigger exited between '" +
                     triggerPair.first->GetGameObject()->GetName() + "' and '" +
                     triggerPair.second->GetGameObject()->GetName() + "'");
        }
    }
    for (const auto& exitedPair : exitedPairs) {
        triggersEnteredSet.erase(exitedPair);
    }
}

void PhysicsSystem::Subscribe(ColliderComponent* collider) {
    std::cout << "Subscribe " << collider << std::endl;
    colliders.push_back(collider);
}

void PhysicsSystem::Unsubscribe(ColliderComponent* collider) {
    std::cout << "Unsubscribe " << collider << std::endl;

    std::vector<std::pair<ColliderComponent*, ColliderComponent*>> toRemove;
    for (const auto& p : triggersEnteredSet) {
        if (p.first == collider || p.second == collider) toRemove.push_back(p);
    }
    for (const auto& p : toRemove) {
        triggersEnteredSet.erase(p);
    }

    colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
                                   [collider](ColliderComponent* obj) {
                                       return obj == collider;
                                   }),
                    colliders.end());
}

}  // namespace MyEngine