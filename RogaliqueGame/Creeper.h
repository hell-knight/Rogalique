#pragma once
#include "Character.h"
#include "Vector.h"

namespace RogaliqueGame {
class Creeper : public Character {
   public:
    Creeper(const MyEngine::Vector2Df& position, MyEngine::GameObject* player);
};
}  // namespace RogaliqueGame
