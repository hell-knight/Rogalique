#include "ParticleEmitter.h"
#include "ParticleSystemComponent.h"
#include "GameWorld.h"
#include "RigidbodyComponent.h"
#include <cstdlib>

namespace RogaliqueGame {
MyEngine::GameObject* ParticleEmitter::Create(
    const MyEngine::Vector2Df& position, sf::Color color, int count,
    float emissionRadius, float minSpeed, float maxSpeed, float minLife,
    float maxLife, float particleSize) {
    auto* obj = MyEngine::GameWorld::Instance()->CreateGameObject("ParticleEmitter");
    auto* transform = obj->GetComponent<MyEngine::TransformComponent>();
    transform->SetWorldPosition(position);

    // RigidbodyComponent kinematic, so as not to interfere with physics
    auto* rigid = obj->AddComponent<MyEngine::RigidbodyComponent>();
    rigid->SetKinematic(true);

    auto* particleSystem = obj->AddComponent<ParticleSystemComponent>();
    auto particles =
        GenerateBurst(position, count, color, emissionRadius, minSpeed,
                      maxSpeed, minLife, maxLife, particleSize);
    particleSystem->Emit(particles);

    return obj;
}

std::vector<Particle> ParticleEmitter::GenerateBurst(
    const MyEngine::Vector2Df& origin, int count, sf::Color color, float radius,
    float minSpeed, float maxSpeed, float minLife, float maxLife, float size) {
    std::vector<Particle> result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        Particle p;
        // Random displacement within the radius
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159265f;
        float dist = static_cast<float>(rand()) / RAND_MAX * radius;
        p.position.x = origin.x + cosf(angle) * dist;
        p.position.y = origin.y + sinf(angle) * dist;

        // Speed: random direction and speed
        float speed = minSpeed + static_cast<float>(rand()) / RAND_MAX *
                                     (maxSpeed - minSpeed);
        float velAngle =
            static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159265f;
        p.velocity.x = cosf(velAngle) * speed;
        p.velocity.y = sinf(velAngle) * speed;

        p.maxLife = minLife +
                    static_cast<float>(rand()) / RAND_MAX * (maxLife - minLife);
        p.life = p.maxLife;
        p.color = color;
        p.size = size;
        result.push_back(p);
    }

    return result;
}


}  // namespace RogaliqueGame