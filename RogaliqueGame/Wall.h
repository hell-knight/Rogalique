#pragma once
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "GameObject.h"

namespace RogaliqueGame {
class Wall {
   public:
    Wall(const MyEngine::Vector2Df position, int textureMapIndex);
    MyEngine::Vector2Df GetPosition() const;
    MyEngine::GameObject* GetGameObject() const { return gameObject; }

   private:
    MyEngine::GameObject* gameObject;
};
}  // namespace RogaliqueGame