#include "AuraComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"

namespace RogaliqueGame {

AuraComponent::AuraComponent(MyEngine::GameObject* gameObject)
    : Component(gameObject), vertices(sf::Quads) {}

void AuraComponent::Emit(const std::vector<Particle>& localParticles) {
    particles.insert(particles.end(), localParticles.begin(),
                     localParticles.end());
}

void AuraComponent::Update(float deltaTime) {
    if (particles.empty()) return;

    // Updating the lifespan
    for (auto& p : particles) {
        p.life -= deltaTime;
    }
    // Removing dead
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle& p) { return p.life <= 0.0f; }),
        particles.end());

    UpdateVertices();
}

void AuraComponent::Render() {
    if (particles.empty()) return;
    auto& window = MyEngine::RenderSystem::Instance()->GetMainWindow();
    // Rendering in world coordinates; the current view is already set
    window.draw(vertices);
}

void AuraComponent::UpdateVertices() {
    vertices.clear();
    auto* transform = gameObject->GetComponent<MyEngine::TransformComponent>();
    MyEngine::Vector2Df worldPos = transform->GetWorldPosition();

    for (const auto& p : particles) {
        float lifeRatio = p.life / p.maxLife;
        float currentSize = p.size * (0.3f + 0.7f * lifeRatio);
        float half = currentSize * 0.5f;
        sf::Color col = p.color;
        col.a = static_cast<sf::Uint8>(col.a * lifeRatio);

        // Global position = object position + local offset
        sf::Vector2f pos(worldPos.x + p.position.x, worldPos.y + p.position.y);

        vertices.append(
            sf::Vertex(sf::Vector2f(pos.x - half, pos.y - half), col));
        vertices.append(
            sf::Vertex(sf::Vector2f(pos.x + half, pos.y - half), col));
        vertices.append(
            sf::Vertex(sf::Vector2f(pos.x + half, pos.y + half), col));
        vertices.append(
            sf::Vertex(sf::Vector2f(pos.x - half, pos.y + half), col));
    }
}

}  // namespace RogaliqueGame