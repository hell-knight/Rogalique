#pragma once
#include "GameObject.h"
#include "Vector.h"
#include <functional>

namespace RogaliqueGame
{
class LevelExit {
   public:
	// position – location on the map, callback – what to do when a player joins
    LevelExit(const MyEngine::Vector2Df& position, std::function<void()> onPlayerEnter);
    ~LevelExit() = default;
    MyEngine::GameObject* GetGameObject() const { return gameObject; }

   private:
    MyEngine::GameObject* gameObject = nullptr;
    int triggerSubId = -1;
};
}
