#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "Matrix2D.h"
#include "Logger.h"


using namespace RogaliqueGame;

int main()
{
	MyEngine::Engine::Instance();
	LOG_INFO("Logger is alive before resources");
	MyEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "RogaliqueGame"));

	ResourceSystem::Instance()->LoadTextureMap("player", "Resources/Textures/Player.png", { 48, 63 }, 4, false);
	ResourceSystem::Instance()->LoadTextureMap("level_floors", "Resources/Textures/Floor.png", { 16, 16 }, 49, false);
	ResourceSystem::Instance()->LoadTextureMap("level_walls", "Resources/Textures/Wall.png", { 16, 16 }, 48, false);
	ResourceSystem::Instance()->LoadTextureMap("ai", "Resources/Textures/AI.png", { 784 / 3, 1168 / 3 }, 9, false);

	ResourceSystem::Instance()->LoadSound("music", "Resources/Sounds/game_background.ogg");

	auto developerLevel = std::make_shared<DeveloperLevel>();
	developerLevel->Start();

	MyEngine::Engine::Instance()->Run();

	return 0;
}
