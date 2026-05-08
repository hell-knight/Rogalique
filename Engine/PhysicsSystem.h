#pragma once
#include <set>
#include <map>
#include <iostream>
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"
#include "EngineAPI.h"

namespace MyEngine {
class ENGINE_API PhysicsSystem {
   public:
    static PhysicsSystem* Instance();

    void Update();

    float GetFixedDeltaTime() const;
    void Subscribe(ColliderComponent* collider);
    void Unsubscribe(ColliderComponent* collider);

    void SetSkipFrames(int frames) { skipFrames = frames; }

   private:
    PhysicsSystem() {}
    ~PhysicsSystem() {}

    PhysicsSystem(PhysicsSystem const&) = delete;
    PhysicsSystem& operator=(PhysicsSystem const&) = delete;

    std::vector<ColliderComponent*> colliders;

    // A new structure for comparing pairs without regard to order
    struct PairCompare {
        bool operator()(
            const std::pair<ColliderComponent*, ColliderComponent*>& lhs,
            const std::pair<ColliderComponent*, ColliderComponent*>& rhs)
            const {
            // reduce it to canonical form: the left index is 1
            auto canonical = [](const std::pair<ColliderComponent*, ColliderComponent*>& p) {
                return (p.first < p.second) ? p : std::make_pair(p.second, p.first);
            };
            auto cl = canonical(lhs);
            auto cr = canonical(rhs);
            if (cl.first != cr.first) return cl.first < cr.first;
            return cl.second < cr.second;
        }
    };

    std::set<std::pair<ColliderComponent*, ColliderComponent*>, PairCompare>
        triggersEnteredSet;

    float fixedDeltaTime = 0.02f;
    int skipFrames = 0;

    void ProcessColliders();
    void ProcessCollisionPair(ColliderComponent* a, ColliderComponent* b,
                              const sf::FloatRect& intersection);
    void ResolveCollision(ColliderComponent* a, ColliderComponent* b,
                          const sf::FloatRect& intersection);
    void ProcessTriggerExits();
};
}  // namespace MyEngine