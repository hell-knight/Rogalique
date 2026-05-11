#pragma once
#include "Component.h"
#include "Vector.h"
#include <random>

namespace MyEngine {
class GameObject;
class TransformComponent;
class RigidbodyComponent;
class HealthComponent;
class AttackComponent;
class AIAttackComponent;
class SpriteRendererComponent;
}  // namespace MyEngine

namespace RogaliqueGame {
class BossAIComponent : public MyEngine::Component {
   public:
    enum State {
        IDLE,
        CHASING,
        ATTACK_CHARGE,
        ATTACK_SHOCKWAVE,
        ATTACK_TELEPORT,
        DEAD
    };

    BossAIComponent(MyEngine::GameObject* gameObject,
                    MyEngine::GameObject* player);
    void Update(float deltaTime) override;
    void Render() override {}

   private:
    void SetState(State newState);
    void UpdateIdle(float dt);
    void UpdateChasing(float dt);
    void UpdateAttackCharge(float dt);
    void UpdateAttackShockwave(float dt);
    void UpdateAttackTeleport(float dt);

    void ExecuteCharge();
    void ExecuteShockwave();
    void ExecuteTeleport();

    MyEngine::GameObject* player;
    MyEngine::TransformComponent* transform;
    MyEngine::TransformComponent* playerTransform;
    MyEngine::RigidbodyComponent* rigidbody;
    MyEngine::HealthComponent* health;
    MyEngine::AttackComponent* attackComp;
    MyEngine::SpriteRendererComponent* spriteRenderer = nullptr;

    State currentState = IDLE;
    float stateTimer = 0.0f;
    float attackCooldown = 3.0f;
    float attackCooldownTimer = 0.0f;

    float moveSpeed = 80.0f;
    float chargeSpeed = 300.0f;
    float chargeDuration = 1.5f;
    MyEngine::Vector2Df chargeDirection;

    float shockwaveDelay = 1.0f;
    float shockwaveRadius = 250.0f;
    float shockwaveDamage = 35.0f;

    float teleportRange = 200.0f;

    std::mt19937 rng;
};
}  // namespace RogaliqueGame