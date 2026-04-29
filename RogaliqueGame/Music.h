#pragma once
#include <GameObject.h>
#include <GameWorld.h>
#include <AudioComponent.h>
#include <ResourceSystem.h>

namespace RogaliqueGame
{
	class Music
	{
	public:
		Music(const std::string& soundName);

	private:
		MyEngine::GameObject* gameObject;
	};
}