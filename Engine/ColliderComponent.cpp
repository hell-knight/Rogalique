#include "pch.h"
#include "ColliderComponent.h"

namespace MyEngine
{
	ColliderComponent::ColliderComponent(GameObject* gameObject) : Component(gameObject)
	{

	}

	void ColliderComponent::SetTrigger(bool newIsTrigger)
	{
		isTrigger = newIsTrigger;
	}

	void ColliderComponent::SubscribeCollision(std::function<void(const Collision&)> onCollisionAction)
	{
		onCollisionActions.push_back(onCollisionAction);
	}
	void ColliderComponent::UnsubscribeCollision(std::function<void(const Collision&)> onCollisionAction)
	{
		onCollisionActions.erase(std::remove_if
		(
			onCollisionActions.begin(),
			onCollisionActions.end(),
			[&onCollisionAction](const std::function<void(Collision&)>& action)
			{
				return action.target<void(const Collision&)>() == onCollisionAction.target<void(const Collision&)>();
			}
		), onCollisionActions.end());
	}

	void ColliderComponent::SubscribeTriggerEnter(std::function<void(const Trigger&)> onTriggerEnterAction)
	{
		onTriggerEnterActions.push_back(onTriggerEnterAction);
	}
	void ColliderComponent::UnsubscribeTriggerEnter(std::function<void(const Trigger&)> onTriggerEnterAction)
	{
		onTriggerEnterActions.erase(std::remove_if
		(
			onTriggerEnterActions.begin(),
			onTriggerEnterActions.end(),
			[&onTriggerEnterAction](const std::function<void(const Trigger&)>& action)
			{
				return action.target<void(const Trigger&)>() == onTriggerEnterAction.target<void(const Trigger&)>();
			}
		), onTriggerEnterActions.end());
	}

	void ColliderComponent::SubscribeTriggerExit(std::function<void(const Trigger&)> onTriggerExitAction)
	{
		onTriggerExitActions.push_back(onTriggerExitAction);
	}
	void ColliderComponent::UnsubscribeTriggerExit(std::function<void(const Trigger&)> onTriggerExitAction)
	{
		onTriggerExitActions.erase(std::remove_if
		(
			onTriggerExitActions.begin(),
			onTriggerExitActions.end(),
			[&onTriggerExitAction](const std::function<void(const Trigger&)>& action)
			{
				return action.target<void(const Trigger&)>() == onTriggerExitAction.target<void(const Trigger&)>();
			}
		), onTriggerExitActions.end());
	}

	void ColliderComponent::OnCollision(const Collision& collision)
	{
		for (auto& action : onCollisionActions)
		{
			action(collision);
		}
	}
	void ColliderComponent::OnTriggerEnter(const Trigger& trigger)
	{
		for (auto& action : onTriggerEnterActions)
		{
			action(trigger);
		}
	}
	void ColliderComponent::OnTriggerExit(const Trigger& trigger)
	{
		for (auto& action : onTriggerExitActions)
		{
			action(trigger);
		}
	}
}