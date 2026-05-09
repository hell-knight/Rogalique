#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "Level1Scene.h"
#include "SceneManager.h"
#include "HUDRendererComponent.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "Matrix2D.h"
#include "Logger.h"
#include "Music.h"
#include "BaseLevel.h"
#include "InventoryComponent.h"
#include "InventoryUIComponent.h"

class RogaliqueEngine : public MyEngine::Engine {
   public:
    void OnFrameEnd() override { RogaliqueGame::SceneManager::Instance()->ProcessSwitch(); }
};

int main() {
    RogaliqueEngine rogaliqueEngine;

    auto* window =
        new sf::RenderWindow(sf::VideoMode(1280, 720), "RogaliqueGame");
    MyEngine::RenderSystem::Instance()->SetMainWindow(window);

    MyEngine::ResourceSystem::Instance()->LoadTextureMap(
        "player", "Resources/Textures/Player.png", {48, 63}, 4, false);
    MyEngine::ResourceSystem::Instance()->LoadTextureMap(
        "level_floors", "Resources/Textures/Floor.png", {16, 16}, 49, false);
    MyEngine::ResourceSystem::Instance()->LoadTextureMap(
        "level_walls", "Resources/Textures/Wall.png", {16, 16}, 48, false);
    MyEngine::ResourceSystem::Instance()->LoadTextureMap(
        "ai", "Resources/Textures/AI.png", {784 / 3, 1168 / 3}, 9, false);
    MyEngine::ResourceSystem::Instance()->LoadTexture(
        "icon_health", "Resources/Textures/health.png");
    MyEngine::ResourceSystem::Instance()->LoadTexture(
        "icon_armor", "Resources/Textures/armor.png");
    MyEngine::ResourceSystem::Instance()->LoadTexture(
        "icon_stamina", "Resources/Textures/stamina.png");
    MyEngine::ResourceSystem::Instance()->LoadTexture(
        "icon_key", "Resources/Textures/key.png");
    MyEngine::ResourceSystem::Instance()->LoadTexture(
        "icon_potion", "Resources/Textures/health_potion.png");

    MyEngine::ResourceSystem::Instance()->LoadSound(
        "music", "Resources/Sounds/game_background.ogg");

    LOG_INFO("Resources loaded.");

    auto music = std::make_unique<RogaliqueGame::Music>("music");

    RogaliqueGame::CreateBaseWorld();

    auto playerPos = MyEngine::Vector2Df(15 / 2 * 128.f, 15 / 2 * 128.f);
    auto player = std::make_shared<RogaliqueGame::Player>(playerPos);
    MyEngine::GameObject* playerObj = player->GetGameObject();

    auto hudObj = MyEngine::GameWorld::Instance()->CreateGameObject("HUD");
    auto* health = playerObj->GetComponent<MyEngine::HealthComponent>();
    auto* stamina = playerObj->GetComponent<MyEngine::StaminaComponent>();
    auto* healthTex =
        MyEngine::ResourceSystem::Instance()->GetTextureShared("icon_health");
    auto* staminaTex =
        MyEngine::ResourceSystem::Instance()->GetTextureShared("icon_stamina");
    auto* armorTex =
        MyEngine::ResourceSystem::Instance()->GetTextureShared("icon_armor");
    hudObj->AddComponent<MyEngine::HUDRendererComponent>(
        hudObj, health, stamina, healthTex, staminaTex, armorTex);

    auto* playerInventory = playerObj->AddComponent<RogaliqueGame::InventoryComponent>(playerObj);

    // create a separate GameObject for the inventory UI
    auto* inventoryObj = MyEngine::GameWorld::Instance()->CreateGameObject("InventoryUI");
    inventoryObj->AddComponent<RogaliqueGame::InventoryUIComponent>(inventoryObj, playerInventory);
    auto* keyTex =
        MyEngine::ResourceSystem::Instance()->GetTextureShared("icon_key");
    auto* potionTex =
        MyEngine::ResourceSystem::Instance()->GetTextureShared("icon_potion");

    // Filling up the inventory
    playerInventory->AddItem({"Key", keyTex, 1});
    playerInventory->AddItem({"Health Potion", potionTex, 3});

    RogaliqueGame::SceneManager::Instance()->Init(playerObj);
    RogaliqueGame::SceneManager::Instance()->SetHUD(hudObj);
    RogaliqueGame::SceneManager::Instance()->SetInventoryUI(inventoryObj);

    auto firstLevel = std::make_shared<RogaliqueGame::Level1Scene>();
    RogaliqueGame::SceneManager::Instance()->RequestSwitch(firstLevel.get());
    RogaliqueGame::SceneManager::Instance()->ProcessSwitch();
    if (playerObj) MyEngine::GameWorld::Instance()->BringToFront(playerObj);
    if (hudObj) MyEngine::GameWorld::Instance()->BringToFront(hudObj);
    if (inventoryObj) MyEngine::GameWorld::Instance()->BringToFront(inventoryObj);

    LOG_INFO("Starting game loop...");
    rogaliqueEngine.Run();

    delete window;

    return 0;
}
