#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"


using namespace RogaliqueGame;

int main()
{
	MyEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "RogaliqueGame"));

	MyEngine::ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");

	auto developerLevel = std::make_shared<DeveloperLevel>();
	developerLevel->Start();

	MyEngine::Engine::Instance()->Run();

	return 0;
}
