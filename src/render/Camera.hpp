#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

constexpr glm::vec3 WORLD_UP{0.0f, 1.0f, 0.0f};  // up direction in world

// perspective settings, clipping planes are distances from the camera
struct CameraProjection {
    float fovDegrees = 45.0f;  // vertical field of view in degrees
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};

class Camera {
public:
    explicit Camera(glm::vec3 position, float yawDegrees = -90.0f, float pitchDegrees = 0.0f);

    void move(const glm::vec3& offset); // move right, up, and forward
    void rotate(float yawDelta, float pitchDelta);  // rotate by angle offsets in degrees

    void setPose(glm::vec3 newPosition, float yawDegrees, float pitchDegrees);  // set the absolute pose
    bool setProjection(const CameraProjection& settings);  // apply settings if valid

    [[nodiscard]]
    CameraProjection projectionSettings() const { return projection; }  // get a copy for editing

    [[nodiscard]]
    glm::vec3 forward() const;  // get camera forward direction

    [[nodiscard]]
    glm::mat4 viewMatrix() const;   // get camera view matrix

    [[nodiscard]]
    glm::mat4 projectionMatrix(float aspectRatio) const;    // get projection for the current viewport

private:
    glm::vec3 position;
    float yaw = -90.0f;
    float pitch = 0.0f;
    CameraProjection projection;
};