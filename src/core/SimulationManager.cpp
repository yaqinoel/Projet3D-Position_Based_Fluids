#include "SimulationManager.hpp"

#include <utility>

// initial solver by particles
SimulationManager::SimulationManager(std::vector<Particle> particles)
    : initialParticles(std::move(particles)), solver(initialParticles) {
}

// reset all particles in solver to initial state
void SimulationManager::reset() {
    solver.setParticles(initialParticles);
}

// execute simulation
void SimulationManager::updateSimulation(float dt) {
    if (!isPaused) {
        solver.step(dt);
    }
}

// return all particles, but just can be read
const std::vector<Particle>& SimulationManager::getParticles() const {
    return solver.getParticles();
}