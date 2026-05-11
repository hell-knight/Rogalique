#include "MazeGenerator.h"
#include "Wall.h"
#include "Floor.h"
#include "randomizer.h"

namespace RogaliqueGame {
// Constructor: Initializes the maze generator with the given dimensions and
// level reference.
MazeGenerator::MazeGenerator(int width, int height, LevelScene* level)
    : width(width), height(height), level(level) {
    // Resize the grid to match the maze dimensions and initialize all cells as
    // unvisited (false).
    grid.resize(height, std::vector<bool>(width, false));
}

// Generate: Creates a maze using the Depth-First Search (DFS) algorithm.
void MazeGenerator::Generate() {
    // Seed the random number generator for consistent randomness.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Start from a random cell in the grid.
    int startX = random(2, width - 3);
    int startY = random(2, height - 3);

    // Use a stack to keep track of visited cells during DFS.
    std::stack<std::pair<int, int>> stack;
    stack.push({startX, startY});
    grid[startY][startX] = true;  // Mark the starting cell as visited.

    // Continue until the stack is empty (all cells are processed).
    while (!stack.empty()) {
        // Get the current cell from the top of the stack.
        auto [x, y] = stack.top();
        stack.pop();

        // Get all available directions (unvisited neighboring cells) from the
        // current cell.
        std::vector<std::pair<int, int>> directions =
            GetAvailableDirections(x, y);

        // If there are available directions, process them.
        if (!directions.empty()) {
            // Push the current cell back onto the stack to revisit later.
            stack.push({x, y});

            // Randomly select one of the available directions.
            int idx = random(0, static_cast<int>(directions.size()) - 1);
            auto dir = directions[idx];
            int nx = x + dir.first;
            int ny = y + dir.second;

            // Remove the wall between the current cell and the selected
            // neighbor.
            RemoveWall(x, y, nx, ny);

            // Mark the neighbor as visited and push it onto the stack for
            // further exploration.
            grid[ny][nx] = true;
            stack.push({nx, ny});
        }
    }
}

// GetAvailableDirections: Returns a list of valid, unvisited neighboring cells.
std::vector<std::pair<int, int>> MazeGenerator::GetAvailableDirections(int x,
                                                                       int y) {
    // Define possible directions: up, down, left, right (2 cells away to leave
    // space for walls).
    std::vector<std::pair<int, int>> directions = {
        {0, -2},  // Up
        {0, 2},   // Down
        {-2, 0},  // Left
        {2, 0}    // Right
    };

    std::vector<std::pair<int, int>> available;

    // Check each direction to see if it leads to a valid, unvisited cell.
    for (const auto& dir : directions) {
        int nx = x + dir.first;
        int ny = y + dir.second;

        // Ensure the neighbor is within bounds and unvisited.
        if (nx > 0 && nx < width && ny > 0 && ny < height && !grid[ny][nx]) {
            available.push_back(dir);
        }
    }

    return available;
}

// RemoveWall: Removes the wall between two cells and adds floors and walls to
// the level.
void MazeGenerator::RemoveWall(int x1, int y1, int x2, int y2) {
    // Calculate the position of the wall between the two cells.
    int wallX = (x1 + x2) / 2;
    int wallY = (y1 + y2) / 2;

    // Add a wall at the midpoint if the cells are not directly adjacent.
    if (wallX != x1 || wallY != y1) {
        level->AddWall(std::make_unique<Wall>(
            MyEngine::Vector2Df{wallX * 128.f, wallY * 128.f}, 14));
    }
}

}  // namespace RogaliqueGame