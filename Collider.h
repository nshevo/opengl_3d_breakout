#pragma once

#include "ColoredCubeProducer.h"
#include "cube.h"
#include "wall.h"
#include "Ball.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Collider class */
/* represents see through cube as a "tennis racket" */
class Collider {
	public:

		Collider() {}

		Collider(Wall & wall, Ball *ball) {
			width = wall.WIDTH;
			height = wall.HEIGHT;
			viewOffset = wall.VIEW_OFFSET;
			position = glm::vec3(0.0f, 0.0f, viewOffset + zDimension / 2.0f);
			coloredCubeProducer = new ColoredCubeProducer(1.0f, 0.8f, 0.0f);
			(*coloredCubeProducer).changeScale(xDimension, yDimension, zDimension);
			(*coloredCubeProducer).changeOpacity(0.01f);
			cube.setCubeProducer(coloredCubeProducer);
			this->ball = ball;
		}
		
		int getBallMissedCollider() {
			return ballMissedCollider;
		}

		void draw(Shader shader) {
			this->cube.draw(shader, position.x, position.y, position.z);
		}

		void update() {
			if ((*ball).virtualPos.z > (*ball).invisibleWall_z) {
				float dist = (*ball).virtualPos.z - (*ball).invisibleWall_z;
				float dt = dist / (*ball).velocity.z;
				glm::vec3 contactPoint = (*ball).virtualPos - dt * (*ball).velocity;
				if (inRange(contactPoint)) {
					(*ball).setRandomVelocity();
					(*ball).virtualPos = glm::vec3(position.x, position.y, (*ball).invisibleWall_z);
					(*ball).virtualPos += dt * (*ball).velocity;
				}
			}
			if ((*ball).virtualPos.z >= -2.0) {
				(*ball).velocity = glm::vec3(0);
				(*ball).attached = true;
				(*ball).virtualPos.x = position.x;
				(*ball).virtualPos.y = position.y;
				(*ball).virtualPos.z = (*ball).invisibleWall_z;
				ballMissedCollider += 1;
			}
		}

		bool inRange(glm::vec3 contactPoint) {
			bool in_x_range = abs(contactPoint.x - position.x) < xDimension / 2;
			bool in_y_range = abs(contactPoint.y - position.y) < yDimension / 2;
			return in_x_range && in_y_range;
		}

		void processMouseMovement(GLfloat xoffset, GLfloat yoffset) {
			xoffset *= mouse_sensitivity;
			yoffset *= mouse_sensitivity;

			correctPosition(0, xoffset, width);
			correctPosition(1, -yoffset, height);

			if ((*ball).attached) {
				(*ball).position.x = position.x;
				(*ball).position.y = position.y;
			}
		}

		void correctPosition(int index, float offset, float range) {
			if (abs(position[index] + offset) < range / 2 - xDimension / 2) {
				position[index] += offset;
			}
			else if (position[index] + offset >= range / 2 - xDimension / 2) {
				position[index] = range / 2 - xDimension / 2;
			}
			else {
				position[index] = -range / 2 + xDimension / 2;
			}
		}

		void releaseBall() {
			if ((*ball).attached) {
				(*ball).attached = false;
				(*ball).setRandomVelocity();
			}
		}

		private:
			glm::vec3 position;
			Cube cube;
			Ball *ball;
			ColoredCubeProducer *coloredCubeProducer;

			int ballMissedCollider = 0;

			float width;
			float height;
			float viewOffset;
			float xDimension = 4.0f;
			float yDimension = 4.0f;
			float zDimension = 0.05f;
			float mouse_sensitivity = 0.01;
};