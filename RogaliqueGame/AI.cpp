#include "AI.h"
#include "GameWorld.h"
#include <MovementComponent.h>
#include "SpriteDirectionComponent.h"
#include "SpriteAnimationComponent.h"
#include "FollowComponent.h"
#include "SpriteColliderComponent.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "AIAttackComponent.h"
#include "ResourceSystem.h"
#include "SceneManager.h"
#include "ParticleEmitter.h"

namespace RogaliqueGame {
AI::AI(const MyEngine::Vector2Df& position, MyEngine::GameObject* player)
    : Character(position, player, "AI") {
    auto* tex =
        MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("ai",
                                                                         0);
    if (!tex) {
        LOG_ERROR(
            "AI: failed to get texture for AI, object will have no texture.");
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
        return;  // or gameObject->RemoveComponent(this);
    }

    auto renderer =
        gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
    renderer->SetTexture(*tex);
    renderer->SetPixelSize(100, 100);

    auto follower = gameObject->AddComponent<MyEngine::FollowComponent>();
    follower->SetTarget(player);
    follower->SetSpeed(120.f);

    auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
    rigidbody->SetKinematic(false);

    auto collider =
        gameObject->AddComponent<MyEngine::SpriteColliderComponent>();

    auto animator =
        gameObject->AddComponent<MyEngine::SpriteAnimationComponent>();
    animator->Initialize("ai", 6.f);

    auto health = gameObject->AddComponent<MyEngine::HealthComponent>(
        gameObject, 80.f, 5.f);
    health->SubscribeOnDamage([this](float damage) {
        auto pos = gameObject->GetComponent<MyEngine::TransformComponent>()
                       ->GetWorldPosition();
        ParticleEmitter::Create(pos, sf::Color::Red, 15, 15.f, 40.f, 100.f,
                                0.4f, 0.8f, 10.f);
    });
    health->SubscribeOnDeath([this]() {
        LOG_INFO("AI died.");
        auto* scene = RogaliqueGame::SceneManager::Instance()->GetCurrent();
        if (scene) {
            scene->DecrementEnemyCount();
            scene->RemoveSceneObject(gameObject);
        }
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
    });

    auto attack = gameObject->AddComponent<MyEngine::AttackComponent>(
        gameObject, 20.f, 150.f, 1.0f);

    auto aiAttack = gameObject->AddComponent<MyEngine::AIAttackComponent>();
    aiAttack->SetTarget(player);
}

}  // namespace RogaliqueGame