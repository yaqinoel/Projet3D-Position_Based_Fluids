#include "Fluids.hpp"

#include <glm/gtc/random.hpp>

Fluids::Fluids(float density, InitialFluidCube cube)
    : density(density), cube(cube) {
}


[[nodiscard]]
std::vector<Particle> Fluids::createParticles(const SimulationParams &settings) const {
    std::vector<Particle> particles;

    particles.resize(settings.particleCount);
    // calcube fluid initial volume and mass
    const float volume = 8.0f * cube.halfSize.x * cube.halfSize.y * cube.halfSize.z;
    const float fluidMass = volume * density;

    // set all particles randomly in a cube
    for (int i = 0; i < settings.particleCount; i++) {
        particles[i].mass = fluidMass / settings.particleCount;
        particles[i].velocity = glm::vec3(0.0f);
        const glm::vec3 random = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
        particles[i].position = cube.center + cube.halfSize * random;
    }

    return particles;
}


