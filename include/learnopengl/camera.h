#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
  float speed       = 2.0f;
  float sensitivity = 0.05f;
  float fov         = 45.0f;

  Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = -90.0f, float pitch = 0.0f
  )
  {
    pos = position;
    this->worldUp = worldUp;
    this->yaw = yaw;
    this->pitch = pitch;
    updateVectors();
  }

  void updateVectors() {
    glm::vec3 nFront;
    nFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    nFront.y = sin(glm::radians(pitch));
    nFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(nFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
  }

  glm::mat4 getViewMat() {
    return glm::lookAt(pos, pos + front, up);
  }

  void processMove(CameraMoveDir move, double deltaTime) {
    float velocity = speed * deltaTime;
    switch (move) {
      case FORWARD:
        pos += front * velocity;
        break;
      case BACKWARD:
        pos -= front * velocity;
        break;
      case LEFT:
  	    pos -= right * velocity;
        break;
      case RIGHT:
  	    pos += right * velocity;
        break;
      default:
        break;
    }
  }

  void processMouse(double xoffset, double yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw   += xoffset;
    pitch += yoffset;
    if(pitch > 89.0f)
      pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;
    updateVectors();
  }

  void processScroll(double xoffset, double yoffset) {
    if(fov >= 1.0f && fov <= 45.0f)
    	fov -= yoffset;
    if(fov <= 1.0f)
    	fov = 1.0f;
    if(fov >= 45.0f)
    	fov = 45.0f;
    }
};

