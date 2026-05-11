#include "pch.h"
#include "Level2Scene.h"
#include "Level3Scene.h"
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
#include <algorithm>
#include "Boss.h"
#include "randomizer.h"

using namespace MyEngine;

namespace RogaliqueGame {
void Level2Scene::Start() {
    LOG_INFO("Starting Level 2...");
    const int width = 15;
    const int height = 15;

    MazeGenerator maze(width, height, this);
    maze.Generate();

    const auto& passGrid = maze.GetPassabilityGrid();

    auto playerPos = Vector2Df(width / 2.f * 128.f, height / 2.f * 128.f);

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
    for (int y = 2; y < height - 1; ++y) {
        for (int x = 2; x < width - 1; ++x) {
            if (passGrid[y][x]) {
                floorPositions.push_back(
                    Vector2Df(x * 128.f, y * 128.f));
            }
        }
    }

    floorPositions.erase(
        std::remove(floorPositions.begin(), floorPositions.end(), playerPos),
        floorPositions.end());

    Spawner mixedSpawner(
        [this](const Vector2Df& pos) -> std::shared_ptr<Character> {
            if (random(0, 1) == 1)
                return std::make_shared<Creeper>(pos, player);
            else
                return std::make_shared<AI>(pos, player);
        });

    int enemyCount = 7;
    auto enemies = mixedSpawner.SpawnRandom(enemyCount, floorPositions);
    for (auto& enemy : enemies) {
        Vector2Df enemyPos = enemy->GetGameObject()
                                 ->GetComponent<TransformComponent>()
                                 ->GetWorldPosition();
        // Remove the enemy unit from the list of available units (so they don't
        // overlap)
        floorPositions.erase(
            std::remove(floorPositions.begin(), floorPositions.end(), enemyPos),
            floorPositions.end());
        AddEnemy(enemy);
        IncrementEnemyCount();
    }
    LOG_INFO("Spawned " + std::to_string(enemies.size()) + " enemies.");

    SetAllEnemiesDeadCallback([this, floorPositions]() {
        LOG_INFO("All ordinary enemies defeated! The boss emerges...");
        RemoveInnerWalls();
        Vector2Df bossPos(7 * 128.f, 7 * 128.f);
        auto boss = std::make_shared<Boss>(bossPos, player);
        AddEnemy(boss);
        IncrementEnemyCount();
        bossSpawned = true;

        SetBossDeadCallback([this, floorPositions]() {
            if (!floorPositions.empty()) {
                int exitIdx = std::rand() % floorPositions.size();
                auto exitPos = floorPositions[exitIdx];
                auto exit = std::make_shared<LevelExit>(exitPos, [this]() {
                    SceneManager::Instance()->RequestSwitch(new Level3Scene());
                });
                AddSceneObject(exit->GetGameObject());
                LOG_INFO("Boss defeated! Exit opened.");
            }
        });
    });

    LOG_INFO("Level 2 setup complete.");
}
}  // namespace RogaliqueGame