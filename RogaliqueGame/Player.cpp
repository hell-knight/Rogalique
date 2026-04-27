#include "Player.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"

namespace RogaliqueGame
{
	Player::Player()
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Player");
		gameObject->AddComponent<MyEngine::RigidbodyComponent>();
		auto playerRenderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();

		playerRenderer->SetTexture(*MyEngine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = gameObject->AddComponent<MyEngine::CameraComponent>();
		playerCamera->SetWindow(&MyEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto playerInput = gameObject->AddComponent<MyEngine::InputComponent>();

		auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();

		auto collider = gameObject->AddComponent<MyEngine::SpriteColliderComponent>();
	}

	MyEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}