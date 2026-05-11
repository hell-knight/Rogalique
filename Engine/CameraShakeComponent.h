#pragma once
#include "Component.h"
#include "EngineAPI.h"
#include "Vector.h"

namespace MyEngine {
class CameraComponent;
class HealthComponent;
class AttackComponent;

class ENGINE_API CameraShakeComponent : public Component {
   public:
    CameraShakeComponent(GameObject* gameObject, CameraComponent* camera,
                         HealthComponent* health,
                         AttackComponent* attack = nullptr);
    ~CameraShakeComponent();

    void Update(float deltaTime) override;
    void Render() override {}

   private:
    void StartShake(float intensity, float duration);
    CameraComponent* camera;
    HealthComponent* health;
    AttackComponent* attack;

    // Parameters shake
    bool isShaking = false;
    float shakeIntensity = 0.f;
    float shakeDuration = 0.f;
    float shakeTimer = 0.f;

    // To track changes in health
    float previousHealth = 0.f;

    // Id subscribe
    int damageSubId = 1;
    int attackSubId = 1;
};
}
