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
#include "Boss.h"

using namespace MyEngine;

namespace RogaliqueGame {
void Level1Scene::Start() {
    LOG_INFO("Level1Scene::Start() - Player pointer: " +
             std::to_string((uintptr_t)player));
    LOG_INFO("Starting Level 1...");
    int width = 15;
    int height = 15;

    
    // Maze Generator
    MazeGenerator mazeGenerator(width, height, this);
    mazeGenerator.Generate();

    const auto& passGrid = mazeGenerator.GetPassabilityGrid();

    auto playerPos = MyEngine::Vector2Df(
        {width / 2 * 128.f, height / 2 * 128.f});
    if (player) {
        player->GetComponent<TransformComponent>()->SetWorldPosition(playerPos);
        LOG_INFO("Player created at (" + std::to_string(playerPos.x) + ", " +
                 std::to_string(playerPos.y) + ")");
    } else {
        LOG_ERROR("Player is null in Level1Scene!");
        return;
    }

    std::vector<Vector2Df> floorPositions;
    for (int y = 2; y < height - 1; ++y) {
        for (int x = 2; x < width - 1; ++x) {
            if (passGrid[y][x]) {
                floorPositions.push_back(
                    Vector2Df(x * 128.f, y * 128.f ));
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
        IncrementEnemyCount();
    }
    LOG_INFO("Spawned " + std::to_string(creepers.size()) + " creepers.");

    /*SpawnBoss(MyEngine::Vector2Df(2 * 128.f, 2 * 128.f));*/
    /*
    SetAllEnemiesDeadCallback([this, floorPositions]() mutable {
        if (!floorPositions.empty()) {
            int exitIdx = std::rand() % floorPositions.size();
            Vector2Df exitPos = floorPositions[exitIdx];
            auto exit = std::make_shared<LevelExit>(exitPos, [this]() {
                SceneManager::Instance()->RequestSwitch(new Level2Scene());
            });
            AddSceneObject(exit->GetGameObject());
            LOG_INFO("All enemies defeated! Exit placed at (" +
                     std::to_string(exitPos.x) + ", " +
                     std::to_string(exitPos.y) + ")");
        }
    });*/

    // Колбэк, когда все обычные враги убиты
    SetAllEnemiesDeadCallback([this, floorPositions]() {
        LOG_INFO("All ordinary enemies defeated! The boss emerges...");
        // 1. Удаляем внутренние стены
        RemoveInnerWalls();
        // 3. Создаём босса в центре
        Vector2Df bossPos(7 * 128.f, 7 * 128.f);
        auto boss = std::make_shared<Boss>(bossPos, player);
        AddEnemy(boss);  // увеличит aliveEnemies
        IncrementEnemyCount();
        bossSpawned = true;

        // 4. Колбэк после смерти босса — открыть выход
        SetBossDeadCallback([this, floorPositions]() {
            if (!floorPositions.empty()) {
                int exitIdx = std::rand() % floorPositions.size();
                auto exitPos = floorPositions[exitIdx];
                auto exit = std::make_shared<LevelExit>(exitPos, [this]() {
                    SceneManager::Instance()->RequestSwitch(new Level2Scene());
                });
                AddSceneObject(exit->GetGameObject());
                LOG_INFO("Boss defeated! Exit opened.");
            }
        });
    });

    LOG_INFO("Level 1 setup complete.");
}
}  // namespace RogaliqueGame