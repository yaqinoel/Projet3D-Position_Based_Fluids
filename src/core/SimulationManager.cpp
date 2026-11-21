#include "SimulationManager.hpp"

#include <utility>

// initial solver by particles
SimulationManager::SimulationManager(std::vector<Particle> particles)
    : initialParticles(std::move(particles)), solver(initialParticles) {
}


// reset all particles in solver to initial state
void SimulationManager::reset() {
    solver.setParticles(initialParticles);
    isPaused = true;
}

// execute simulation
void SimulationManager::updateSimulation(float dt) {
    // handle one step request
    if (stepRequested) {
        solver.step(dt);
        stepRequested = false;  // reset one step request
    }

    if (!isPaused) {
        solver.step(dt);
    }
}

// return all particles, but just can be read
const std::vector<Particle>& SimulationManager::getParticles() const {
    return solver.getParticles();
}

// toggle pause state
void SimulationManager::togglePause() {
    isPaused = !isPaused;
}


// set one step execution request
void SimulationManager::oneStepRequest() {
    isPaused = true;
    stepRequested = true;
}