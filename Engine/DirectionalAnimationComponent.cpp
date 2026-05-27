#include "pch.h"
#include "DirectionalAnimationComponent.h"
#include <cmath>

namespace MyEngine {
DirectionalAnimationComponent::DirectionalAnimationComponent(
    GameObject* gameObject, int numDirections) : Component(gameObject), numDirections(numDirections){
    transform = gameObject->GetComponent<TransformComponent>();
    renderer = gameObject->GetComponent<SpriteRendererComponent>();
    if (!transform || !renderer) {
        LOG_ERROR(
            "DirectionalAnimationComponent needs Transform and SpriteRenderer. "
            "Removing.");
        gameObject->RemoveComponent(this);
    }
}

void DirectionalAnimationComponent::Initialize(
    const std::string& textureMapName, float framerate) {
    int totalFrames =
        ResourceSystem::Instance()->GetTextureMapElementsCount(textureMapName);
    if (totalFrames == 0) {
        LOG_ERROR("No frames in texture map: " + textureMapName);
        return;
    }
    if (totalFrames % numDirections != 0) {
        LOG_WARN(
            "Total frames not divisible by numDirections, some frames "
            "ignored.");
    }
    framesPerRow = totalFrames / numDirections;
    allFrames.clear();
    for (int i = 0; i < totalFrames; ++i) {
        allFrames.push_back(
            ResourceSystem::Instance()->GetTextureMapElementShared(
                textureMapName, i));
    }
    secondsPerFrame = 1.f / framerate;
    renderer->SetTexture(*allFrames[0]);  // opening shot
    LOG_INFO("DirectionalAnimationComponent initialized with " +
             std::to_string(totalFrames) + " frames (" +
             std::to_string(numDirections) + " directions)");
}

void DirectionalAnimationComponent::SetMoveDirection(
    const Vector2Df& dir) {
    if (std::fabs(dir.x) < 0.01f && std::fabs(dir.y) < 0.01f) {
        isMoving = false;
        return;
    }
    isMoving = true;

    // Determine the dominant axis and select a row in the atlas
    if (std::fabs(dir.x) > std::fabs(dir.y)) {
        // Horizontal movement
        if (dir.x < 0) {
            currentDirection = 2;  // line 2 – left
        } else {
            currentDirection = 3;  // line 3 – right
        }
    } else {
        // Vertical movement
        if (dir.y < 0) {
            currentDirection = 0;  // line 0 – down
        } else {
            currentDirection = 1;  // line 1 – up
        }
    }
    //LOG_INFO("Direction: " + std::to_string(currentDirection));
}

void DirectionalAnimationComponent::Update(float deltaTime) {
    if (!transform || !renderer || allFrames.empty()) return;

    if (!isMoving) {
        // When stationary, display the first frame of the current direction
        int baseIdx = currentDirection * framesPerRow;
        renderer->SetTexture(*allFrames[baseIdx]);
        timer = 0.f;
        return;
    }

    timer += deltaTime;
    if (timer >= secondsPerFrame) {
        timer -= secondsPerFrame;
        int baseIdx = currentDirection * framesPerRow;
        currentFrame = (currentFrame + 1) % framesPerRow;
        renderer->SetTexture(*allFrames[baseIdx + currentFrame]);
    }
}


}  // namespace MyEngine