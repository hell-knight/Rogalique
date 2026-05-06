#include "pch.h"
#include "LevelExit.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "Logger.h"

namespace RogaliqueGame {
LevelExit::LevelExit(const MyEngine::Vector2Df& position,
                     std::function<void()> onPlayerEnter) {
    gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("LevelExit");

    auto* tf = gameObject->GetComponent<MyEngine::TransformComponent>();
    tf->SetWorldPosition(position);

    static sf::Texture dummyTex;
    static bool dummyLoaded = false;
    if (!dummyLoaded) {
        sf::Image img;
        img.create(1, 1, sf::Color::White);
        dummyTex.loadFromImage(img);
        dummyLoaded = true;
    }

    auto* renderer =
        gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
    renderer->SetTexture(dummyTex);
    renderer->SetPixelSize(128, 128);

    auto* rigid = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
    rigid->SetKinematic(true);

    auto* collider =
        gameObject->AddComponent<MyEngine::SpriteColliderComponent>();
    collider->SetTrigger(true);

    triggerSubId = collider->SubscribeTriggerEnter([this, onPlayerEnter](const MyEngine::Trigger& trigger) {
        // check that the other object is a player
            MyEngine::GameObject* other = trigger.second->GetGameObject();
            if (other->GetName() == "Player") {
                LOG_INFO("Player entered exit trigger!");
                onPlayerEnter();
            }
        });
}
}  // namespace RogaliqueGame