#pragma once
#include "ColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include <SFML/Graphics/Sprite.hpp>
#include "EngineAPI.h"

namespace MyEngine {
class ENGINE_API SpriteColliderComponent : public ColliderComponent {
   public:
    SpriteColliderComponent(GameObject* gameObject);
    ~SpriteColliderComponent();

    void Update(float deltaTime) override;
    void Render() override;

    // // Set a fixed collider size (in pixels). If (0,0), the sprite's size is used.
    void SetFixedColliderSize(float width, float height);

   private:
    const sf::Sprite* sprite;
    sf::Vector2f fixedSize;     // // {0,0} means “auto”
    bool useFixedSize = false;
};
}  // namespace MyEngine