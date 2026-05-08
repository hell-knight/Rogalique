#pragma once
#include "LevelScene.h"

namespace RogaliqueGame {
class Level2Scene : public LevelScene {
   public:
    void Start() override;
    void Restart() override {
        Stop();
        Start();
    }
};
}  // namespace RogaliqueGame