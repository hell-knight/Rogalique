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
#include "AuraComponent.h"

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

    auto* aura =
        gameObject->AddComponent<RogaliqueGame::AuraComponent>(gameObject);

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
    health->SubscribeOnHeal([aura, this](float /*amount*/) {
        const int count = 240;
        const float radius = 80.0f;
        const float minLife = 1.5f;
        const float maxLife = 2.5f;
        const float size = 26.0f;
        const sf::Color color(100, 255, 100, 180);

        std::vector<RogaliqueGame::Particle> newParticles;
        newParticles.reserve(count);
        for (int i = 0; i < count; ++i) {
            RogaliqueGame::Particle p;
            float angle = (static_cast<float>(std::rand()) / RAND_MAX) * 2.0f *
                          3.14159265f;
            float dist = (static_cast<float>(std::rand()) / RAND_MAX) * radius;
            p.position = MyEngine::Vector2Df(std::cos(angle) * dist,
                                             std::sin(angle) * dist);
            p.velocity = MyEngine::Vector2Df(0.0f, 0.0f);
            p.maxLife = minLife + (static_cast<float>(std::rand()) / RAND_MAX) *
                                      (maxLife - minLife);
            p.life = p.maxLife;
            p.color = color;
            p.size = size;
            newParticles.push_back(p);
        }
        aura->Emit(newParticles);
    });
    health->SubscribeOnDeath([this]() { LOG_INFO("Player died."); });

    auto attack = gameObject->AddComponent<MyEngine::AttackComponent>(
        gameObject, 25.f, 150.f, 0.5f);
    
    gameObject->AddComponent<MyEngine::InputAttackComponent>();

    gameObject->AddComponent<MyEngine::CameraShakeComponent>(gameObject, camera, health, attack);
}

MyEngine::GameObject* Player::GetGameObject() { return gameObject; }
}  // namespace RogaliqueGame