#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "Vector.h"

namespace RogaliqueGame
{
	class Character;

	class Spawner
	{
	public:
		// Factory: takes a position, returns the created character
		using SpawnCallBack = std::function<std::shared_ptr<Character>(const MyEngine::Vector2Df)>;

		explicit Spawner(SpawnCallBack callback);

		// Spawn a specified number of enemies at random points on the map (dimensions in tiles)
		// Returns the created characters (so they can be controlled)
		std::vector<std::shared_ptr<Character>> SpawnRandom(int count, const std::vector<MyEngine::Vector2Df>& validPositions);

	private:
		SpawnCallBack create_;
	};
}
