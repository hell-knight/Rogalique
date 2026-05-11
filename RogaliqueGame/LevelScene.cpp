#include "pch.h"
#include "LevelScene.h"
#include "Floor.h"
#include "Wall.h"
#include "GameWorld.h"
#include "Boss.h"

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
    enemies.clear();

    aliveEnemies = 0;
    bossSpawned = false;
    m_allEnemiesDeadCalled = false;
    m_bossDeadCalled = false;
    onAllEnemiesDead = nullptr;
    onBossDead = nullptr;
}

void LevelScene::AddSceneObject(MyEngine::GameObject* obj) {
    if (obj) {
        sceneObjects.push_back(obj);
    }
}

void LevelScene::RemoveSceneObject(MyEngine::GameObject* obj) {
    sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), obj), sceneObjects.end());
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

void LevelScene::SetAllEnemiesDeadCallback(std::function<void()> callback) {
    onAllEnemiesDead = std::move(callback);
}

void LevelScene::IncrementEnemyCount() { ++aliveEnemies; }

void LevelScene::DecrementEnemyCount() { 
    if (aliveEnemies > 0) {
        --aliveEnemies;
        if (aliveEnemies == 0) {
            if (!bossSpawned && !m_allEnemiesDeadCalled) {
                m_allEnemiesDeadCalled = true;
                if (onAllEnemiesDead) onAllEnemiesDead();
            } else if (bossSpawned && !m_bossDeadCalled) {
                m_bossDeadCalled = true;
                if (onBossDead) onBossDead();
            }
        }
    }
}

void LevelScene::SetBossDeadCallback(std::function<void()> callback) {
    onBossDead = std::move(callback);
}

void LevelScene::RemoveInnerWalls() {
    const float tileSize = 128.f;
    const int width = 15, height = 15;
    std::vector<Wall*> toRemove;

    for (auto& wall : walls) {
        auto pos = wall->GetPosition();
        int tileX = static_cast<int>(pos.x / tileSize);
        int tileY = static_cast<int>(pos.y / tileSize);
        // keep only the boundary walls (x==0, x==width, y==0, y==height)
        if (tileX > 0 && tileX < width && tileY > 0 && tileY < height) {
            toRemove.push_back(wall.get());
        }
    }

    for (auto* w : toRemove) {
        auto it = std::find_if(
            walls.begin(), walls.end(),
            [w](const std::unique_ptr<Wall>& p) { return p.get() == w; });
        if (it != walls.end()) {
            MyEngine::GameObject* go = (*it)->GetGameObject();
            RemoveSceneObject(go);
            MyEngine::GameWorld::Instance()->DestroyGameObject(go);
            walls.erase(it);
        }
    }
}

}  // namespace RogaliqueGame