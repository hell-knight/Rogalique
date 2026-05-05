#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "TransformComponent.h"
#include "EngineAPI.h"

namespace MyEngine {
class ENGINE_API CameraComponent : public Component {
   public:
    CameraComponent(GameObject* gameObject);
    ~CameraComponent();

    void Update(float deltaTime) override;
    void Render() override;

    void SetWindow(sf::RenderWindow* newWindow);
    void ZoomBy(float newZoom);
    void SetBaseResolution(int width, int height);

    void SetShakeOffset(const Vector2Df& offset);

   private:
    TransformComponent* transform;
    sf::RenderWindow* window;
    sf::View* view;
    Vector2Df shakeOffset;
};
}  // namespace MyEngine