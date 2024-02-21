#pragma once

#include "CubeProducer.h"

/* Colored cube producer class */
/* represents production of colored cube */
class ColoredCubeProducer : public CubeProducer {
	public:
		ColoredCubeProducer() {}

		ColoredCubeProducer(GLfloat red, GLfloat green, GLfloat blue) {
			this->red = red;
			this->green = green;
			this->blue = blue;
		}

		void changeScale(float x, float y, float z) {
			scale_x = x;
			scale_y = y;
			scale_z = z;
		}

		void changeOpacity(float value) {
			opacity = value;
		}

		void draw(Shader shader, float x, float y, float z, GLuint VAO, GLuint diffuseMap, GLuint specularMap) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glUniform1i(glGetUniformLocation(shader.program, "diffuseMap"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
			glUniform1i(glGetUniformLocation(shader.program, "specularMap"), 0);

			glm::mat4 model;
			model = glm::translate(model, glm::vec3(x, y, z));
			model = glm::scale(model, glm::vec3(scale_x, scale_y, scale_z));
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform4f(glGetUniformLocation(shader.program, "inputColor"), red, green, blue, opacity);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	private:
		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat scale_x = 1.0f;
		GLfloat scale_y = 1.0f;
		GLfloat scale_z = 1.0f;
		GLfloat opacity = 1.0f;

};
