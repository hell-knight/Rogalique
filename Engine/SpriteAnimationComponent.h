#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"
#include "Vector.h"
#include "EngineAPI.h"

namespace MyEngine
{
    class ENGINE_API SpriteAnimationComponent : public Component
    {
    public:
        SpriteAnimationComponent(GameObject* gameObject);

        void Initialize(const std::string& textureMapName, float framerate);

        void Update(float deltaTime) override;
        void Render() override {}

    private:
        TransformComponent* transform;
        SpriteRendererComponent* renderer;

        std::vector<const sf::Texture*> frames;
        float secondsPerFrame = 0.1f;
        float timer = 0.f;
        int currentFrame = 0;

        Vector2Df lastPosition;
        bool isMoving = false;
    };
}