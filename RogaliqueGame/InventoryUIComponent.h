#pragma once
#include "Component.h"
#include "InventoryComponent.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace RogaliqueGame {
class InventoryUIComponent : public MyEngine::Component {
   public:
    InventoryUIComponent(MyEngine::GameObject* gameObject, InventoryComponent* playerInventory);

    void Update(float deltaTime) override;
    void Render() override;

   private:
    InventoryComponent* inventory;
    bool visible = false;
    bool keyWasPressed = false;

    // Constantes cell
    static constexpr float CELL_SIZE = 50.f;
    static constexpr float CELL_MARGIN = 5.f;
    static constexpr int COLS = 4;

    sf::RectangleShape background;
    sf::Font font;
    sf::Text textTemplate;
    std::vector<sf::Sprite> iconSprites;    // for reuse
    std::vector<sf::Vector2f> cellPositions;    // the top-left corner of each cell

    // for tooltip
    int hoveredItem = -1;
    sf::RectangleShape tooltipBg;
    sf::Text tooltipText;
};
}
