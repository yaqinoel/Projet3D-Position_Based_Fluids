#pragma once

#include <vector>

#include "Particle.hpp"
#include "SimulationParams.hpp"

struct InitialFluidCube {
    glm::vec3 center;
    glm::vec3 halfSize;
};

class Fluids {
public:
    Fluids(float density, InitialFluidCube cube);   // initialize fluid by density and shape

    [[nodiscard]]
    std::vector<Particle> createParticles(const SimulationParams& settings) const;    // sampling particles in this fluid by settings

    float getDensity() const;

private:
    float density;
    InitialFluidCube cube;  // initial shape
};