#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods
enum CameraMoveDir {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};


class Camera {
public:
  glm::vec3 pos;
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);
  glm::vec3 right;
  glm::vec3 worldUp;
  float yaw;
  float pitch;
  float speed = 3.0f;
  float sensitivity = 0.25f;
  float fov = 45.0f;

  Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = -90.0f, float pitch = 0.0f
  )
  {
    pos = position;
    worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateVectors();
  }

  // Constructor with scalar values
  //Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
  //{
  //  this->Position = glm::vec3(posX, posY, posZ);
  //  this->WorldUp = glm::vec3(upX, upY, upZ);
  //  this->Yaw = yaw;
  //  this->Pitch = pitch;
  //  this->updateCameraVectors();
  //}

  void updateVectors() {
    printf("\n");
  }

  glm::mat4 getViewMat() {
    return glm::lookAt(pos, pos + front, up);
  }

  void processMove(CameraMoveDir move, double deltaTime) {
    GLfloat velocity = speed * deltaTime;
    switch (move) {
      case FORWARD
        pos += front * velocity;
        break;
      case BACKWARD:
        pos -= front * velocity;
        break;
      case LEFT:
  	    pos -= glm::normalize(glm::cross(front, up)) * speed;
        break;
      case RIGHT:
  	    pos += glm::normalize(glm::cross(front, up)) * speed;
        break;
      default:
        break;
    }
  }
}
