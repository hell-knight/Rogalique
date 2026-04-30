#include "Player.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include <MovementComponent.h>
#include <SpriteDirectionComponent.h>
#include "SpriteAnimationComponent.h"

namespace RogaliqueGame
{
	Player::Player(const MyEngine::Vector2Df& position)
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Player");
		auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
		transform->SetWorldPosition(position);
		//gameObject->AddComponent<MyEngine::RigidbodyComponent>();
		auto renderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
		renderer->SetTexture(*MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		renderer->SetPixelSize(100, 100);

		auto camera = gameObject->AddComponent<MyEngine::CameraComponent>();
		camera->SetWindow(&MyEngine::RenderSystem::Instance()->GetMainWindow());
		camera->SetBaseResolution(1280, 720);
		camera->ZoomBy(2.f);

		auto input = gameObject->AddComponent<MyEngine::InputComponent>();

		auto movement = gameObject->AddComponent<MyEngine::MovementComponent>();
		movement->SetSpeed(400.f);

		auto spriteDirection = gameObject->AddComponent<MyEngine::SpriteDirectionComponent>();

		auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		auto collider = gameObject->AddComponent<MyEngine::SpriteColliderComponent>();

		auto animator = gameObject->AddComponent<MyEngine::SpriteAnimationComponent>();
		animator->Initialize("player", 6.f);
	}

	MyEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}