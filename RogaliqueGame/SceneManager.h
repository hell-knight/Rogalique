#pragma once
#include <memory>
#include "LevelScene.h"

namespace RogaliqueGame {
class SceneManager {
   public:
    static SceneManager* Instance();

    void Init(MyEngine::GameObject* newPlayer);
    void RequestSwitch(LevelScene* newScene);
    void ProcessSwitch();

    LevelScene* GetCurrent() const { return currentScene; }
    MyEngine::GameObject* GetPlayer() const { return player; }

    void SetHUD(MyEngine::GameObject* hud) { hudObject = hud; }

   private:
    SceneManager() = default;
    LevelScene* currentScene = nullptr;
    LevelScene* pendingScene = nullptr;
    MyEngine::GameObject* player = nullptr;
    MyEngine::GameObject* hudObject = nullptr;
    MyEngine::GameObject* inventoryUIobj = nullptr;
};
}
