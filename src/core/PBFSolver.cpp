#include "PBFSolver.hpp"


// initialize solver with particles
PBFSolver::PBFSolver(std::vector<Particle> initialParticles)
    : particles(std::move(initialParticles)) {
}

// execute one step PBF algorithm
void PBFSolver::step(float dt) {

}

// reset solver states, particles
void PBFSolver::reset() {

}

// get all particles, just for read, only the solver can change particles attributions
[[nodiscard]]
const std::vector<Particle>& PBFSolver::getParticles() {
    return particles;
}
