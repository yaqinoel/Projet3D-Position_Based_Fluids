#pragma once

#include <PBFSolver.hpp>
#include <Particle.hpp>

#include "SimulationParams.hpp"
#include "SimulationScene.hpp"

class SimulationManager {
public:
    explicit SimulationManager(SimulationScene scene, SimulationParams settings);
    void reset();
    void updateSimulation(float dt);
    const std::vector<Particle>& getParticles() const;
    const BoxContainer& getContainer() const;
    void togglePause();
    void oneStepRequest();

private:
    bool isPaused = true;
    bool stepRequested = false;
    double containerWidthCycleTime = 0.0;
    SimulationParams params;
    SimulationScene simulationScene;
    std::vector<Particle> initialParticles; // keep all particles initial state in simulation manager
    PBFSolver solver;   // solver as a private member
};
