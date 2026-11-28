#include "Fluids.hpp"

#include <glm/gtc/random.hpp>
#include <stdexcept>

Fluids::Fluids(float density, InitialFluidCube cube)
    : density(density), cube(cube) {
}

// initialize particles in grid
[[nodiscard]]
std::vector<Particle> Fluids::createParticles(const SimulationParams& settings) const {
    std::vector<Particle> particles;

    // calcube fluid initial volume and mass
    const float volume = 8.0f * cube.halfSize.x * cube.halfSize.y * cube.halfSize.z;
    const float fluidMass = volume * density;

    // TODO
    // initialize particles by spcing
    if (settings.particleSamplingMode == ParticleSamplingMode::BySpacing) {
        if (!std::isfinite(settings.particleSpacing) || settings.particleSpacing <= 0.0f) {
            throw std::invalid_argument("particleSpacing must be finite and positive");
        }
        const glm::vec3 boxMin = cube.center - cube.halfSize;
        const std::size_t nx = std::max(1, static_cast<int>(std::round(2.0 * cube.halfSize.x / settings.particleSpacing)));
        const std::size_t ny = std::max(1, static_cast<int>(std::round(2.0 * cube.halfSize.y / settings.particleSpacing)));
        const std::size_t nz = std::max(1, static_cast<int>(std::round(2.0 * cube.halfSize.z / settings.particleSpacing)));
        const glm::vec3 cellSize = 2.0f * cube.halfSize / glm::vec3(nx, ny, nz);

        const std::size_t count = nx * ny * nz;
        particles.resize(count);

        const float particleMass = fluidMass / static_cast<float>(count);

        // set all particles in a regular grid
        for (int z = 0; z < nz; ++z) {
            for (int y = 0; y < ny; ++y) {
                for (int x = 0; x < nx; ++x) {
                    const glm::vec3 cellIndex(x, y, z);
                    const std::size_t particleIndex = z * nx * ny + y * nx + x;
                    particles[particleIndex].mass = particleMass;
                    particles[particleIndex].velocity = glm::vec3(0.0f);
                    particles[particleIndex].position = boxMin + (cellIndex + glm::vec3(0.5f)) * cellSize;
                }
            }
        }
    }
    // initialize particles by particleCount
    else if (settings.particleSamplingMode == ParticleSamplingMode::ByCount) {
        if (settings.particleCount <= 0) {
            throw std::invalid_argument("particleCount must be positive");
        }
        particles.resize(settings.particleCount);
        const float particleMass = fluidMass / static_cast<float>(settings.particleCount);

        // set all particles randomly in a cube
        for (int i = 0; i < settings.particleCount; i++) {
            particles[i].mass = particleMass;
            particles[i].velocity = glm::vec3(0.0f);
            const glm::vec3 random = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
            particles[i].position = cube.center + cube.halfSize * random;
        }
    }

    // set a initial height from bottom to top
    const float bottomY = cube.center.y - cube.halfSize.y;
    for (Particle& particle : particles) {
        particle.initialHeight = particle.position.y - bottomY;
    }

    return particles;
}

float Fluids::getDensity() const {
    return density;
}

float Fluids::getInitialShapeHeight() const {
    return 2.0f * cube.halfSize.y;
}
