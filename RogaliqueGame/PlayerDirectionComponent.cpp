#include "PlayerDirectionComponent.h"
#include "Logger.h"

namespace RogaliqueGame {
PlayerDirectionComponent::PlayerDirectionComponent(
    MyEngine::GameObject* gameObject) : Component(gameObject){
    input = gameObject->GetComponent<MyEngine::InputComponent>();
    dirAnim =
        gameObject->GetComponent<MyEngine::DirectionalAnimationComponent>();
    if (!input || !dirAnim) {
        LOG_ERROR(
            "PlayerDirectionComponent requires Input and DirectionalAnimation. "
            "Removing.");
        gameObject->RemoveComponent(this);
    }
}

void PlayerDirectionComponent::Update(float deltaTime) {
    float x = input->GetHorizontalAxis();
    float y = input->GetVerticalAxis();
    dirAnim->SetMoveDirection(MyEngine::Vector2Df(x, y));
}


}  // namespace RogaliqueGame