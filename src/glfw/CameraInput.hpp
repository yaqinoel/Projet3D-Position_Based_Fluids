#pragma once

#include "render/Camera.hpp"

struct GLFWwindow;
class Camera;

// stock mouse position, rotation state every frame
struct MouseState {
    double x = 0.0;
    double y = 0.0;
    bool rotating = false;
};

// camera moving parameters
struct CameraInputSettings {
    float moveSpeed = 3.0f;
    float mouseSensitivity = 0.20f;
};

void updateCamera(GLFWwindow* window, Camera& camera, MouseState& mouse, float dt, const CameraInputSettings& settings);