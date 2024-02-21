#pragma once

#include "Model.h"
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Ball Class */
/* represents a ball*/
class Ball {
public:
	Model ball;

	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 virtualPos;

	float abs_velocity = 20.0f;
	
	// Z coordinate of the see through wall
	float invisibleWall_z;
	
	// represents attachment to the collider
	float attached = true;

	// default constructor
	Ball() { }

	Ball(float viewOffset) {
		ball = Model("model/Kugel/kugel.obj");
		invisibleWall_z = viewOffset + 0.5f;
		position = glm::vec3(0.0f, 0.0f, invisibleWall_z);
		time_t t;
		time(&t);
		srand((unsigned int)t);
	}

	// draw ball with shader
	void draw(Shader shader) {
		glm::mat4 model;
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->ball.draw(shader);
	}

	void setUpVirtualPosition(GLfloat dt) {
		virtualPos = position + velocity * dt;
	}

	void updatePosition(GLfloat dt) {
		position = this->virtualPos;
	}

	void setRandomVelocity() {
		float phi = glm::radians((float)(rand() % 360));
		float theta = glm::radians((float) (rand() % 40));
		float v_x = sin(theta) * cos(phi);
		float v_y = sin(theta) * sin(phi);
		float v_z = -cos(theta);
		this->velocity = abs_velocity * glm::vec3(v_x, v_y, v_z);
	}
};
