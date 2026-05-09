#pragma once
#include "Component.h"
#include "HealthComponent.h"
#include <SFML/Window/Keyboard.hpp>

namespace RogaliqueGame {
class HealTestComponent : public MyEngine::Component {
   public:
    HealTestComponent(MyEngine::GameObject* gameObject,
                      MyEngine::HealthComponent* health)
        : Component(gameObject), health(health) {}

    void Update(float) override {
        bool pressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::H);
            
        if (pressedNow && !wasPressed) {
            health->Heal(30.f);
        }
        wasPressed = pressedNow;
    }
    void Render() override {}

   private:
    MyEngine::HealthComponent* health;
    bool wasPressed = false;
};
}  // namespace RogaliqueGame