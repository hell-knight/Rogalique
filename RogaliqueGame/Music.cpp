#include "Music.h"
#include "Logger.h"

namespace RogaliqueGame {
Music::Music(const std::string& soundName) {
    auto gameObject = MyEngine::GameWorld::Instance()->CreateGameObject(
        "Music: " + soundName);
    auto* soundBuf = MyEngine::ResourceSystem::Instance()->GetSound(soundName);
    if (!soundBuf) {
        LOG_ERROR("Music: sound '" + soundName + "' not found.");
        MyEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        gameObject = nullptr;
        return;
    }
    auto music = gameObject->AddComponent<MyEngine::AudioComponent>();
    music->SetAudio(*soundBuf);
    music->SetLoop(true);
    music->Play();
}
}  // namespace RogaliqueGame