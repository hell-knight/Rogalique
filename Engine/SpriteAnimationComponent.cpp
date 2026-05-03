#include "pch.h"
#include "SpriteAnimationComponent.h"

namespace MyEngine
{
    SpriteAnimationComponent::SpriteAnimationComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        transform = gameObject->GetComponent<TransformComponent>();
        renderer = gameObject->GetComponent<SpriteRendererComponent>();

        if (!transform || !renderer)
        {
            LOG_ERROR("SpriteAnimationComponent needs Transform and SpriteRenderer. Removing.");
            gameObject->RemoveComponent(this);
        }
    }

    void SpriteAnimationComponent::Initialize(const std::string& textureMapName, float framerate)
    {
        if (!transform || !renderer) return;

        int frameCount = ResourceSystem::Instance()->GetTextureMapElementsCount(textureMapName);
        if (frameCount == 0)
        {
            LOG_ERROR("No frames in texture map: " + textureMapName);
            return;
        }

        frames.clear();
        for (int i = 0; i < frameCount; ++i)
            frames.push_back(ResourceSystem::Instance()->GetTextureMapElementShared(textureMapName, i));

        secondsPerFrame = 1.f / framerate;

        // Set the starting frame
        renderer->SetTexture(*frames[0]);
        lastPosition = transform->GetWorldPosition();
        LOG_INFO("SpriteAnimationComponent initialized with " + std::to_string(frameCount) + " frames from " + textureMapName);
    }

    void SpriteAnimationComponent::Update(float deltaTime)
    {
        if (!transform || !renderer || frames.empty()) return;

        Vector2Df currentPos = transform->GetWorldPosition();
        float distance = (currentPos - lastPosition).GetLength();
        //std::cout << "AI pos: (" << currentPos.x << ", " << currentPos.y << ") dist: " << distance << std::endl;
        const float threshold = 0.01f;  // pixels per frame

        if (distance > threshold)
        {
            if (!isMoving)
            {
                // Once we start moving, we'll immediately switch to the second frame (if available)
                timer = 0.f;
                currentFrame = 0;
                isMoving = true;
            }

            timer += deltaTime;
            if (timer >= secondsPerFrame)
            {
                timer -= secondsPerFrame;
                currentFrame = (currentFrame + 1) % frames.size();
                renderer->SetTexture(*frames[currentFrame]);
            }
        }
        else
        {
            if (isMoving)
            {
                // paused—let's go back to the first frame
                currentFrame = 0;
                renderer->SetTexture(*frames[0]);
                timer = 0.f;
                isMoving = false;
            }
        }

        lastPosition = currentPos;
    }
}