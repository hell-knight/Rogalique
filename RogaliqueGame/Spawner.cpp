#include "Spawner.h"
#include "Character.h"
#include "randomizer.h"

namespace RogaliqueGame {
Spawner::Spawner(SpawnCallBack callback) : create_(std::move(callback)) {}

std::vector<std::shared_ptr<Character>> Spawner::SpawnRandom(
    int count, const std::vector<MyEngine::Vector2Df>& validPositions) {
    std::vector<std::shared_ptr<Character>> spawned;
    if (validPositions.empty()) {
        return spawned;
    }

    count = std::min(count, static_cast<int>(validPositions.size()));

    std::vector<MyEngine::Vector2Df> positions = validPositions;

    for (int i = 0; i < count; ++i) {
        int idx = random(0, static_cast<int>(positions.size()) - 1);

        auto character = create_(positions[idx]);
        if (character) {
            spawned.push_back(character);
        }
        // remove the used unit so that there aren't two enemies in the same
        // cell
        positions.erase(positions.begin() + idx);
    }

    return spawned;
}
}  // namespace RogaliqueGame