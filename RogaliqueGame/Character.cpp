#include "Character.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "Logger.h"

namespace RogaliqueGame {
Character::Character(const MyEngine::Vector2Df& position,
                     MyEngine::GameObject* player, const std::string& name)
    : player(player) {
    gameObject = MyEngine::GameWorld::Instance()->CreateGameObject(name);
    auto transform = gameObject->GetComponent<MyEngine::TransformComponent>();
    transform->SetWorldPosition(position);
}

Character::~Character() {
    // The GameObject will be destroyed via GameWorld unless it is deleted first
}

MyEngine::GameObject* Character::GetGameObject() const { return gameObject; }

}  // namespace RogaliqueGame
