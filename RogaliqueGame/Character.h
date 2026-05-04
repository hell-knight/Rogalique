#pragma once
#include "GameObject.h"
#include "Vector.h"

namespace RogaliqueGame {
class Character {
   public:
    Character(const MyEngine::Vector2Df& position, MyEngine::GameObject* player,
              const std::string& name = "Character");
    virtual ~Character();

    MyEngine::GameObject* GetGameObject() const;

   protected:
    MyEngine::GameObject* gameObject = nullptr;
    MyEngine::GameObject* player = nullptr;
};
}  // namespace RogaliqueGame