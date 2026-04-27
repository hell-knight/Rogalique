#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"


using namespace RogaliqueGame;

int main()
{
	MyEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "RogaliqueGame"));

	auto player = std::make_shared<Player>();

	MyEngine::Engine::Instance()->Run();

	return 0;
}
