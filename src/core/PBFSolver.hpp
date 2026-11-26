#pragma once

#include <vector>
#include "Particle.hpp"
#include "BoxContainer.hpp"
#include "NeighborSearchEngine.hpp"
#include "SimulationParams.hpp"

class PBFSolver {
public:
    explicit PBFSolver(const float density, const std::vector<Particle>& initialParticles, BoxContainer box, SimulationParams params);

    void step(float dt);

    [[nodiscard]]
    const std::vector<Particle>& getParticles() const;
    void setParticles(const std::vector<Particle>& particles);

private:
    const float restDensity;
    std::vector<Particle> particles;    // particles during the simulation
    BoxContainer container; // container for particles
    SimulationParams params; // parameters for simulation
    NeighborSearchEngine searchEngine;
};
