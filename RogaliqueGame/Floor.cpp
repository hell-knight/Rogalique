#include "Floor.h"

namespace RogaliqueGame
{
	Floor::Floor(const MyEngine::Vector2Df& position, int textureMapIndex)
	{
		gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Floor");
		auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
		renderer->SetTexture(*MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared("level_floors", textureMapIndex));
		renderer->SetPixelSize(128, 128);
	}
}