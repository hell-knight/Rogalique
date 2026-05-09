#pragma once
#include "EngineAPI.h"

namespace MyEngine {
class GameObject;

class ENGINE_API Component {
   public:
    Component(GameObject* gameObject);
    virtual ~Component();

    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    GameObject* GetGameObject();

    // Returns true if the component has expired and its owner should be removed
    virtual bool IsExpired() const { return false; }

   protected:
    GameObject* gameObject;
};
}  // namespace MyEngine