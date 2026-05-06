#pragma once
#include "LevelScene.h"
#include <memory>

namespace RogaliqueGame {
class Level1Scene : public LevelScene {
   public:
    void Start() override;
    void Restart() override {
        Stop();
        Start();
    }
};
}
