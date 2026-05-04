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
    for (size_t i = 0; i < colliders.size(); ++i) {
        auto* body =
            colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();

        // Ignore colliders without a rigid body or those set to kinematic
        if (!body || body->GetKinematic()) continue;

        for (size_t j = 0; j < colliders.size(); ++j) {
            if (j == i) continue;

            sf::FloatRect intersection;
            if (colliders[i]->bounds.intersects(colliders[j]->bounds,
                                                intersection)) {
                ProcessCollisionPair(colliders[i], colliders[j], intersection);
            }
        }
    }
}

void PhysicsSystem::ProcessCollisionPair(ColliderComponent* a,
                                         ColliderComponent* b,
                                         const sf::FloatRect& intersection) {
    // One is a trigger and the other is not → trigger enter logic
    if (a->isTrigger != b->isTrigger) {
        auto pair = std::make_pair(a, b);
        if (triggersEnteredSet.find(pair) == triggersEnteredSet.end()) {
            Trigger triggerEnter(a, b);
            a->OnTriggerEnter(triggerEnter);
            b->OnTriggerEnter(triggerEnter);

            triggersEnteredSet.insert(pair);
            LOG_INFO("Trigger entered between '" +
                     a->GetGameObject()->GetName() + "' and '" +
                     b->GetGameObject()->GetName() + "'");
        }
        return;
    }

    // Both are non-trigger colliders → resolve physical collision
    if (!a->isTrigger) {
        ResolveCollision(a, b, intersection);
    }
}

void PhysicsSystem::ResolveCollision(ColliderComponent* a, ColliderComponent* b,
                                     const sf::FloatRect& intersection) {
    float intersectionWidth = intersection.width;
    float intersectionHeight = intersection.height;
    Vector2Df intersectionPosition = {
        intersection.left - 0.5f * intersectionWidth,
        intersection.top - 0.5f * intersectionHeight};

    Vector2Df aPosition = {a->bounds.left, a->bounds.top};
    auto aTransform = a->GetGameObject()->GetComponent<TransformComponent>();

    // Determine collision direction and push a out of b
    std::string collisionDir;
    if (intersectionWidth > intersectionHeight) {
        if (intersectionPosition.y > aPosition.y) {
            aTransform->MoveBy({0, -intersectionHeight});
            collisionDir = "Top";
        } else {
            aTransform->MoveBy({0, intersectionHeight});
            collisionDir = "Down";
        }
    } else {
        if (intersectionPosition.x > aPosition.x) {
            aTransform->MoveBy({-intersectionWidth, 0.f});
            collisionDir = "Right";
        } else {
            aTransform->MoveBy({intersectionWidth, 0.f});
            collisionDir = "Left";
        }
    }

    Collision collision(a, b, intersection);
    a->OnCollision(collision);
    b->OnCollision(collision);

    LOG_INFO("Collision (" + collisionDir + ") between '" +
             a->GetGameObject()->GetName() + "' and '" +
             b->GetGameObject()->GetName() + "'");
}

void PhysicsSystem::ProcessTriggerExits() {
    std::vector<std::pair<ColliderComponent*, ColliderComponent*>> exitedPairs;
    for (const auto& p : triggersEnteredSet) {
        if (!p.first->bounds.intersects(p.second->bounds)) {
            Trigger triggerExit(p.first, p.second);
            p.first->OnTriggerExit(triggerExit);
            p.second->OnTriggerExit(triggerExit);

            exitedPairs.push_back(p);
            LOG_INFO("Trigger exited between '" +
                     p.first->GetGameObject()->GetName() + "' and '" +
                     p.second->GetGameObject()->GetName() + "'");
        }
    }
    for (const auto& p : exitedPairs) {
        triggersEnteredSet.erase(p);
    }
}

void PhysicsSystem::Subscribe(ColliderComponent* collider) {
    std::cout << "Subscribe " << collider << std::endl;
    colliders.push_back(collider);
}

void PhysicsSystem::Unsubscribe(ColliderComponent* collider) {
    std::cout << "Unsubscribe " << collider << std::endl;
    colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
                                   [collider](ColliderComponent* obj) {
                                       return obj == collider;
                                   }),
                    colliders.end());
}

}  // namespace MyEngine