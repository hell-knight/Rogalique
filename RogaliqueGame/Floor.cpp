#include "Floor.h"
#include "Logger.h"

namespace RogaliqueGame {
Floor::Floor(const MyEngine::Vector2Df& position, int textureMapIndex) {
    gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Floor");
    auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
    transform->SetWorldPosition(position);

    auto* tex =
        MyEngine::ResourceSystem::Instance()->GetTextureMapElementShared(
            "level_floors", textureMapIndex);
    if (!tex) {
        LOG_ERROR("Floor: failed to get floor texture at index " +
                  std::to_string(textureMapIndex));
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
        return;
    }

    auto renderer =
        gameObject->AddComponent<MyEngine::SpriteRendererComponent>();
    renderer->SetTexture(*tex);
    renderer->SetPixelSize(128, 128);
}

MyEngine::Vector2Df Floor::GetPosition() const {
    return gameObject->GetComponent<MyEngine::TransformComponent>()
        ->GetWorldPosition();
}
}  // namespace RogaliqueGame