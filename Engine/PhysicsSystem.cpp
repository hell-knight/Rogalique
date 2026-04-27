#include "pch.h"
#include "PhysicsSystem.h"

namespace MyEngine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		for (int i = 0; i < colliders.size(); i++)
		{
			auto body = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
			if (body->GetKinematic())
			{
				continue;
			}

			for (int j = 0; j < colliders.size(); j++)
			{
				if (j == i)
				{
					continue;
				}

				sf::FloatRect intersection;
				if (colliders[i]->bounds.intersects(colliders[j]->bounds, intersection))
				{
					if (colliders[i]->isTrigger != colliders[j]->isTrigger)
					{
						if (triggersEnteredPair.find(colliders[i]) == triggersEnteredPair.end() && triggersEnteredPair.find(colliders[j]) == triggersEnteredPair.end())
						{
							Trigger triggerEnter(colliders[i], colliders[j]);
							colliders[i]->OnTriggerEnter(triggerEnter);
							colliders[j]->OnTriggerEnter(triggerEnter);

							triggersEnteredPair.emplace(colliders[i], colliders[j]);
						}
					}
					else if (!colliders[i]->isTrigger)
					{
						float intersectionWidth = intersection.width;
						float intersectionHeight = intersection.height;
						Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };

						Vector2Df aPosition = { colliders[i]->bounds.left,  colliders[i]->bounds.top };
						auto aTransform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

						if (intersectionWidth > intersectionHeight)
						{
							if (intersectionPosition.y > aPosition.y)
							{
								aTransform->MoveBy({ 0, -intersectionHeight });
								std::cout << "Top collision" << std::endl;
							}
							else
							{
								aTransform->MoveBy({ 0, intersectionHeight });
								std::cout << "Down collision" << std::endl;
							}
						}
						else
						{
							if (intersectionPosition.x > aPosition.x)
							{
								aTransform->MoveBy({ -intersectionWidth, 0.f });
								std::cout << "Right collision" << std::endl;
							}
							else
							{
								aTransform->MoveBy({ intersectionWidth, 0.f });
								std::cout << "Left collision" << std::endl;
							}
						}

						Collision collision(colliders[i], colliders[j], intersection);
						colliders[i]->OnCollision(collision);
						colliders[j]->OnCollision(collision);
					}
				}
			}
		}

		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (!triggeredPair->first->bounds.intersects(triggeredPair->second->bounds))
			{
				Trigger triggerExit(triggeredPair->first, triggeredPair->second);
				triggeredPair->first->OnTriggerExit(triggerExit);
				triggeredPair->second->OnTriggerExit(triggerExit);

				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		std::cout << "Subscribe " << collider << std::endl;
		colliders.push_back(collider);
	}
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		std::cout << "Unsubscribe " << collider << std::endl;

		colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());
	}
}