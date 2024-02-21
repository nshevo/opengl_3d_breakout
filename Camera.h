/* from https://learnopengl.com/Getting-started/Camera */
#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//default camera values
const GLfloat YAW = -90.0f;

const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 100.0f;
const GLfloat SENSITIVITY = 0.25f;

/* Camera Class */
class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler angles
	GLfloat Yaw;
	GLfloat Pitch;
	// camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY) {
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY) {
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// return view matrix calc using euler
	glm::mat4 getViewMatrix();

	// x,y mouse movement proc
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch);

private:
	// calculate the front vector (also right and up) from cameras updated euler angles
	void updateCameraVectors();
};
#endif
