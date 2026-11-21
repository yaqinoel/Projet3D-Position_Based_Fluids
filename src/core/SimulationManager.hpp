#pragma once

#include <PBFSolver.hpp>
#include <Particle.hpp>

class SimulationManager {
public:
    explicit SimulationManager(std::vector<Particle> particles);
    void reset();
    void updateSimulation(float dt);
    const std::vector<Particle>& getParticles() const;
    void togglePause();
    void oneStepRequest();

private:
    bool isPaused = false;
    bool stepRequested = false;
    std::vector<Particle> initialParticles; // keep all particles initial state in simulation manager
    PBFSolver solver;   // solver as a private member
};