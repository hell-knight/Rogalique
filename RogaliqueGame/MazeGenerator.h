#pragma once
#include "LevelScene.h"
#include <vector>
#include <stack>

namespace RogaliqueGame {
class MazeGenerator {
   public:
    MazeGenerator(int width, int height, LevelScene* level);
    void Generate();

    const std::vector<std::vector<bool>>& GetPassabilityGrid() const {
        return grid;
    }

   private:
    int width;
    int height;
    LevelScene* level;
    std::vector<std::vector<bool>> grid;

    std::vector<std::pair<int, int>> GetAvailableDirections(int x, int y);
    void RemoveWall(int x1, int y1, int x2, int y2);
};
}  // namespace RogaliqueGame