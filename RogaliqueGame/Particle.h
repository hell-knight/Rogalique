#pragma once
#include <SFML/Graphics/Color.hpp>
#include "Vector.h"

namespace RogaliqueGame {
	struct Particle {
    MyEngine::Vector2Df position;
    MyEngine::Vector2Df velocity;
    float life = 1.0f;  // remaining lifetime in seconds
    float maxLife = 1.0f;
    sf::Color color = sf::Color::White;
    float size = 4.0f;  // size in pixels
};
}