#include "DeveloperLevel.h"
#include "Logger.h"

using namespace MyEngine;

namespace RogaliqueGame
{
	void DeveloperLevel::Start()
	{
		LOG_INFO("Starting DeveloperLevel...");
		int width = 15;
		int height = 15;

		for (int y = 0; y < height + 1; y++)
		{
			for (int x = 0; x < width + 1; x++)
			{
				//if not wall place
				if (x != 0 && x != width && y != 0 && y != height)
				{
					floors.push_back(std::make_unique<Floor>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 0));
				}

				//if left-bottom corner
				if (x == 0 && y == 0)
				{
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 25));
				}

				//if right-bottom corner
				if (x == width && y == 0)
				{
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 27));
				}

				//if left-top corner
				if (x == 0 && y == height)
				{
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 1));
				}

				//if right-top corner
				if (x == width && y == height)
				{
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 3));
				}

				//if left (not corner)
				if (x == 0 && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 18));
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df( x * 128.f, y * 128.f ), 12));
				}

				//if right (not corner)
				if (x == width && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(MyEngine::Vector2Df(x * 128.f, y * 128.f), 19));
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df(x * 128.f, y * 128.f), 12));
				}

				//if bottom (not corner)
				if (y == 0 && x != width && x != 0)
				{
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df(x * 128.f, y * 128.f), 38));
				}

				//if top (not corner)
				if (y == height && x != width && x != 0)
				{
					walls.push_back(std::make_unique<Wall>(MyEngine::Vector2Df(x * 128.f, y * 128.f), 38));
				}
			}
		}
		LOG_INFO("Generated " + std::to_string(walls.size()) + " walls and " + std::to_string(floors.size()) + " floors");

		auto playerPos = MyEngine::Vector2Df({ width / 2 * 128.f, height / 2 * 128.f });
		player = std::make_unique<Player>(playerPos);
		LOG_INFO("Player created at (" + std::to_string(playerPos.x) + ", " + std::to_string(playerPos.y) + ")");

		auto aiPos = MyEngine::Vector2Df((width / 2 + 2) * 128.f, (height / 2) * 128.f);
		ai = std::make_unique<AI>(aiPos, player->GetGameObject());
		LOG_INFO("AI created at (" + std::to_string(aiPos.x) + ", " + std::to_string(aiPos.y) + ")");
		music = std::make_unique<Music>("music");
	}

	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}

	void DeveloperLevel::Stop()
	{
		GameWorld::Instance()->Clear();
	}
}