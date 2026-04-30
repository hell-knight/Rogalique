#pragma once
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "GameObject.h"

namespace RogaliqueGame
{
	class Wall
	{
	public:
		Wall(const MyEngine::Vector2Df position, int textureMapIndex);

	private:
		MyEngine::GameObject* gameObject;
	};
}