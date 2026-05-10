#include "ParticleSystemComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include <cmath>

static sf::Color LerpColor(const sf::Color& a, const sf::Color& b, float t) {
    return sf::Color(static_cast<sf::Uint8>(a.r + (b.r - a.r) * t),
                     static_cast<sf::Uint8>(a.g + (b.g - a.g) * t),
                     static_cast<sf::Uint8>(a.b + (b.b - a.b) * t),
                     static_cast<sf::Uint8>(a.a + (b.a - a.a) * t));
}

namespace RogaliqueGame {
ParticleSystemComponent::ParticleSystemComponent(
    MyEngine::GameObject* gameObject)
    : Component(gameObject), vertices(sf::Quads) {}

void ParticleSystemComponent::Update(float deltaTime) {
    // Updating positions and lifetimes
    for (auto& p : particles) {
        p.position.x += p.velocity.x * deltaTime;
        p.position.y += p.velocity.y * deltaTime;
        p.life -= deltaTime;
    }
    // Removing dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle& p) { return p.life <= 0.0f; }),
        particles.end());

    // If it's empty, you can destroy the GameObject (we'll do this externally)
    UpdateVertices();
}

void ParticleSystemComponent::Render() {
    if (particles.empty()) return;
    auto& window = MyEngine::RenderSystem::Instance()->GetMainWindow();
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());
    // Let's restore the old view
    window.setView(oldView);
    // draw directly in world coordinates (like sprites). Since the camera is already
    // set up, we'll keep the current view.
    window.draw(vertices);
}

void ParticleSystemComponent::Emit(const std::vector<Particle>& newParticles) {
    particles.insert(particles.end(), newParticles.begin(), newParticles.end());
}

bool ParticleSystemComponent::IsEmpty() const { return particles.empty(); }

void ParticleSystemComponent::UpdateVertices() {
    vertices.clear();
    for (const auto& p : particles) {
        float lifeRatio = p.life / p.maxLife;  // from 1 to 0
        float currentSize =
            p.size *
            (0.3f + 0.7f * lifeRatio);  // minimum amount: 30% of the initial amount
        float half = currentSize * 0.5f;
        // Color as a function of transparency over time
        float alpha = p.life / p.maxLife;
        sf::Color col = p.color;
        col.a = static_cast<sf::Uint8>(col.a * alpha);

        // Add the four vertices of the square
        sf::Vertex v0(sf::Vector2f(p.position.x - half, p.position.y - half),
                      col);
        sf::Vertex v1(sf::Vector2f(p.position.x + half, p.position.y - half),
                      col);
        sf::Vertex v2(sf::Vector2f(p.position.x + half, p.position.y + half),
                      col);
        sf::Vertex v3(sf::Vector2f(p.position.x - half, p.position.y + half),
                      col);
        vertices.append(v0);
        vertices.append(v1);
        vertices.append(v2);
        vertices.append(v3);
    }
}


}  // namespace RogaliqueGame