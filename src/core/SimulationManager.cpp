#include "SimulationManager.hpp"

#include <utility>

// initial simulation manager by scene
SimulationManager::SimulationManager(SimulationScene scene, SimulationParams settings)
    : params(settings),
    simulationScene(std::move(scene)),
    initialParticles(simulationScene.fluids.createParticles(params)),
    solver(simulationScene.fluids.getDensity(),initialParticles, simulationScene.container, params) {
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
        solver.step(params.fixedTimeStep);
        stepRequested = false;  // reset one step request
    }

    if (!isPaused) {
        solver.step(params.fixedTimeStep);
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