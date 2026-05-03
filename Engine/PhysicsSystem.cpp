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
			if (!body)
			{
				LOG_WARN("Collider without RigidbodyComponent on object '" +
					colliders[i]->GetGameObject()->GetName() + "', treating as kinematic.");
				continue;
			}
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
						auto pair = std::make_pair(colliders[i], colliders[j]);
						if (triggersEnteredSet.find(pair) == triggersEnteredSet.end())
						{
							Trigger triggerEnter(colliders[i], colliders[j]);
							colliders[i]->OnTriggerEnter(triggerEnter);
							colliders[j]->OnTriggerEnter(triggerEnter);

							triggersEnteredSet.insert(pair);
							LOG_INFO("Trigger entered between '" +
								colliders[i]->GetGameObject()->GetName() + "' and '" +
								colliders[j]->GetGameObject()->GetName() + "'");
						}
					}
					else if (!colliders[i]->isTrigger)
					{
						// simple collision
						float intersectionWidth = intersection.width;
						float intersectionHeight = intersection.height;
						Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };

						Vector2Df aPosition = { colliders[i]->bounds.left,  colliders[i]->bounds.top };
						auto aTransform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

						// Determine the direction of the collision and push the object away
						std::string collisionDir;
						if (intersectionWidth > intersectionHeight)
						{
							if (intersectionPosition.y > aPosition.y)
							{
								aTransform->MoveBy({ 0, -intersectionHeight });
								collisionDir = "Top";
							}
							else
							{
								aTransform->MoveBy({ 0, intersectionHeight });
								collisionDir = "Down";
							}
						}
						else
						{
							if (intersectionPosition.x > aPosition.x)
							{
								aTransform->MoveBy({ -intersectionWidth, 0.f });
								collisionDir = "Right";
							}
							else
							{
								aTransform->MoveBy({ intersectionWidth, 0.f });
								collisionDir = "Left";
							}
						}

						Collision collision(colliders[i], colliders[j], intersection);
						colliders[i]->OnCollision(collision);
						colliders[j]->OnCollision(collision);

						LOG_INFO("Collision (" + collisionDir + ") between '" +
							colliders[i]->GetGameObject()->GetName() + "' and '" +
							colliders[j]->GetGameObject()->GetName() + "'");
					}
				}
			}
		}

		std::vector<std::pair<ColliderComponent*, ColliderComponent*>> exitedPairs;
		for (const auto& p : triggersEnteredSet)
		{
			if (!p.first->bounds.intersects(p.second->bounds))
			{
				Trigger triggerExit(p.first, p.second);
				p.first->OnTriggerExit(triggerExit);
				p.second->OnTriggerExit(triggerExit);

				exitedPairs.push_back(p);
				LOG_INFO("Trigger exited between '" +
					p.first->GetGameObject()->GetName() + "' and '" +
					p.second->GetGameObject()->GetName() + "'");
			}
		}
		for (const auto& p : exitedPairs)
		{
			triggersEnteredSet.erase(p);
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