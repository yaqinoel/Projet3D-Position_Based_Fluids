#include "CameraInput.hpp"
#include "render/Camera.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>

void updateCamera(GLFWwindow* window, Camera& camera, MouseState& mouse,
    float frameDt, const CameraInputSettings& settings ) {

    // get current mouse state
    double x = 0.0;
    double y = 0.0;
    glfwGetCursorPos(window, &x, &y);
    const bool rotating = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    // rotate camera by mouse moving offset (last frame mouse position - current mouse position)
    if (rotating && mouse.rotating) {
        const float dx = static_cast<float>(x - mouse.x);
        const float dy = static_cast<float>(mouse.y - y);
        camera.rotate(dx * settings.mouseSensitivity, dy * settings.mouseSensitivity);
    }

    mouse = {x, y, rotating};   // update mouse state

    // lambda function to get input as a vector in every axis
    const auto axis = [window](int positive, int negative) {
        const bool positivePressed = glfwGetKey(window, positive) == GLFW_PRESS;
        const bool negativePressed = glfwGetKey(window, negative) == GLFW_PRESS;
        return static_cast<float>(positivePressed) - static_cast<float>(negativePressed);
    };

    // compute move direction
    const glm::vec3 direction{
        axis(GLFW_KEY_D, GLFW_KEY_A),
        axis(GLFW_KEY_E, GLFW_KEY_Q),
        axis(GLFW_KEY_W, GLFW_KEY_S)
    };

    // keep movement speed consistent when pressing multiple keys
    if (glm::dot(direction, direction) > 0.0f) {
        camera.move(glm::normalize(direction) * settings.moveSpeed * frameDt);
    }
}