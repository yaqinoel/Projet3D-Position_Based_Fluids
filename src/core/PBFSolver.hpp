#pragma once

#include <vector>
#include "Particle.hpp"

class PBFSolver {
public:
    explicit PBFSolver(const std::vector<Particle>& initialParticles);

    void step(float dt);

    [[nodiscard]]
    const std::vector<Particle>& getParticles() const;
    void setParticles(const std::vector<Particle>& particles);

private:
    std::vector<Particle> particles;    // particles during the simulation
};
