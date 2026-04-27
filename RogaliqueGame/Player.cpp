#include "Player.h"

namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject();
		auto playerRenderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();

		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile("Resources/Textures/ball.png"))
		{
			newTexture->setSmooth(true);
			playerRenderer->SetTexture(*newTexture);
			playerRenderer->SetPixelSize(32, 32);
		}

		auto playerCamera = gameObject->AddComponent<MyEngine::CameraComponent>();
		playerCamera->SetWindow(&MyEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto playerInput = gameObject->AddComponent<MyEngine::InputComponent>();
	}

	MyEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}