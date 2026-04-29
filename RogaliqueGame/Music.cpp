#include "Music.h"

namespace RogaliqueGame
{
	Music::Music(const std::string& soundName)
	{
		auto gameObject = MyEngine::GameWorld::Instance()->CreateGameObject("Music: " + soundName);
		auto music = gameObject->AddComponent<MyEngine::AudioComponent>();
		music->SetAudio(*MyEngine::ResourceSystem::Instance()->GetSound(soundName));
		music->SetLoop(true);
		music->Play();
	}
}