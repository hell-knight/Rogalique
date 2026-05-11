#include "BossAIComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "SpriteRendererComponent.h"
#include "GameWorld.h"
#include "Logger.h"
#include "ParticleEmitter.h"
#include <cmath>

namespace RogaliqueGame {

BossAIComponent::BossAIComponent(MyEngine::GameObject* gameObject,
                                 MyEngine::GameObject* player)
    : Component(gameObject), m_player(player->GetWeakPtr()), rng(std::random_device{}()) {
    transform = gameObject->GetComponent<MyEngine::TransformComponent>();
    rigidbody = gameObject->GetComponent<MyEngine::RigidbodyComponent>();
    health = gameObject->GetComponent<MyEngine::HealthComponent>();
    attackComp = gameObject->GetComponent<MyEngine::AttackComponent>();
    spriteRenderer =
        gameObject->GetComponent<MyEngine::SpriteRendererComponent>();
}

void BossAIComponent::SetState(State newState) {
    if (currentState == newState) return;
    currentState = newState;
    stateTimer = 0.0f;
}

void BossAIComponent::Update(float deltaTime) {
    if (!health || health->IsDead()) {
        SetState(DEAD);
        return;
    }

    auto player = m_player.lock();
    if (!player) {
        return;
    }

    auto playerTransform = player->GetComponent<MyEngine::TransformComponent>();
    if (!playerTransform) return;

    // Sprite facing the player (always)
    if (spriteRenderer && playerTransform) {
        MyEngine::Vector2Df dir =
            playerTransform->GetWorldPosition() - transform->GetWorldPosition();
        spriteRenderer->FlipX(dir.x < 0);
    }

    if (attackCooldownTimer > 0.0f) attackCooldownTimer -= deltaTime;

    switch (currentState) {
        case IDLE:
            UpdateIdle(deltaTime);
            break;
        case CHASING:
            UpdateChasing(deltaTime);
            break;
        case ATTACK_CHARGE:
            UpdateAttackCharge(deltaTime);
            break;
        case ATTACK_SHOCKWAVE:
            UpdateAttackShockwave(deltaTime);
            break;
        case ATTACK_TELEPORT:
            UpdateAttackTeleport(deltaTime);
            break;
        case DEAD:
            break;
    }
}

void BossAIComponent::UpdateIdle(float dt) {
    // just switch to pursuit mode
    SetState(CHASING);
}

void BossAIComponent::UpdateChasing(float dt) {
    auto player = m_player.lock();
    if (!player) {
        return;
    }
    auto playerTransform = player->GetComponent<MyEngine::TransformComponent>();
    if (!playerTransform) return;

    MyEngine::Vector2Df myPos = transform->GetWorldPosition();
    MyEngine::Vector2Df targetPos = playerTransform->GetWorldPosition();
    MyEngine::Vector2Df dir = targetPos - myPos;
    float dist = dir.GetLength();

    // Moving toward the player
    if (dist > 60.0f) {             // If it's far away, let's go over there
        dir = dir * (1.0f / dist);  // normalization
        transform->MoveBy(dir * moveSpeed * dt);
    }

    // Assessing the likelihood of an attack
    if (attackCooldownTimer <= 0.0f && dist < 400.0f) {
        // Selecting a random attack
        std::uniform_int_distribution<int> rollDist(0, 99);
        int roll = rollDist(rng);
        if (roll < 50) {
            // Normal Attack (50%)
            if (attackComp) attackComp->Attack();
        } else{
            // One of three random abilities
            int skill = roll % 3;
                if (skill == 0) {
                    SetState(ATTACK_CHARGE);
                    ExecuteCharge();
                } else if (skill == 1){
                    SetState(ATTACK_SHOCKWAVE);
                    ExecuteShockwave();
                } else {
                    SetState(ATTACK_TELEPORT);
                    ExecuteTeleport();
                }
        }
        attackCooldownTimer = attackCooldown;
    }
}

void BossAIComponent::UpdateAttackCharge(float dt) {
    auto player = m_player.lock();
    if (!player) {
        return;
    }
    auto playerTransform = player->GetComponent<MyEngine::TransformComponent>();
    if (!playerTransform) return;
    stateTimer += dt;
    // Moving in the same direction
    transform->MoveBy(chargeDirection * chargeSpeed * dt);

    // Checking for a hit on a player
    MyEngine::Vector2Df myPos = transform->GetWorldPosition();
    MyEngine::Vector2Df playerPos = playerTransform->GetWorldPosition();
    float dist = (playerPos - myPos).GetLength();

    if (dist < 150.0f) {
        auto* playerHealth = player->GetComponent<MyEngine::HealthComponent>();
        if (playerHealth) {
            playerHealth->ApplyDamage(30.0f);
            ParticleEmitter::Create(myPos, sf::Color::Red, 10, 10.f, 30.f, 60.f,
                                    0.2f, 0.5f, 4.f);
        }
        SetState(CHASING);
        return;
    }

    // If time runs out, just finish the dash without taking any damage
    if (stateTimer >= chargeDuration) {
        SetState(CHASING);
    }
}

void BossAIComponent::UpdateAttackShockwave(float dt) {
    stateTimer += dt;
    if (stateTimer >= shockwaveDelay) {
        // Deals damage to all targets within a radius of
        MyEngine::Vector2Df myPos = transform->GetWorldPosition();
        auto allObjects = MyEngine::GameWorld::Instance()->GetAllGameObjects();
        for (auto* obj : allObjects) {
            if (obj == gameObject) continue;
            auto* otherHealth = obj->GetComponent<MyEngine::HealthComponent>();
            if (!otherHealth || otherHealth->IsDead()) continue;
            auto* otherTrans =
                obj->GetComponent<MyEngine::TransformComponent>();
            if (!otherTrans) continue;
            float d = (otherTrans->GetWorldPosition() - myPos).GetLength();
            if (d <= shockwaveRadius) {
                otherHealth->ApplyDamage(shockwaveDamage);
            }
        }
        // Visual effect
        ParticleEmitter::Create(myPos, sf::Color(255, 100, 100), 150, 150.0f,
                                80.0f, 200.0f, 0.9f, 1.7f, 26.0f);
        SetState(CHASING);
    }
}

void BossAIComponent::UpdateAttackTeleport(float dt) {
    auto player = m_player.lock();
    if (!player) {
        return;
    }
    auto playerTransform = player->GetComponent<MyEngine::TransformComponent>();
    if (!playerTransform) return;
    // Instant execution on entry
    if (stateTimer == 0.0f) {
        // Teleport to a random location near the player
        MyEngine::Vector2Df playerPos = playerTransform->GetWorldPosition();
        std::uniform_real_distribution<float> angleDist(0.0f,
                                                        2.0f * 3.14159265f);
        float angle = angleDist(rng);
        float dist = 150.0f;  // fixed distance
        MyEngine::Vector2Df newPos =
            playerPos +
            MyEngine::Vector2Df(cosf(angle) * dist, sinf(angle) * dist);
        transform->SetWorldPosition(newPos);
        // Effect
        ParticleEmitter::Create(transform->GetWorldPosition(),
                                sf::Color(200, 100, 255), 20, 5.0f, 40.0f,
                                80.0f, 0.7f, 0.9f, 24.0f);
    }
    stateTimer += dt;
    if (stateTimer > 0.3f) {
        SetState(CHASING);
    }
}

void BossAIComponent::ExecuteCharge() {
    auto player = m_player.lock();
    if (!player) {
        return;
    }
    auto playerTransform = player->GetComponent<MyEngine::TransformComponent>();
    if (!playerTransform) return;
    // keep the player in our sights
    MyEngine::Vector2Df dir =
        playerTransform->GetWorldPosition() - transform->GetWorldPosition();
    float len = dir.GetLength();
    if (len > 1.0f) dir = dir * (1.0f / len);
    chargeDirection = dir;
    stateTimer = 0.0f;
}

void BossAIComponent::ExecuteShockwave() {
    stateTimer = 0.0f;
    // The deterrent effect can be seen right away
    ParticleEmitter::Create(transform->GetWorldPosition(),
                            sf::Color(255, 200, 0), 10, 20.0f, 0.0f, 0.0f, 0.3f,
                            0.5f, 8.0f);
}

void BossAIComponent::ExecuteTeleport() {
    // The teleport will be executed immediately in UpdateAttackTeleport on the first frame
    stateTimer = 0.0f;
}

}  // namespace RogaliqueGame