#pragma once
#include "Component.h"
#include "InputComponent.h"
#include "DirectionalAnimationComponent.h"

namespace RogaliqueGame {

class PlayerDirectionComponent : public MyEngine::Component {
   public:
    PlayerDirectionComponent(MyEngine::GameObject* gameObject);
    void Update(float deltaTime) override;
    void Render() override {}

   private:
    MyEngine::InputComponent* input = nullptr;
    MyEngine::DirectionalAnimationComponent* dirAnim = nullptr;
};

}  // namespace RogaliqueGame