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
    /*LOG_INFO("SpriteColliderComponent subscribed for " + gameObject->GetName());*/
}
SpriteColliderComponent::~SpriteColliderComponent() {
    PhysicsSystem::Instance()->Unsubscribe(this);
    /*LOG_INFO("SpriteColliderComponent unsubscribed for " +
             (gameObject ? gameObject->GetName() : "unknown"));*/
}

void SpriteColliderComponent::Update(float deltaTime) {
    if (useFixedSize) {
        // The position is relative to the sprite, and the size is fixed
        auto pos = sprite->getPosition();
        bounds =
            sf::FloatRect(pos.x - fixedSize.x / 2.f, pos.y - fixedSize.y / 2.f,
                          fixedSize.x, fixedSize.y);
    } else {
        bounds = sprite->getGlobalBounds();
    }
}

void SpriteColliderComponent::Render() {
    sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));
    rectangle.setPosition(bounds.left, bounds.top);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(4);

    RenderSystem::Instance()->Render(rectangle);
}

void SpriteColliderComponent::SetFixedColliderSize(float width, float height) {
    fixedSize = {width, height};
    useFixedSize = (width > 0 && height > 0);
}

}  // namespace MyEngine