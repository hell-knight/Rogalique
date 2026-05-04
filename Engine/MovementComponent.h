#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "StaminaComponent.h"
#include "EngineAPI.h"

namespace MyEngine
{
	class ENGINE_API MovementComponent : public Component
	{
	public:
		MovementComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetSpeed(float newSpeed);
		float GetSpeed() const;
		float GetAccelerationSquared() const;
	private:
		InputComponent* input;
		TransformComponent* transform;
        StaminaComponent* stamina = nullptr;

		float speed = 0;
		Vector2Df previousPosition = { 0, 0 };
		Vector2Df acceleration = { 0, 0 };
	};
}