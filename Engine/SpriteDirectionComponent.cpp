#include "pch.h"
#include "SpriteDirectionComponent.h"

namespace MyEngine
{
	SpriteDirectionComponent::SpriteDirectionComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		input = gameObject->GetComponent<InputComponent>();
		spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
	}

	void SpriteDirectionComponent::Update(float deltaTime)
	{
		if (input->GetHorizontalAxis() < 0)
		{
			spriteRenderer->FlipX(true);
		}
		if (input->GetHorizontalAxis() > 0)
		{
			spriteRenderer->FlipX(false);
		}
	}

	void SpriteDirectionComponent::Render()
	{
	}
}