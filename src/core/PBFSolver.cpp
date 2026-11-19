#include <iostream>

#include "PBFSolver.hpp"


// initialize solver with particles
PBFSolver::PBFSolver(std::vector<Particle> initialParticles)
    : particles(std::move(initialParticles)) {
    std::cout<< "Initialized solver with " << particles.size() << " particles" << std::endl;
}

// execute one step PBF algorithm
void PBFSolver::step(float dt) {
    std::cout<< "step one..." << std::endl;

}

// reset solver states, particles
void PBFSolver::reset() {
    std::cout<< "Reset solver..." << std::endl;

}

// get all particles, just for read, only the solver can change particles attributions
[[nodiscard]]
const std::vector<Particle>& PBFSolver::getParticles() {
    return particles;
}
