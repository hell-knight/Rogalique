#include "BaseLevel.h"
#include "Wall.h"
#include "Floor.h"
#include "Vector.h"

namespace RogaliqueGame {
void CreateBaseWorld(){
    const int width = 15, height = 15;

    for (int y = 0; y < height + 1; y++) {
        for (int x = 0; x < width + 1; x++) {
            // if not wall place
            if (x != 0 && x != width && y != 0 && y != height) {
                new Floor(MyEngine::Vector2Df(x * 128.f, y * 128.f), 0);
            }

            // if left-bottom corner
            if (x == 0 && y == 0) {
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 25);
            }

            // if right-bottom corner
            if (x == width && y == 0) {
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 27);
            }

            // if left-top corner
            if (x == 0 && y == height) {
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 1);
            }

            // if right-top corner
            if (x == width && y == height) {
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 3);
            }

            // if left (not corner)
            if (x == 0 && y != height && y != 0) {
                new Floor(MyEngine::Vector2Df(x * 128.f, y * 128.f), 18);
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 12);
            }

            // if right (not corner)
            if (x == width && y != height && y != 0) {
                new Floor(MyEngine::Vector2Df(x * 128.f, y * 128.f), 19);
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 12);
            }

            // if bottom (not corner)
            if (y == 0 && x != width && x != 0) {
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 38);
            }

            // if top (not corner)
            if (y == height && x != width && x != 0) {
                new Wall(MyEngine::Vector2Df(x * 128.f, y * 128.f), 38);
            }
        }
    }
}
}