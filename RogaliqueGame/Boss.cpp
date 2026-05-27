#include "Boss.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "BossAIComponent.h"
#include "SceneManager.h"
#include <SpriteAnimationComponent.h>
#include "ParticleEmitter.h"
#include <DirectionalAnimationComponent.h>
#include "FollowDirectionComponent.h"

namespace RogaliqueGame {
Boss::Boss(const MyEngine::Vector2Df& position, MyEngine::GameObject* player)
    : Character(position, player, "Boss") {
    auto* tex =
        MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared(
            "boss_dir", 0);
    if (!tex) {
        LOG_ERROR("Boss texture not found, using AI texture");
        tex = MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared(
            "ai", 0);
    }
    auto* renderer =
        gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
    renderer->SetTexture(*tex);
    renderer->SetPixelSize(256.f, 256.f);

    auto* rigid = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
    rigid->SetKinematic(true);  // doesn't push the player, but blocks them

    auto collider = gameObject->AddComponent<MyEngine::SpriteColliderComponent>();
    collider->SetFixedColliderSize(128.f, 128.f);

    auto animator =
        gameObject->AddComponent<MyEngine::DirectionalAnimationComponent>(gameObject, 4);
    animator->Initialize("boss_dir", 8.f);

    auto* health = gameObject->AddComponent<MyEngine::HealthComponent>(
        gameObject, 200.0f, 10.0f);
    health->SubscribeOnDamage([this](float damage) {
        auto pos = gameObject->GetComponent<MyEngine::TransformComponent>()
                       ->GetWorldPosition();
        ParticleEmitter::Create(pos, sf::Color::Red, 15, 15.f, 40.f, 100.f,
                                0.4f, 0.8f, 10.f);
    });
    health->SubscribeOnDeath([this]() {
        LOG_INFO("Boss defeated!");
        auto* scene = RogaliqueGame::SceneManager::Instance()->GetCurrent();
        if (scene) {
            scene->DecrementEnemyCount();
            scene->RemoveSceneObject(gameObject);
        }
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
    });
    auto* attack = gameObject->AddComponent<MyEngine::AttackComponent>(
        gameObject, 30.0f, 150.0f, 2.0f);

    gameObject->AddComponent<BossAIComponent>(gameObject, player);
}
}  // namespace RogaliqueGame