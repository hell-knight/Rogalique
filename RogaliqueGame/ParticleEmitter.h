#pragma once
#include "GameObject.h"
#include "Vector.h"
#include <SFML/Graphics/Color.hpp>
#include "Particle.h"

namespace RogaliqueGame {

class ParticleEmitter {
   public:
    // Creates an emitter at the specified world position.
    // Returns a reference to the emitter's GameObject
    static MyEngine::GameObject* Create(
        const MyEngine::Vector2Df& position, sf::Color color, int count,
        float emissionRadius = 20.0f, float minSpeed = 50.0f,
        float maxSpeed = 150.0f, float minLife = 0.3f, float maxLife = 0.8f,
        float particleSize = 4.0f);

   private:
    // Generates a set of particles for the flash
    static std::vector<Particle> GenerateBurst(
        const MyEngine::Vector2Df& origin, int count, sf::Color color,
        float radius, float minSpeed, float maxSpeed, float minLife,
        float maxLife, float size);
};

}  // namespace RogaliqueGame
