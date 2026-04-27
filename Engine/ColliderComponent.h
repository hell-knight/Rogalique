#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Component.h"
#include "Collision.h"
#include "Trigger.h"
#include "PhysicsSystem.h"
#include "EngineAPI.h"

namespace MyEngine
{
	class ENGINE_API ColliderComponent : public Component
	{
	public:
		ColliderComponent(GameObject* gameObject);

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		void SetTrigger(bool newIsTrigger);

		void SubscribeCollision(std::function<void(const Collision&)> onCollisionAction);
		void UnsubscribeCollision(std::function<void(const Collision&)> onCollisionAction);

		void SubscribeTriggerEnter(std::function<void(const Trigger&)> onTriggerEnterAction);
		void UnsubscribeTriggerEnter(std::function<void(const Trigger&)> onTriggerEnterAction);

		void SubscribeTriggerExit(std::function<void(const Trigger&)> onTriggerExitAction);
		void UnsubscribeTriggerExit(std::function<void(const Trigger&)> onTriggerExitAction);

		friend class PhysicsSystem;

	protected:
		sf::FloatRect bounds;
		bool isTrigger = false;

		void OnCollision(const Collision& collision);
		void OnTriggerEnter(const Trigger& trigger);
		void OnTriggerExit(const Trigger& trigger);

		std::vector<std::function<void(const Collision&)>> onCollisionActions;
		std::vector<std::function<void(const Trigger&)>> onTriggerEnterActions;
		std::vector<std::function<void(const Trigger&)>> onTriggerExitActions;
	};
}