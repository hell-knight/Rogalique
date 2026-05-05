#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

namespace MyEngine {
CameraComponent::CameraComponent(GameObject* gameObject)
    : Component(gameObject) {
    view = new sf::View(sf::FloatRect(0, 0, 800, -600));
    transform = gameObject->GetComponent<TransformComponent>();
}
CameraComponent::~CameraComponent() { delete view; }

void CameraComponent::Update(float deltaTime) {
    auto position = transform->GetWorldPosition();
    auto rotation = transform->GetWorldRotation();

    sf::Vector2f finalPos = Convert<sf::Vector2f, Vector2Df>(position) +
                            Convert<sf::Vector2f, Vector2Df>(shakeOffset);
    view->setCenter(finalPos);
    view->setRotation(rotation);

    if (!window) {
        LOG_ERROR("CameraComponent: window is null, cannot set view.");
        return;
    }
    window->setView(*view);
}

void CameraComponent::Render() {
    if (!window) {
        LOG_ERROR("CameraComponent: window is null, cannot set view.");
        return;
    }
}

void CameraComponent::SetBaseResolution(int width, int height) {
    view->reset(sf::FloatRect(0, 0, static_cast<float>(width),
                              static_cast<float>(-height)));
}

void CameraComponent::SetShakeOffset(const Vector2Df& offset) {
    shakeOffset = offset;
}

void CameraComponent::SetWindow(sf::RenderWindow* newWindow) {
    window = newWindow;
}

void CameraComponent::ZoomBy(float newZoom) {
    if (newZoom <= 0) {
        std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
        return;
    }
    view->zoom(newZoom);
}
}  // namespace MyEngine