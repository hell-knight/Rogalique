#pragma once
#include "Component.h"
#include "FollowComponent.h"
#include "DirectionalAnimationComponent.h"
#include "TransformComponent.h"
#include "EngineAPI.h"

namespace MyEngine {

class ENGINE_API FollowDirectionComponent : public Component {
   public:
    FollowDirectionComponent(GameObject* gameObject);
    void Update(float deltaTime) override;
    void Render() override {}

   private:
    FollowComponent* follow = nullptr;
    DirectionalAnimationComponent* dirAnim = nullptr;
    TransformComponent* selfTransform = nullptr;
};

}  // namespace MyEngine