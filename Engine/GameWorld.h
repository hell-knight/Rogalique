#pragma once
#include "GameObject.h"
#include "EngineAPI.h"
#include "PhysicsSystem.h"

namespace MyEngine {
class ENGINE_API GameWorld {
   public:
    static GameWorld* Instance();

    void Update(float deltaTime);
    void FixedUpdate(float deltaTime);
    void Render();
    void LateUpdate();

    GameObject* CreateGameObject();
    GameObject* CreateGameObject(std::string name);
    void DestroyGameObject(GameObject* gameObject);
    void Clear();

    void Print() const;

    const std::vector<GameObject*>& GetAllGameObjects() const {
        return gameObjects;
    }

    void BringToFront(GameObject* obj);

   private:
    GameWorld() {}
    ~GameWorld() {}

    GameWorld(GameWorld const&) = delete;
    GameWorld& operator=(GameWorld const&) = delete;

    float fixedCounter = 0.f;

    std::vector<GameObject*> gameObjects = {};
    std::vector<GameObject*> markedToDestroyGameObjects = {};

    void DestroyGameObjectImmediate(GameObject* gameObject);
};
}  // namespace MyEngine