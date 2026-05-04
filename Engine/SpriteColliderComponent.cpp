#include "pch.h"
#include "SpriteColliderComponent.h"

namespace MyEngine {
SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject)
    : ColliderComponent(gameObject) {
    auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
    if (!spriteRenderer) {
        LOG_ERROR("SpriteColliderComponent requires SpriteRenderer. Removing.");
        gameObject->RemoveComponent(this);
        return;
    }

    sprite = gameObject->GetComponent<SpriteRendererComponent>()->GetSprite();
    PhysicsSystem::Instance()->Subscribe(this);
    LOG_INFO("SpriteColliderComponent subscribed for " + gameObject->GetName());
}
SpriteColliderComponent::~SpriteColliderComponent() {
    PhysicsSystem::Instance()->Unsubscribe(this);
    LOG_INFO("SpriteColliderComponent unsubscribed for " +
             (gameObject ? gameObject->GetName() : "unknown"));
}

void SpriteColliderComponent::Update(float deltaTime) {
    bounds = sprite->getGlobalBounds();
}
void SpriteColliderComponent::Render() {
    sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));
    rectangle.setPosition(bounds.left, bounds.top);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(4);

    RenderSystem::Instance()->Render(rectangle);
}
}  // namespace MyEngine