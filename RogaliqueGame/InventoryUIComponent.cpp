#include "InventoryUIComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include <cmath>

static constexpr float TOOLTIP_OFFSET_X = 20.f; // space after the cursor
static constexpr float TOOLTIP_OFFSET_Y = 20.f;

namespace RogaliqueGame {
InventoryUIComponent::InventoryUIComponent(
    MyEngine::GameObject* gameObject, InventoryComponent* playerInventory) 
    : Component(gameObject), inventory(playerInventory) {
    // Attempting to load a font (can be replaced with a system font)
    if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf")) {
        LOG_ERROR("InventoryUI: Failed to load font, text disabled.");
    }

    // The window size is now dynamic; we'll recalculate it in the Update method
    background.setFillColor(sf::Color(20, 20, 20, 200));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);

    textTemplate.setFont(font);
    textTemplate.setCharacterSize(14);
    textTemplate.setFillColor(sf::Color::White);

    // Configuring tooltips
    tooltipBg.setFillColor(sf::Color(40, 40, 40, 230));
    tooltipBg.setOutlineThickness(1);
    tooltipBg.setOutlineColor(sf::Color(180, 180, 180));

    tooltipText.setFont(font);
    tooltipText.setCharacterSize(12);
    tooltipText.setFillColor(sf::Color::White);
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
    int count = static_cast<int>(items.size());

    // Determine the dimensions of the window for the screen
    int rows = std::max(1, (count + COLS - 1) / COLS);
    float totalWidth = COLS * (CELL_SIZE + CELL_MARGIN) - CELL_MARGIN + 40;
    float totalHeight = rows * (CELL_SIZE + CELL_MARGIN) - CELL_MARGIN + 40;
    background.setSize(sf::Vector2f(totalWidth, totalHeight));

    // Cell positions
    cellPositions.clear();
    cellPositions.reserve(count);
    for (int i = 0; i < count; ++i) {
        int col = i % COLS;
        int row = i / COLS;
        float x = 20 + col * (CELL_SIZE + CELL_MARGIN);
        float y = 20 + row * (CELL_SIZE + CELL_MARGIN);
        cellPositions.emplace_back(x, y);
    }

    // Preparing icons
    iconSprites.resize(count);
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].icon) {
            iconSprites[i].setTexture(*items[i].icon);
            sf::Vector2u texSize = items[i].icon->getSize();
            if (texSize.x > 0 && texSize.y > 0){
                // Place the icon in the cell with an 8px margin from the edges
                float maxIconSize = CELL_SIZE - 16.f;
                float scale = maxIconSize / std::max(texSize.x, texSize.y);
                iconSprites[i].setScale(scale, scale);
                // Center relative to the cell
                sf::FloatRect localBounds = iconSprites[i].getLocalBounds();
                iconSprites[i].setOrigin(localBounds.width / 2.f,
                                         localBounds.height / 2.f);
            } 
        }
    }

    // --- Mouse Hover Detection ---
    hoveredItem = -1;
    auto& window = MyEngine::RenderSystem::Instance()->GetMainWindow();
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    // Convert to the coordinates of the default view (screen)
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, window.getDefaultView());

    // Calculating the background position (centered)
    sf::Vector2u winSize = window.getSize();
    sf::Vector2f bgPos(winSize.x / 2.f - background.getSize().x / 2.f,
                       winSize.y / 2.f - background.getSize().y / 2.f);

    for (size_t i = 0; i < cellPositions.size(); ++i) {
        sf::FloatRect cellRect(bgPos + cellPositions[i], sf::Vector2f(CELL_SIZE, CELL_SIZE));
        if (cellRect.contains(mouseWorld)) {
            hoveredItem = static_cast<int>(i);
            break;
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
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Vector2f cellPos = backgroundPos + cellPositions[i];

        // Cell background
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setPosition(cellPos);
        // The cell under the cursor is slightly highlighted
        if (static_cast<int>(i) == hoveredItem) {
            cell.setFillColor(sf::Color(80, 80, 80, 150));
        } else {
            cell.setFillColor(sf::Color(60, 60, 60, 150));
        }
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color(120, 120, 120));
        window.draw(cell);

        // icon
        if (i < iconSprites.size() && items[i].icon)
        {
            // Center cell
            sf::Vector2f center(cellPos.x + CELL_SIZE / 2.f, cellPos.y + CELL_SIZE / 2.f);
            iconSprites[i].setPosition(center);
            window.draw(iconSprites[i]);
        }

        // Quantity
        if (items[i].quantity > 0) {
            std::string quantity = std::to_string(items[i].quantity);
            textTemplate.setString(quantity);
            // Place the cell with indentation in the lower-right corner
            sf::FloatRect textBox = textTemplate.getLocalBounds();
            float textX = cellPos.x + CELL_SIZE - textBox.width - 5.f;
            float textY = cellPos.y + CELL_SIZE - textBox.height - 5.f;
            textTemplate.setPosition(textX, textY);
            window.draw(textTemplate);
        }
    }

    // --- Display a tooltip if a description is available ---
    if (hoveredItem >= 0 && hoveredItem < static_cast<int>(items.size())) {
        const std::string& desc = items[hoveredItem].description;
        if (!desc.empty()){
            tooltipText.setString(desc);

            // Get the mouse position for placement nearby
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, window.getDefaultView());

            // Size text
            sf::FloatRect textBounds = tooltipText.getLocalBounds();
            float padding = 8.f;
            float bgW = textBounds.width + padding * 2;
            float bgH = textBounds.height + padding * 2;

            // Align to the bottom right of the cursor
            float bgX = mouseWorld.x + TOOLTIP_OFFSET_X;
            float bgY = mouseWorld.y + TOOLTIP_OFFSET_Y;

            // If it doesn't fit on the right, move it to the left
            if (bgX + bgW > winSize.x) {
                bgX = mouseWorld.x - bgW - TOOLTIP_OFFSET_X;
            }
            // If it doesn't fit at the bottom, try higher up
            if (bgY + bgH > winSize.y) {
                bgY = mouseWorld.y - bgH - TOOLTIP_OFFSET_Y;
            }

            tooltipBg.setSize(sf::Vector2f(bgW, bgH));
            tooltipBg.setPosition(bgX, bgY);
            tooltipText.setPosition(bgX + padding, bgY + padding);

            window.draw(tooltipBg);
            window.draw(tooltipText);
        }
    }

    window.setView(oldView);
}

}  // namespace RogaliqueGame
