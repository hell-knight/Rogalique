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
    // --- Безопасное удаление помеченных объектов ---
    // Берём копию, чтобы избежать модификации вектора во время итерации
    std::vector<GameObject*> toDestroy = markedToDestroyGameObjects;
    markedToDestroyGameObjects.clear();

    for (GameObject* go : toDestroy) {
        // Проверяем, жив ли ещё объект (мог быть уже удалён как потомок
        // другого)
        auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                               [go](const std::shared_ptr<GameObject>& sp) {
                                   return sp.get() == go;
                               });
        if (it != gameObjects.end()) {
            DestroyGameObjectImmediate(go);
        }
        // Если объект уже отсутствует, просто пропускаем
    }

    // --- Автоудаление по истечению срока (IsExpired) ---
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
            DestroyGameObject(go.get());  // будет удалён на следующем кадре
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
    // Перестраховка: если объекта нет в хранилище – выходим
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [gameObject](const std::shared_ptr<GameObject>& sp) {
                               return sp.get() == gameObject;
                           });
    if (it == gameObjects.end()) return;

    // Проверка, что не помечен уже
    if (std::find(markedToDestroyGameObjects.begin(),
                  markedToDestroyGameObjects.end(),
                  gameObject) == markedToDestroyGameObjects.end()) {
        markedToDestroyGameObjects.push_back(gameObject);
    }
}

void GameWorld::Clear() {
    // Сначала сбросим родительские связи, чтобы деструкторы не пытались удалить
    // детей повторно
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

    // Проверяем, существует ли объект в главном хранилище
    auto itRoot =
        std::find_if(gameObjects.begin(), gameObjects.end(),
                     [gameObject](const std::shared_ptr<GameObject>& sp) {
                         return sp.get() == gameObject;
                     });
    if (itRoot == gameObjects.end()) return;

    // Собираем множество удаляемых (корень + потомки)
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

    // Отсоединяем от родителя (если есть)
    if (rootTransform) {
        auto* parent = rootTransform->GetParent();
        if (parent) {
            parent->GetGameObject()->RemoveChild(gameObject);
        }
    }

    // Удаляем из gameObjects все объекты из множества
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
                       [&](const std::shared_ptr<GameObject>& sp) {
                           return toRemoveSet.find(sp.get()) !=
                                  toRemoveSet.end();
                       }),
        gameObjects.end());
}
}  // namespace MyEngine