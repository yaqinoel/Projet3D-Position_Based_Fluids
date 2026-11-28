#include "SimulationManager.hpp"

#include <cmath>
#include <utility>

#include <glm/gtc/constants.hpp>

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
    solver.setContainer(simulationScene.container);
    containerWidthCycleTime = 0.0;
    isPaused = true;
    stepRequested = false;
}

// execute simulation
void SimulationManager::updateSimulation(float /*frameDt*/) {
    if (isPaused && !stepRequested) {
        return;
    }

    const float dt = params.fixedTimeStep;

    // change container shape by the time
    const double duration = params.containerWidthCycleDuration;
    if (duration > 0.0) {
        containerWidthCycleTime = std::fmod(containerWidthCycleTime + dt, duration);
        const double phase = glm::two_pi<double>() * containerWidthCycleTime / duration;
        const float widthScale = static_cast<float>(1.0 + 0.5 * (1.0 - std::cos(phase)));

        BoxContainer container = simulationScene.container;
        container.halfSize.x *= widthScale;
        solver.setContainer(container);
    }

    solver.step(dt);
    stepRequested = false;
}

// return all particles, but just can be read
const std::vector<Particle>& SimulationManager::getParticles() const {
    return solver.getParticles();
}

const BoxContainer& SimulationManager::getContainer() const {
    return solver.getContainer();
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