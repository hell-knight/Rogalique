#pragma once
#include "Component.h"
#include "Particle.h"
#include <vector>
#include <SFML/Graphics/VertexArray.hpp>

namespace RogaliqueGame {
class AuraComponent : public MyEngine::Component {
   public:
    AuraComponent(MyEngine::GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    // Adds particles in local coordinates (relative to the object's center)
    void Emit(const std::vector<Particle>& localParticles);

    // true if all particles have disappeared
    bool IsEmpty() const { return particles.empty(); }

   private:
    std::vector<Particle> particles;
    sf::VertexArray vertices;
    void UpdateVertices();
};
}  // namespace RogaliqueGame