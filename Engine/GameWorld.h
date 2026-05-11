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

    // Returns a list of raw pointers, as before,
    // but now the owners are shared_ptrs.
    std::vector<GameObject*> GetAllGameObjects() const;

    void BringToFront(GameObject* obj);

    void InsertBefore(GameObject* obj, GameObject* beforeThis);

   private:
    GameWorld() {}
    ~GameWorld() {}

    GameWorld(GameWorld const&) = delete;
    GameWorld& operator=(GameWorld const&) = delete;

    float fixedCounter = 0.f;

    // Now GameWorld owns the objects via shared_ptr
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<GameObject*> markedToDestroyGameObjects;

    void DestroyGameObjectImmediate(GameObject* gameObject);
};
}  // namespace MyEngine