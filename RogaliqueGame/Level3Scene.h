#pragma once
#include "LevelScene.h"

namespace RogaliqueGame {
class Level3Scene : public LevelScene {
   public:
    void Start() override;
    void Restart() override {
        Stop();
        Start();
    }
};
}  // namespace RogaliqueGame