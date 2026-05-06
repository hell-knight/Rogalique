#include "pch.h"
#include "Level2Scene.h"
#include "Level1Scene.h"
#include "SceneManager.h"
#include "Player.h"
#include "AI.h"
#include "Creeper.h"
#include "Floor.h"
#include "Wall.h"
#include "MazeGenerator.h"
#include "LevelExit.h"
#include "Spawner.h"
#include "GameWorld.h"
#include "Logger.h"
#include <set>
#include <algorithm>

using namespace MyEngine;

namespace RogaliqueGame {
void Level2Scene::Start() {
    LOG_INFO("Starting Level 2...");
    const int width = 15;
    const int height = 15;

    for (int y = 0; y <= height; ++y) {
        for (int x = 0; x <= width; ++x) {
            if (x != 0 && x != width && y != 0 && y != height) {
                AddFloor(std::make_unique<Floor>(
                    Vector2Df(x * 128.f, y * 128.f), 0));
            }

            if (x == 0 && y == 0)
                AddWall(std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f),
                                               25));
            if (x == width && y == 0)
                AddWall(std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f),
                                               27));
            if (x == 0 && y == height)
                AddWall(
                    std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f), 1));
            if (x == width && y == height)
                AddWall(
                    std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f), 3));

            if (x == 0 && y != 0 && y != height) {
                AddFloor(std::make_unique<Floor>(
                    Vector2Df(x * 128.f, y * 128.f), 18));
                AddWall(std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f),
                                               12));
            }
            if (x == width && y != 0 && y != height) {
                AddFloor(std::make_unique<Floor>(
                    Vector2Df(x * 128.f, y * 128.f), 19));
                AddWall(std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f),
                                               12));
            }
            if (y == 0 && x != 0 && x != width)
                AddWall(std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f),
                                               38));
            if (y == height && x != 0 && x != width)
                AddWall(std::make_unique<Wall>(Vector2Df(x * 128.f, y * 128.f),
                                               38));
        }
    }

    MazeGenerator maze(width, height, this);
    maze.Generate();

    int playerStartX = 1;
    int playerStartY = height / 2;
    auto playerPos = Vector2Df(playerStartX * 128.f, playerStartY * 128.f);

    if (player) {
        player->GetComponent<TransformComponent>()->SetWorldPosition(
            playerPos);
        LOG_INFO("Player placed at (" + std::to_string(playerPos.x) + ", " +
                 std::to_string(playerPos.y) + ")");
    } else {
        LOG_ERROR("Player is null in Level2Scene!");
        return;
    }

    std::vector<Vector2Df> floorPositions;
    for (auto& floor : GetFloors())
        floorPositions.push_back(floor->GetPosition());

    std::set<Vector2Df> wallPositions;
    for (auto& w : GetWalls()) wallPositions.insert(w->GetPosition());

    floorPositions.erase(
        std::remove_if(
            floorPositions.begin(), floorPositions.end(),
            [&](const Vector2Df& pos) { return wallPositions.count(pos) > 0; }),
        floorPositions.end());

    floorPositions.erase(
        std::remove(floorPositions.begin(), floorPositions.end(), playerPos),
        floorPositions.end());

    Spawner mixedSpawner(
        [this](const Vector2Df& pos) -> std::shared_ptr<Character> {
            if (rand() % 2)
                return std::make_shared<Creeper>(pos, player);
            else
                return std::make_shared<AI>(pos, player);
        });

    int enemyCount = 7;
    auto enemies = mixedSpawner.SpawnRandom(enemyCount, floorPositions);
    for (auto& e : enemies) {
        AddSceneObject(e->GetGameObject());
        Vector2Df ePos = e->GetGameObject()
                             ->GetComponent<TransformComponent>()
                             ->GetWorldPosition();
        floorPositions.erase(
            std::remove(floorPositions.begin(), floorPositions.end(), ePos),
            floorPositions.end());
    }
    LOG_INFO("Spawned " + std::to_string(enemies.size()) + " enemies.");

    if (!floorPositions.empty()) {
        int exitIdx = std::rand() % floorPositions.size();
        Vector2Df exitPos = floorPositions[exitIdx];
        auto exit = std::make_shared<LevelExit>(exitPos, [this]() {
            SceneManager::Instance()->RequestSwitch(new Level1Scene());
        });
        AddSceneObject(exit->GetGameObject());
        LOG_INFO("Exit placed at (" + std::to_string(exitPos.x) + ", " +
                 std::to_string(exitPos.y) + ")");
    } else {
        LOG_WARN("No free cell for exit!");
    }

    LOG_INFO("Level 2 setup complete.");
}
}  // namespace RogaliqueGame