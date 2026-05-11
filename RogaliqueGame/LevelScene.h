#pragma once
#include "Scene.h"
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Vector.h"
#include "Character.h"
#include <functional>

namespace RogaliqueGame {
class Floor;
class Wall;

class LevelScene : public MyEngine::Scene{
   public:
    LevelScene();
    ~LevelScene();

    void SetPlayer(MyEngine::GameObject* newPlayer) { player = newPlayer; }
    MyEngine::GameObject* GetPlayer() const { return player; }

    // Redefining Stop: removing all scene objects except the player
    void Stop() override;

    // The inherited Start and Restart methods will be purely virtual in the derived classes
    virtual void Start() = 0;
    virtual void Restart() = 0;

    // Adds the object to the watchlist (will be removed when “Stop” is
    // selected)
    void AddSceneObject(MyEngine::GameObject* obj);

    void RemoveSceneObject(MyEngine::GameObject* obj);

    void AddFloor(std::unique_ptr<Floor> floor);
    void AddWall(std::unique_ptr<Wall> wall);
    void AddEnemy(std::shared_ptr<Character> enemy);

    const std::vector<std::unique_ptr<Floor>>& GetFloors() const {
        return floors;
    }
    const std::vector<std::unique_ptr<Wall>>& GetWalls() const {
        return walls;
    }

    void SetAllEnemiesDeadCallback(std::function<void()> callback);
    void IncrementEnemyCount();
    void DecrementEnemyCount();
    int GetAliveEnemies() const { return aliveEnemies; }

    void SetBossDeadCallback(std::function<void()> callback);
    void RemoveInnerWalls();  // removes all walls except the boundary walls

   protected:
    MyEngine::GameObject* player = nullptr;
    bool bossSpawned = false;

   private:
    std::vector<MyEngine::GameObject*> sceneObjects;
    std::vector<std::unique_ptr<Floor>> floors;
    std::vector<std::unique_ptr<Wall>> walls;
    std::vector<std::shared_ptr<Character>> enemies;
    int aliveEnemies = 0;
    std::function<void()> onAllEnemiesDead;

    std::function<void()> onBossDead;

    bool m_allEnemiesDeadCalled = false;
    bool m_bossDeadCalled = false;
};
}
