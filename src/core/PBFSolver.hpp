#pragma once

#include <vector>
#include "Particle.hpp"

class PBFSolver {
public:
    explicit PBFSolver(std::vector<Particle> initialParticles);

    void step(float dt);
    void reset();

    [[nodiscard]]
    const std::vector<Particle>& getParticles();

private:
    std::vector<Particle> particles;

};
