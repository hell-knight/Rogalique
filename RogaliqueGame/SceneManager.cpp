#include "pch.h"
#include "SceneManager.h"
#include "GameWorld.h"
#include "Logger.h"

namespace RogaliqueGame {
SceneManager* SceneManager::Instance() { 
	static SceneManager manager;
	return &manager; 
}

void SceneManager::Init(MyEngine::GameObject* newPlayer) { player = newPlayer; }

void SceneManager::RequestSwitch(LevelScene* newScene) {
    if (pendingScene) {
        delete pendingScene;
    }
    pendingScene = newScene;
}

void SceneManager::ProcessSwitch() {
    if (!pendingScene) {
        return;
    }

    // Stop the current scene (it will remove its objects)
    if (currentScene) {
        currentScene->Stop();
    }

    // Start a new one
    currentScene = pendingScene;
    pendingScene = nullptr;

    if (currentScene) {
        currentScene->SetPlayer(player);
        currentScene->Start();
        LOG_INFO("Switched to new scene.");
    }

}


}  // namespace RogaliqueGame