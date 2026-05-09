#pragma once
#include "Component.h"
#include "InventoryComponent.h"
#include <SFML/Graphics.hpp>

namespace RogaliqueGame {
static constexpr float ICON_SIZE = 32.f;

class InventoryUIComponent : public MyEngine::Component {
   public:
    InventoryUIComponent(MyEngine::GameObject* gameObject, InventoryComponent* playerInventory);

    void Update(float deltaTime) override;
    void Render() override;

   private:
    InventoryComponent* inventory;
    bool visible = false;
    bool keyWasPressed = false;

    sf::RectangleShape background;
    sf::Font font;
    sf::Text textTemplate;
    std::vector<sf::Sprite> iconSprites;    // for reuse
};
}
