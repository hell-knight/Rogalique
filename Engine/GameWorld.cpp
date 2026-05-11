#include "pch.h"
#include "GameWorld.h"
#include "Logger.h"
#include <algorithm>
#include <unordered_set>

namespace MyEngine {
GameWorld* GameWorld::Instance() {
    static GameWorld world;
    return &world;
}

void GameWorld::Update(float deltaTime) {
    for (auto& gObj : gameObjects) {
        gObj->Update(deltaTime);
    }
}

void GameWorld::FixedUpdate(float deltaTime) {
    fixedCounter += deltaTime;
    if (fixedCounter > PhysicsSystem::Instance()->GetFixedDeltaTime()) {
        fixedCounter -= PhysicsSystem::Instance()->GetFixedDeltaTime();
        PhysicsSystem::Instance()->Update();
    }
}

void GameWorld::Render() {
    for (auto& gObj : gameObjects) {
        gObj->Render();
    }
}

void GameWorld::LateUpdate() {
    // --- Safely delete selected items ---
    // take a copy to prevent the vector from being modified during the iteration
    std::vector<GameObject*> toDestroy = markedToDestroyGameObjects;
    markedToDestroyGameObjects.clear();

    for (GameObject* go : toDestroy) {
        // check whether the object still exists (it may have already been deleted as a descendant of another object)
        auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                               [go](const std::shared_ptr<GameObject>& sp) {
                                   return sp.get() == go;
                               });
        if (it != gameObjects.end()) {
            DestroyGameObjectImmediate(go);
        }
    }

    // --- Automatic deletion upon expiration (IsExpired) ---
    for (int i = (int)gameObjects.size() - 1; i >= 0; --i) {
        auto& go = gameObjects[i];
        bool expired = false;
        for (const auto* comp : go->GetAllComponents()) {
            if (comp->IsExpired()) {
                expired = true;
                break;
            }
        }
        if (expired) {
            DestroyGameObject(go.get());  // will be removed in the next frame
        }
    }
}

GameObject* GameWorld::CreateGameObject() {
    auto gObject =  std::make_shared<GameObject>();
    gameObjects.push_back(gObject);
    return gObject.get();
}

GameObject* GameWorld::CreateGameObject(std::string name) {
    auto gObject = std::make_shared<GameObject>(name);
    gameObjects.push_back(gObject);
    return gObject.get();
}

void GameWorld::DestroyGameObject(GameObject* gameObject) {
    // Safety net: if the object isn't in the storage, we exit
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [gameObject](const std::shared_ptr<GameObject>& sp) {
                               return sp.get() == gameObject;
                           });
    if (it == gameObjects.end()) return;

    // Check that it hasn't already been marked
    if (std::find(markedToDestroyGameObjects.begin(),
                  markedToDestroyGameObjects.end(),
                  gameObject) == markedToDestroyGameObjects.end()) {
        markedToDestroyGameObjects.push_back(gameObject);
    }
}

void GameWorld::Clear() {
    // First, let's reset the parent-child relationships so that the destructors
    // don't try to delete the children again
    for (auto& go : gameObjects) {
        auto* transform = go->GetComponent<TransformComponent>();
        if (transform && transform->GetParent()) {
            transform->SetParent(nullptr);
        }
    }
    gameObjects.clear();
    markedToDestroyGameObjects.clear();
    fixedCounter = 0.f;
}

void GameWorld::Print() const {
    for (auto& obj : gameObjects) {
        if (obj == nullptr) {
            continue;
        }
        if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr) {
            obj->Print();
        }
    }
}

std::vector<GameObject*> GameWorld::GetAllGameObjects() const {
    std::vector<GameObject*> raw;
    raw.reserve(gameObjects.size());
    for (const auto& go : gameObjects) {
        raw.push_back(go.get());
    }
    return raw;
}

void GameWorld::BringToFront(GameObject* obj) { 
    auto it = std::find_if(
        gameObjects.begin(), gameObjects.end(),
        [obj](const std::shared_ptr<GameObject>& p) { return p.get() == obj; });
    if (it != gameObjects.end()) {
        auto sp = *it;
        gameObjects.erase(it);
        gameObjects.push_back(sp);
    }
}

void GameWorld::InsertBefore(GameObject* obj, GameObject* beforeThis) {
    auto itBefore =
        std::find_if(gameObjects.begin(), gameObjects.end(),
                  [beforeThis](const std::shared_ptr<GameObject>& p) {
                      return p.get() == beforeThis;
                  });
    auto itObj = std::find_if(
        gameObjects.begin(), gameObjects.end(),
        [obj](const std::shared_ptr<GameObject>& p) { return p.get() == obj; });
    if (itBefore != gameObjects.end() && itObj != gameObjects.end()) {
        auto sp = *itObj;
        gameObjects.erase(itObj);
        gameObjects.insert(itBefore, sp);
    }
}

void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject) {
    if (!gameObject) return;

    // Check whether the object exists in the main storage
    auto itRoot =
        std::find_if(gameObjects.begin(), gameObjects.end(),
                     [gameObject](const std::shared_ptr<GameObject>& sp) {
                         return sp.get() == gameObject;
                     });
    if (itRoot == gameObjects.end()) return;

    // Collect a set of items to be deleted (root + children)
    std::unordered_set<GameObject*> toRemoveSet;
    toRemoveSet.insert(gameObject);

    auto* rootTransform = gameObject->GetComponent<TransformComponent>();
    if (rootTransform) {
        for (auto& go : gameObjects) {
            if (go.get() == gameObject) continue;
            auto* tf = go->GetComponent<TransformComponent>();
            if (!tf) continue;
            auto* parent = tf->GetParent();
            while (parent) {
                if (parent == rootTransform) {
                    toRemoveSet.insert(go.get());
                    break;
                }
                parent = parent->GetParent();
            }
        }
    }

    // Disconnect from the parent (if any)
    if (rootTransform) {
        auto* parent = rootTransform->GetParent();
        if (parent) {
            parent->GetGameObject()->RemoveChild(gameObject);
        }
    }

    // Remove all objects from the set from the gameObjects
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
                       [&](const std::shared_ptr<GameObject>& sp) {
                           return toRemoveSet.find(sp.get()) !=
                                  toRemoveSet.end();
                       }),
        gameObjects.end());
}
}  // namespace MyEngine