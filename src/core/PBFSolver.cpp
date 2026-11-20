#include <iostream>

#include "PBFSolver.hpp"


// initialize solver with particles
PBFSolver::PBFSolver(const std::vector<Particle>& initialParticles)
    : particles(initialParticles) {
    std::cout<< "Initialized solver with " << particles.size() << " particles" << std::endl;
}


// execute simulation one step
void PBFSolver::step(float dt) {
    // for the moment we just let particles go down 0.001f every stp
    for (int i = 0; i < particles.size(); i++) {
        particles[i].position.y -= 0.001f;
    }

    // TODO
    // implement PBF algorithm

}


// set new particles for solver
void PBFSolver::setParticles(const std::vector<Particle>& newParticles) {
    std::cout<< "Reset solver with " << particles.size() << " particles" << std::endl;
    particles = newParticles;
}


// get all particles, just for read, only the solver can change particles attributions
[[nodiscard]]
const std::vector<Particle>& PBFSolver::getParticles() const {
    return particles;
}
