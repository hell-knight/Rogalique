#pragma once
#include "Component.h"
#include "EngineAPI.h"
#include <SFML/Graphics.hpp>

namespace MyEngine {
class HealthComponent;
class StaminaComponent;

class ENGINE_API HUDRendererComponent : public Component {
   public:
    HUDRendererComponent(GameObject* gameObject, HealthComponent* health, StaminaComponent* stamina,
        const sf::Texture* healthIconTex, const sf::Texture* staminaIconTex, const sf::Texture* armorIconTex);

    void Update(float deltaTime) override {};
    void Render() override;

   private:
    HealthComponent* health;
    StaminaComponent* stamina;

    sf::Sprite healthIcon;
    sf::Sprite staminaIcon;
    sf::Sprite armorIcon;
    bool showHealthIcon = false;
    bool showStaminaIcon = false;
    bool showArmorIcon = false;

    void drawBar(sf::RenderWindow& window, float x, float y, float width, float height, float ratio, 
        const sf::Color& fgColor, const sf::Color& bgColor);
};
}
