#include "DeveloperLevel.h"

using namespace MyEngine;

namespace RogaliqueGame
{
	void DeveloperLevel::Start()
	{
		player = std::make_shared<Player>();
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