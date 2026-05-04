#include "pch.h"
#include "MovementComponent.h"

namespace MyEngine {
MovementComponent::MovementComponent(GameObject* gameObject)
    : Component(gameObject) {
    input = gameObject->GetComponent<InputComponent>();
    transform = gameObject->GetComponent<TransformComponent>();
    stamina = gameObject->GetComponent<StaminaComponent>();

    if (transform == nullptr) {
        LOG_ERROR("MovementComponent requires TransformComponent. Removing.");
        gameObject->RemoveComponent(this);
        return;
    }

    if (input == nullptr) {
        LOG_WARN(
            "MovementComponent: InputComponent not found. Movement via keys "
            "disabled.");
        // gameObject->RemoveComponent(this);
    }
}

void MovementComponent::Update(float deltaTime) {
    float xAxis = input->GetHorizontalAxis();
    float yAxis = input->GetVerticalAxis();

    Vector2Df dir = {xAxis, yAxis};
    if (dir.x != 0.f || dir.y != 0.f) {
        LOG_INFO("MovementComponent moving (" + std::to_string(dir.x) + ", " +
                 std::to_string(dir.y) + ")");
    }

    float len = dir.GetLength();
    if (len > 0.01f) {
        // Movement: stamina if the component is available
        if (stamina) stamina->Use(10.f * deltaTime);
    } else {
        // stand: recovering
        if (stamina) stamina->Restore(15.f * deltaTime);
    }

    transform->MoveBy(speed * deltaTime * dir);

    acceleration = transform->GetWorldPosition() - previousPosition;
    previousPosition = transform->GetWorldPosition();
}

void MovementComponent::Render() {}

void MovementComponent::SetSpeed(float newSpeed) { speed = newSpeed; }

float MovementComponent::GetSpeed() const { return speed; }

float MovementComponent::GetAccelerationSquared() const {
    return acceleration.x * acceleration.x + acceleration.y * acceleration.y;
}
}  // namespace MyEngine