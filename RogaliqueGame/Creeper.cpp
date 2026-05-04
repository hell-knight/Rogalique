#include "Creeper.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "AIAttackComponent.h"
#include "FollowComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteAnimationComponent.h"

namespace RogaliqueGame {
Creeper::Creeper(const MyEngine::Vector2Df& position,
                 MyEngine::GameObject* player)
    : Character(position, player, "Creeper") {
    // using an AI texture for demonstration purposes (you can replace it with
    // “creeper”)
    auto* tex =
        MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("ai",
                                                                         0);
    if (!tex) {
        LOG_ERROR(
            "Creeper: failed to get texture for AI, object will have no "
            "texture.");
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
        return;
    }

    auto renderer =
        gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
    renderer->SetTexture(*tex);
    renderer->SetPixelSize(80, 80);

    auto follower = gameObject->AddComponent<MyEngine::FollowComponent>();
    follower->SetTarget(player);
    follower->SetSpeed(80.f);

    auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
    rigidbody->SetKinematic(false);

    auto collider =
        gameObject->AddComponent<MyEngine::SpriteColliderComponent>();

    auto animator =
        gameObject->AddComponent<MyEngine::SpriteAnimationComponent>();
    animator->Initialize("ai", 6.f);

    auto health = gameObject->AddComponent<MyEngine::HealthComponent>(
        gameObject, 50.f, 0.f);
    health->SubscribeOnDeath([this]() {
        LOG_INFO("Creeper died.");
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
    });

    auto attack = gameObject->AddComponent<MyEngine::AttackComponent>(
        gameObject, 15.f, 120.f, 1.5f);

    auto aiAttack = gameObject->AddComponent<MyEngine::AIAttackComponent>();
    aiAttack->SetTarget(player);
}
}  // namespace RogaliqueGame