#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>

namespace RogaliqueGame {
struct Item {
    std::string name;
    std::string description;
    const sf::Texture* icon = nullptr;
    int quantity = 1;
};
}