#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"
#include "Vector.h"
#include "EngineAPI.h"
#include <vector>

namespace MyEngine {
class ENGINE_API DirectionalAnimationComponent : public Component {
   public:
    DirectionalAnimationComponent(GameObject* gameObject,
                                  int numDirections = 4);

    void Initialize(const std::string& textureMapName, float framerate);
    void SetMoveDirection(
        const Vector2Df& dir);  // transfer the momentum vector
    void Update(float deltaTime) override;
    void Render() override {}

   private:
    TransformComponent* transform = nullptr;
    SpriteRendererComponent* renderer = nullptr;
    int numDirections;
    int framesPerRow = 0;

    std::vector<const sf::Texture*> allFrames;  // all images in a single row
    float secondsPerFrame = 0.1f;
    float timer = 0.f;
    int currentFrame = 0;
    int currentDirection = 0;
    bool isMoving = false;
};
}
