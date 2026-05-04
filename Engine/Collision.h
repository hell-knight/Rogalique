#pragma once
#include <SFML/Graphics.hpp>

namespace MyEngine {
class ColliderComponent;
struct Collision {
   public:
    ColliderComponent* first;
    ColliderComponent* second;
    sf::FloatRect collisionRect;

    Collision(ColliderComponent* newFirst, ColliderComponent* newSecond,
              sf::FloatRect newCollisionRect)
        : first(newFirst),
          second(newSecond),
          collisionRect(newCollisionRect) {};
};
}  // namespace MyEngine