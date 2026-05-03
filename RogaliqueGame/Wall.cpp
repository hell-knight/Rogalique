#include "Wall.h"
#include "SpriteColliderComponent.h"
#include "Logger.h"

namespace RogaliqueGame
{
	Wall::Wall(const MyEngine::Vector2Df position, int textureMapIndex)
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Wall");
		auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto* tex = MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("level_walls", textureMapIndex);
		if (!tex) {
			LOG_ERROR("Wall: failed to get wall texture at index " + std::to_string(textureMapIndex));
			MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
			gameObject = nullptr;
			return;
		}

		auto renderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
		renderer->SetTexture(*tex);
		renderer->SetPixelSize(128, 128);

		auto rigidbody = gameObject->AddComponent<MyEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = gameObject->AddComponent<MyEngine::SpriteColliderComponent>();
	}

	MyEngine::Vector2Df Wall::GetPosition() const
	{
		return gameObject->GetComponent<MyEngine::TransformComponent>()->GetWorldPosition();
	}
}