#include "pch.h"
#include "FollowComponent.h"
#include "iostream"

namespace MyEngine
{
	FollowComponent::FollowComponent(GameObject* gameObject) : Component(gameObject)
	{
		transform = gameObject->GetComponent<MyEngine::TransformComponent>();
		spriteRenderer = gameObject->GetComponent<MyEngine::SpriteRendererComponent>();
		if (transform == nullptr)
		{
			LOG_ERROR("FollowComponent requires TransformComponent. Removing.");
			gameObject->RemoveComponent(this);
		}
	}

	void FollowComponent::Update(float deltaTime)
	{
		if (transform == nullptr || targetTransform == nullptr)
		{
			return;
		}

		Vector2Df currentPos = transform->GetWorldPosition();
		Vector2Df targetPos = targetTransform->GetWorldPosition();
		Vector2Df direction = targetPos - currentPos;

		float length = direction.GetLength();

		if (length > 0.001f)
		{
			Vector2Df normalized = Vector2Df(direction.x / length, direction.y / length);
			transform->MoveBy(normalized * speed * deltaTime);

			if (spriteRenderer)
			{
				spriteRenderer->FlipX(direction.x < 0);
			}
		}
	}

	void FollowComponent::Render()
	{
	}

	void FollowComponent::SetTarget(GameObject* targetObject)
	{
		if (!targetObject)
		{
			LOG_WARN("FollowComponent: SetTarget called with nullptr.");
			targetTransform = nullptr;
			return;
		}
		targetTransform = targetObject->GetComponent<TransformComponent>();
		if (targetTransform)
		{
			LOG_INFO("FollowComponent: target set to " + targetObject->GetName());
		}
		else
		{
			LOG_ERROR("FollowComponent: target object has no TransformComponent.");
		}
	}

	void FollowComponent::SetSpeed(float newSpeed)
	{
		speed = newSpeed;
	}

	float FollowComponent::GetSpeed() const
	{
		return speed;
	}


}