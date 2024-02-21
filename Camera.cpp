/* from https://learnopengl.com/Getting-started/Camera */
#include "Camera.h"

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true) {
	Yaw += (MouseSensitivity * xoffset);
	Pitch += (MouseSensitivity * yoffset);

	if (constrainPitch) {
		if (Pitch > 89) {
			Pitch = 89;
		}
		if (Pitch < 89) {
			Pitch = -89;
		}
	}

	this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 front;

	front.x = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
	front.y = glm::sin(glm::radians(Pitch));
	front.z = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw));

	this->Front = glm::normalize(front);
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}