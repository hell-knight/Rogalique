#pragma once
#include "Component.h"
#include <SFML/Window.hpp>
#include "EngineAPI.h"

namespace MyEngine {
class ENGINE_API InputComponent : public Component {
   public:
    InputComponent(GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    float GetHorizontalAxis() const;
    float GetVerticalAxis() const;

    bool IsAttackPressed() const;

   private:
    float horizontalAxis = 0.f;
    float verticalAxis = 0.f;

    bool attackPressed = false;
};
}  // namespace MyEngine