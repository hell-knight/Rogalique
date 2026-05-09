#include "Player.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include <MovementComponent.h>
#include <SpriteDirectionComponent.h>
#include "SpriteAnimationComponent.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "InputAttackComponent.h"
#include "AttackComponent.h"
#include "StaminaComponent.h"
#include "CameraShakeComponent.h"
#include "ParticleEmitter.h"
#include "HealTestComponent.h"

namespace RogaliqueGame {
Player::Player(const MyEngine::Vector2Df& position) {
    gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Player");
    auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
    transform->SetWorldPosition(position);
    LOG_INFO("Player created! GameObject address: " +
             std::to_string((uintptr_t)gameObject));
    auto* tex =
        MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared(
            "player", 0);
    if (!tex) {
        LOG_ERROR("Player: failed to get texture.");
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
        return;
    }

    auto renderer =
        gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
    renderer->SetTexture(*tex);
    renderer->SetPixelSize(100, 100);

    auto camera = gameObject->AddComponent<MyEngine::CameraComponent>();
    camera->SetWindow(&MyEngine::RenderSystem::Instance()->GetMainWindow());
    camera->SetBaseResolution(1280, 720);
    camera->ZoomBy(2.f);

    auto input = gameObject->AddComponent<MyEngine::InputComponent>();

    auto stamina = gameObject->AddComponent<MyEngine::StaminaComponent>(gameObject, 100.f);

    auto movement = gameObject->AddComponent<MyEngine::MovementComponent>();
    movement->SetSpeed(400.f);

    auto spriteDirection =
        gameObject->AddComponent<MyEngine::SpriteDirectionComponent>();

    auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
    rigidbody->SetKinematic(false);

    auto collider =
        gameObject->AddComponent<MyEngine::SpriteColliderComponent>();

    auto animator =
        gameObject->AddComponent<MyEngine::SpriteAnimationComponent>();
    animator->Initialize("player", 6.f);

    auto health = gameObject->AddComponent<MyEngine::HealthComponent>(
        gameObject, 100.f, 10.f);
    gameObject->AddComponent<HealTestComponent>(gameObject, health);
    health->SubscribeOnDamage([this](float damage) {
        auto pos = gameObject->GetComponent<MyEngine::TransformComponent>()
                       ->GetWorldPosition();
        ParticleEmitter::Create(pos, sf::Color::Red, 15, 15.f, 40.f, 100.f,
                                0.4f, 0.9f, 10.f);
    });
    health->SubscribeOnHeal([this](float amount) {
        auto pos = gameObject->GetComponent<MyEngine::TransformComponent>()
                       ->GetWorldPosition();
        RogaliqueGame::ParticleEmitter::Create(pos, sf::Color::Green, 20, 15.f,
                                               30.f, 80.f, 0.4f, 0.9f, 10.f);
    });
    health->SubscribeOnDeath([this]() { LOG_INFO("Player died."); });

    auto attack = gameObject->AddComponent<MyEngine::AttackComponent>(
        gameObject, 25.f, 150.f, 0.5f);
    
    gameObject->AddComponent<MyEngine::InputAttackComponent>();

    gameObject->AddComponent<MyEngine::CameraShakeComponent>(gameObject, camera, health, attack);
}

MyEngine::GameObject* Player::GetGameObject() { return gameObject; }
}  // namespace RogaliqueGame