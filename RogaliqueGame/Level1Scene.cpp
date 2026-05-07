#include "pch.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "SceneManager.h"
#include "Player.h"
#include "AI.h"
#include "Creeper.h"
#include "Floor.h"
#include "Wall.h"
#include "Music.h"
#include "MazeGenerator.h"
#include "LevelExit.h"
#include "GameWorld.h"
#include "Logger.h"
#include "Spawner.h"
#include <algorithm>

using namespace MyEngine;

namespace RogaliqueGame {
void Level1Scene::Start() {
    LOG_INFO("Level1Scene::Start() - Player pointer: " +
             std::to_string((uintptr_t)player));
    LOG_INFO("Starting Level 1...");
    int width = 15;
    int height = 15;

    for (int y = 0; y < height + 1; y++) {
        for (int x = 0; x < width + 1; x++) {
            // if not wall place
            if (x != 0 && x != width && y != 0 && y != height) {
                AddFloor(std::make_unique<Floor>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 0));
            }

            // if left-bottom corner
            if (x == 0 && y == 0) {
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 25));
            }

            // if right-bottom corner
            if (x == width && y == 0) {
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 27));
            }

            // if left-top corner
            if (x == 0 && y == height) {
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 1));
            }

            // if right-top corner
            if (x == width && y == height) {
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 3));
            }

            // if left (not corner)
            if (x == 0 && y != height && y != 0) {
                AddFloor(std::make_unique<Floor>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 18));
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 12));
            }

            // if right (not corner)
            if (x == width && y != height && y != 0) {
                AddFloor(std::make_unique<Floor>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 19));
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 12));
            }

            // if bottom (not corner)
            if (y == 0 && x != width && x != 0) {
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 38));
            }

            // if top (not corner)
            if (y == height && x != width && x != 0) {
                AddWall(std::make_unique<Wall>(
                    MyEngine::Vector2Df(x * 128.f, y * 128.f), 38));
            }
        }
    }
    // Maze Generator
    MazeGenerator mazeGenerator(width, height, this);
    mazeGenerator.Generate();

    const auto& passGrid = mazeGenerator.GetPassabilityGrid();

    auto playerPos =
        MyEngine::Vector2Df({width / 2 * 128.f, height / 2 * 128.f});
    if (player) {
        player->GetComponent<TransformComponent>()->SetWorldPosition(playerPos);
        LOG_INFO("Player created at (" + std::to_string(playerPos.x) + ", " +
                 std::to_string(playerPos.y) + ")");
    } else {
        LOG_ERROR("Player is null in Level1Scene!");
        return;
    }

    std::vector<Vector2Df> floorPositions;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (passGrid[y][x]) {
                floorPositions.push_back(Vector2Df(x * 128.f, y * 128.f));
            }
        }
    }

    // Removing the player's positions
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

    auto creepers = mixedSpawner.SpawnRandom(5, floorPositions);
    for (auto& enemy : creepers) {
        Vector2Df enemyPos = enemy->GetGameObject()->GetComponent<TransformComponent>()->GetWorldPosition();
        // Remove the enemy unit from the list of available units (so they don't overlap)
        floorPositions.erase(std::remove(floorPositions.begin(), floorPositions.end(), enemyPos), floorPositions.end());
        AddEnemy(enemy);
    }
    LOG_INFO("Spawned " + std::to_string(creepers.size()) + " creepers.");

    for (auto& enemy : creepers) {
        Vector2Df enemyPos = enemy->GetGameObject()
                                 ->GetComponent<TransformComponent>()
                                 ->GetWorldPosition();
        floorPositions.erase(
            std::remove(floorPositions.begin(), floorPositions.end(), enemyPos),
            floorPositions.end());
    }

    if (!floorPositions.empty()) {
        int exitIdx = std::rand() % floorPositions.size();
        Vector2Df exitPos = floorPositions[exitIdx];
        auto exit = std::make_shared<LevelExit>(exitPos, [this]() {
            SceneManager::Instance()->RequestSwitch(new Level2Scene());
        });
        AddSceneObject(exit->GetGameObject());
        LOG_INFO("Exit placed at (" + std::to_string(exitPos.x) + ", " +
                 std::to_string(exitPos.y) + ")");
    } else {
        LOG_WARN("No free cell for exit after placing enemies!");
    }

    LOG_INFO("Level 1 setup complete.");
}
}  // namespace RogaliqueGame