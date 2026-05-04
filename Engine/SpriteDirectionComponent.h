#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "SpriteRendererComponent.h"
#include "EngineAPI.h"

namespace MyEngine {
class ENGINE_API SpriteDirectionComponent : public Component {
   public:
    SpriteDirectionComponent(GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

   private:
    InputComponent* input;
    SpriteRendererComponent* spriteRenderer;
};
}  // namespace MyEngine