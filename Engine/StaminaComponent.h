#pragma once
#include "Component.h"
#include "EngineAPI.h"

namespace MyEngine {
class ENGINE_API StaminaComponent : public Component {
   public:
    StaminaComponent(GameObject* gameObject, float maxStamina = 100.f);

    void Update(float deltaTime) override {}
    void Render() override {}

    float GetStamina() const;
    float GetMaxStamina() const;
    void Restore(float amount);
    void Use(float amount);
    bool HasEnough(float amount) const;

   private:
    float stamina;
    float maxStamina;
};
}  // namespace MyEngine
