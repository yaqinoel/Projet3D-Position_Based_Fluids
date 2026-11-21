#include "SimulationInput.hpp"
#include "SimulationManager.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// key call back function
void onSimulationKey(GLFWwindow* window, int key, int, int action, int) {
    if (action != GLFW_PRESS) {
        return;
    }

    // get pointer of simulation manager from window
    auto* simulationManager = static_cast<SimulationManager*>(glfwGetWindowUserPointer(window));

    switch (key) {
        case GLFW_KEY_SPACE:    // change pause state
            simulationManager->togglePause();
            break;
        case GLFW_KEY_R:    // reset simulation
            simulationManager->reset();
            break;
        case GLFW_KEY_P:    // one step execution
            simulationManager->oneStepRequest();
            break;
        default:
            return;
    }

}


// bind key call back
void bindSimulationInput(GLFWwindow* window, SimulationManager& simulation) {
    glfwSetWindowUserPointer(window, &simulation);  // set pointer of simulation manager to window
    glfwSetKeyCallback(window, onSimulationKey);
}