#pragma once
#include "Component.h"
#include "Particle.h"
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>

namespace RogaliqueGame {
class ParticleSystemComponent : public MyEngine::Component {
   public:
    ParticleSystemComponent(MyEngine::GameObject* gameObject);
    void Update(float deltaTime) override;
    void Render() override;

    // Add several particles
    void Emit(const std::vector<Particle>& newParticles);
    bool IsEmpty() const;  // Have all the particles died?

    // notify the engine that the component has expired when all particles have died
    bool IsExpired() const override { return IsEmpty(); }

   private:
    std::vector<Particle> particles;
    sf::VertexArray vertices;  // for rendering (Quads)
    void UpdateVertices();
};
}
