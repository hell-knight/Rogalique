#pragma once

namespace MyEngine
{
	class ColliderComponent;
	struct Trigger
	{
	public:
		ColliderComponent* first;
		ColliderComponent* second;
		Trigger(ColliderComponent* newFirst, ColliderComponent* newSecond) :
			first(newFirst), second(newSecond) {
		};
	};
}