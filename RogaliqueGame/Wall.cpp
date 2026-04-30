#include "Wall.h"
#include "SpriteColliderComponent.h"

namespace RogaliqueGame
{
	Wall::Wall(const MyEngine::Vector2Df position, int textureMapIndex)
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Wall");
		auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
		renderer->SetTexture(*MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("level_walls", textureMapIndex));
		renderer->SetPixelSize(128, 128);

		auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = gameObject->AddComponent<MyEngine::SpriteColliderComponent>();
	}
}