#include <iostream>

#include "PBFSolver.hpp"

#include <glm/common.hpp>


// initialize solver with particles
PBFSolver::PBFSolver(const std::vector<Particle>& initialParticles, BoxContainer box, SimulationParams params)
    : particles(initialParticles), container(box), params(params) {
    std::cout<< "Initialized solver with " << particles.size() << " particles" << std::endl;
}


// execute simulation one step
void PBFSolver::step(float dt) {

    if (dt <= 0.0f) {
        throw std::runtime_error("PBFSolver::step: dt is not positive");
    }

    std::size_t iteration = 0;
    std::vector<glm::vec3> newPositions;    // position predicted
    newPositions.resize(particles.size());

    const glm::vec3 boxMin = container.center - container.halfSize;
    const glm::vec3 boxMax = container.center + container.halfSize;

    // TODO - implement PBF algorithm

    // for all particles predict new positions
    for (int i = 0; i < particles.size(); i++) {
        // apply forces v_i <= v_i + dt * f(x_i)
        auto& p = particles[i];
        p.velocity = p.velocity + dt * params.gravity;
        // predict position
        newPositions[i] = p.position + dt * p.velocity;
    }

    // TODO - find neighboring
    for (int i = 0; i < particles.size(); i++) {

    }

    // TODO - solver iteration
    while (iteration < params.numInteration) {
        // calculate lamda
        for (int i = 0; i < particles.size(); i++) {
        }

        // calculate position fix delta_P
        for (int i = 0; i < particles.size(); i++) {
            // perform collision detection and response
            newPositions[i] = glm::clamp(newPositions[i], boxMin, boxMax);
        }

        // update new position with delta_p
        for (int i = 0; i < particles.size(); i++) {
        }

        iteration++;
    }   // end while

    // update velocity and position
    for (int i = 0; i < particles.size(); i++) {
        auto& p = particles[i];
        p.velocity = (newPositions[i] - p.position) / dt;
        // TODO - apply vorticity confinement and XSPH viscosity
        p.position = newPositions[i];
    }

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
