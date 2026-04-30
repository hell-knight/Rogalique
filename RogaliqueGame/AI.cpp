#include "AI.h"
#include <MovementComponent.h>
#include "SpriteDirectionComponent.h"
#include "SpriteAnimationComponent.h"
#include "FollowComponent.h"
#include "SpriteColliderComponent.h"

namespace RogaliqueGame
{
	AI::AI(const MyEngine::Vector2Df& position, MyEngine::GameObject* player)
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("AI");
		auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
		renderer->SetTexture(*MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("ai", 0));
		renderer->SetPixelSize(100, 100);

		auto follower = gameObject->AddComponent<MyEngine::FollowComponent>();
		follower->SetTarget(player);
		follower->SetSpeed(120.f);

		auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		auto collider = gameObject->AddComponent<MyEngine::SpriteColliderComponent>();

		auto animator = gameObject->AddComponent<MyEngine::SpriteAnimationComponent>();
		animator->Initialize("ai", 6.f);
	}

	MyEngine::GameObject* AI::GetGameObject()
	{
		return gameObject;
	}
}