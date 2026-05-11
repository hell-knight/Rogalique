#pragma once
#include "Character.h"
#include "Vector.h"

namespace RogaliqueGame {
class Boss : public Character {
   public:
    Boss(const MyEngine::Vector2Df& position, MyEngine::GameObject* player);
};
}  // namespace RogaliqueGame