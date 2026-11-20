#include "Camera.hpp"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

// create a camera from a position and view angles
Camera::Camera(glm::vec3 position, float yawDegrees, float pitchDegrees){
    setPose(position, yawDegrees, pitchDegrees);
}


// set camera position and rotation
void Camera::setPose(glm::vec3 newPosition, float yawDegrees, float pitchDegrees) {
    position = newPosition;
    // wrap yaw to [0, 360) degrees
    yaw = std::fmod(yawDegrees, 360.0f);
    if (yaw < 0.0f) {
        yaw += 360.0f;
    }
    // keep the forward direction away from the vertical axis
    pitch = std::clamp(pitchDegrees, -89.0f, 89.0f);
}


// set camera projection settings
bool Camera::setProjection(const CameraProjection& settings) {
    // keep the current projection if the new settings are invalid
    if (!(settings.fovDegrees > 0.0f && settings.fovDegrees < 180.0f
          && settings.nearPlane > 0.0f && settings.farPlane > settings.nearPlane
          && std::isfinite(settings.farPlane))) {
        return false;
          }

    projection = settings;
    return true;
}


// return camera forward direction
[[nodiscard]]
glm::vec3 Camera::forward() const {
    const float yawRadians = glm::radians(yaw);
    const float pitchRadians = glm::radians(pitch);

    return glm::normalize(
        glm::vec3{
            std::cos(yawRadians) * std::cos(pitchRadians),
            std::sin(pitchRadians),
            std::sin(yawRadians) * std::cos(pitchRadians)
        }
    );
}


// move camera in world by offset
void Camera::move(const glm::vec3& offset) {
    const glm::vec3 front = forward();
    const glm::vec3 right = glm::normalize(glm::cross(front, WORLD_UP));

    // convert movement along the camera axes and world up to a world offset
    position += offset.x * right + offset.y * WORLD_UP + offset.z * front;
}


// rotate camera by change yaw and pitch
void Camera::rotate(float yawDelta, float pitchDelta) {
    yaw = std::fmod(yaw + yawDelta, 360.0f);
    if (yaw < 0.0f) {
        yaw += 360.0f;
    }
    pitch = std::clamp(pitch + pitchDelta, -89.0f, 89.0f);
}

[[nodiscard]]
glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(position, position + forward(), WORLD_UP);
}

[[nodiscard]]
glm::mat4 Camera::projectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(projection.fovDegrees),
            aspectRatio,
            projection.nearPlane,
            projection.farPlane
            );
}
