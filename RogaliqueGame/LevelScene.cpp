#include "pch.h"
#include "LevelScene.h"
#include "Floor.h"
#include "Wall.h"
#include "GameWorld.h"

namespace RogaliqueGame {
LevelScene::LevelScene() = default;
LevelScene::~LevelScene() = default;

void LevelScene::Stop() {
    for (auto* obj : sceneObjects) {
        MyEngine::GameWorld::Instance()->DestroyGameObject(obj);
    }
    sceneObjects.clear();
    floors.clear();
    walls.clear();
}

void LevelScene::AddSceneObject(MyEngine::GameObject* obj) {
    if (obj) {
        sceneObjects.push_back(obj);
    }
}

void LevelScene::AddFloor(std::unique_ptr<Floor> floor) {
    if (!floor) {
        return;
    }
    MyEngine::GameObject* go = floor->GetGameObject();
    if (go) {
        sceneObjects.push_back(go);
    }
    floors.push_back(std::move(floor));
}

void LevelScene::AddWall(std::unique_ptr<Wall> wall) {
    if (!wall) {
        return;
    }
    MyEngine::GameObject* go = wall->GetGameObject();
    if (go) {
        sceneObjects.push_back(go);
    }
    walls.push_back(std::move(wall));
}

void LevelScene::AddEnemy(std::shared_ptr<Character> enemy) {
    if (!enemy) {
        return;
    }
    MyEngine::GameObject* go = enemy->GetGameObject();
    if (go) {
        sceneObjects.push_back(go);
    }
    enemies.push_back(std::move(enemy));
}
}  // namespace RogaliqueGame