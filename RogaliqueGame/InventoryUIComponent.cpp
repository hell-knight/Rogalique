#include "InventoryUIComponent.h"
#include "RenderSystem.h"
#include "Logger.h"

namespace RogaliqueGame {
InventoryUIComponent::InventoryUIComponent(
    MyEngine::GameObject* gameObject, InventoryComponent* playerInventory) 
    : Component(gameObject), inventory(playerInventory) {
    // Attempting to load a font (can be replaced with a system font)
    if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf")) {
        LOG_ERROR("InventoryUI: Failed to load font, text disabled.");
    }

    // The window's semi-transparent dark background
    background.setSize(sf::Vector2f(400, 300));
    background.setFillColor(sf::Color(0, 0, 0, 180));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);

    textTemplate.setFont(font);
    textTemplate.setCharacterSize(18);
    textTemplate.setFillColor(sf::Color::White);
}

void InventoryUIComponent::Update(float deltaTime) {
    // Key "I" processing with anti-repeat protection
    bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
    if (pressed && !keyWasPressed) {
        visible = !visible;
    }
    keyWasPressed = pressed;

    if (!visible || !inventory) {
        return;
    }

    // create sprites for the icons based on the number of items
    const auto& items = inventory->GetItems();
    iconSprites.resize(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].icon) {
            iconSprites[i].setTexture(*items[i].icon);
            sf::Vector2u texSize = items[i].icon->getSize();
            if (texSize.x > 0 && texSize.y > 0){
                float scaleX = ICON_SIZE / texSize.x;
                float scaleY = ICON_SIZE / texSize.y;
                float scale = std::min(scaleX, scaleY);
                iconSprites[i].setScale(scale, scale);
            } else {
                iconSprites[i].setScale(1.f, 1.f);
            }
        }
    }
}

void InventoryUIComponent::Render() {
    if (!visible || !inventory) {
        return;
    }

    auto& window = MyEngine::RenderSystem::Instance()->GetMainWindow();
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    // center the window
    sf::Vector2u winSize = window.getSize();
    sf::Vector2f backgroundPos(winSize.x / 2.f - background.getSize().x / 2.f,
                               winSize.y / 2.f - background.getSize().y / 2.f);
    background.setPosition(backgroundPos);
    window.draw(background);

    const auto& items = inventory->GetItems();
    float startX = backgroundPos.x + 20.f;
    float startY = backgroundPos.y + 20.f;
    float stepY = 50.f;

    for (size_t i = 0; i < items.size(); ++i) {
        // icon
        if (i < iconSprites.size() && items[i].icon)
        {
            iconSprites[i].setPosition(startX, startY + i * stepY);
            window.draw(iconSprites[i]);
        }

        // Text (name and quantity)
        std::string label = items[i].name;
        if (items[i].quantity > 1) {
            label += " x" + std::to_string(items[i].quantity);
        }
        textTemplate.setString(label);
        textTemplate.setPosition(startX + 40.f, startY + i * stepY + 5.f);
        window.draw(textTemplate);
    }

    window.setView(oldView);
}

}  // namespace RogaliqueGame
