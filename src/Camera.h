#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         {-90.0F};
const float PITCH       {  0.0F};
const float SPEED       {  2.5F};
const float SENSITIVITY {  0.1F};
const float ZOOM        { 45.0F};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    float Zoom;
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    
    glm::vec3 WorldUp;

    // euler Angles
    double Yaw;
    double Pitch;
    
    // camera options
    float movementSpeed;
    float mouseSensitivity;

public:
    Camera(glm::vec3 position = glm::vec3(0.0F, 0.0F, 0.0F),
            glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F),
            float yaw = YAW,
            float pitch = PITCH)
    :   Front(glm::vec3(0.0F, 0.0F, -1.0F)),
        movementSpeed(SPEED),
        mouseSensitivity(SENSITIVITY),
        Zoom(ZOOM),
        Position(position),
        WorldUp(up),
        Pitch(pitch) {
        updateCameraVectors();
    }
    
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0F, 0.0F, -1.0F)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    auto getViewMatrix() -> glm::mat4 {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    auto processKeyboard(Camera_Movement direction, float deltaTime) -> void {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    auto processMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = 1) -> void {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch != 0U) {
            if (Pitch > 89.0)
                Pitch = 89.0;
            if (Pitch < -89.0)
                Pitch = -89.0;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    auto processMouseScroll(float yoffset) -> void {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0F)
            Zoom = 1.0F;
        if (Zoom > 45.0F)
            Zoom = 45.0F;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    auto updateCameraVectors() -> void {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif