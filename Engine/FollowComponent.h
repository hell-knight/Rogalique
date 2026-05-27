#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "EngineAPI.h"
#include <memory>

namespace MyEngine {
class ENGINE_API FollowComponent : public Component {
   public:
    FollowComponent(GameObject* gameObject);
    void Update(float deltaTime) override;
    void Render() override;

    void SetTarget(GameObject* targetObject);
    void SetSpeed(float newSpeed);
    float GetSpeed() const;

    void SetSkipFrames(int frames) { skipFrames = frames; }

    MyEngine::GameObject* GetTarget() const;

   private:
    TransformComponent* transform = nullptr;
    std::weak_ptr<GameObject> targetGameObject;
    SpriteRendererComponent* spriteRenderer = nullptr;

    float speed = 100.f;
    int skipFrames = 0;
};

}  // namespace MyEngine