#pragma once
#include "Character.h"
#include "Vector.h"

namespace RogaliqueGame {
class AI : public Character {
   public:
    AI(const MyEngine::Vector2Df& position, MyEngine::GameObject* player);
    /*MyEngine::GameObject* GetGameObject();

private:
    MyEngine::GameObject* gameObject;*/
};

}  // namespace RogaliqueGame