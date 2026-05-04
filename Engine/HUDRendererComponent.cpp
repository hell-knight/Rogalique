#include "pch.h"
#include "HUDRendererComponent.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include "GameObject.h"

namespace MyEngine {
HUDRendererComponent::HUDRendererComponent(GameObject* gameObject,
                                           HealthComponent* health,
                                           StaminaComponent* stamina,
                                           const sf::Texture* healthIconTex,
                                           const sf::Texture* staminaIconTex) 
    : Component(gameObject), health(health), stamina(stamina) {
    if (healthIconTex) {
        auto size = healthIconTex->getSize();
        healthIcon.setTexture(*healthIconTex);
        healthIcon.setScale(24.f / size.x, 24.f / size.y);
        healthIcon.setOrigin(0, 0);
        healthIcon.setColor(sf::Color::Red);
        showHealthIcon = true;
    }

    if (staminaIconTex) {
        staminaIcon.setTexture(*staminaIconTex);
        auto size = staminaIconTex->getSize();
        staminaIcon.setScale(24.f / size.x, 24.f / size.y);
        staminaIcon.setOrigin(0, 0);
        showStaminaIcon = true;
    }

    if (!health && !stamina) {
        LOG_ERROR(
            "HUDRendererComponent requires at least HealthComponent or "
            "StaminaComponent. Removing.");
        gameObject->RemoveComponent(this);
    }
}

void HUDRendererComponent::Render() {
    auto& window = RenderSystem::Instance()->GetMainWindow();
    // Save the camera view and switch to screen coordinates
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    float startX = 20.f, startY = 20.f;
    float barWidth = 200.f, barHeight = 20.f;
    float spacing = 10.f;
    float iconOffset = 30.f;

    if (health) {
        if (showHealthIcon) {
            healthIcon.setPosition(startX, startY - 2.f);
            window.draw(healthIcon);
        }
        float barX = showHealthIcon ? startX + iconOffset : startX;
        float ratio = (health->GetMaxHealth() > 0)
                          ? health->GetHealth() / health->GetMaxHealth()
                          : 0.f;
        drawBar(window, barX, startY, barWidth, barHeight, ratio,
                sf::Color::Red, sf::Color(40, 40, 40));
    }

    if (stamina) {
        float y = startY + barHeight + spacing;
        if (showStaminaIcon) {
            staminaIcon.setPosition(startX, y - 2.f);
            window.draw(staminaIcon);
        }
        float barX = showStaminaIcon ? startX + iconOffset : startX;
        float ratio = (stamina->GetMaxStamina() > 0)
                          ? stamina->GetStamina() / stamina->GetMaxStamina()
                          : 0.f;
        drawBar(window, barX, y, barWidth, barHeight, ratio,
                sf::Color::Yellow, sf::Color(40, 40, 40));
    }

    window.setView(oldView);
}

void HUDRendererComponent::drawBar(sf::RenderWindow& window, float x, float y,
                                   float width, float height, float ratio,
                                   const sf::Color& fgColor,
                                   const sf::Color& bgColor) {
    sf::RectangleShape bg(sf::Vector2f(width, height));
    bg.setPosition(x, y);
    bg.setFillColor(bgColor);
    window.draw(bg);

    if (ratio > 0.f) {
        sf::RectangleShape fg(sf::Vector2f(width * ratio, height));
        fg.setPosition(x, y);
        fg.setFillColor(fgColor);
        window.draw(fg);
    }

    sf::RectangleShape border(sf::Vector2f(width, height));
    border.setPosition(x, y);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(1.f);
    window.draw(border);
}


}  // namespace MyEngine
