#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>

namespace RogaliqueGame {
struct Item {
    std::string name;
    const sf::Texture* icon = nullptr;
    int quantity = 1;
};
}