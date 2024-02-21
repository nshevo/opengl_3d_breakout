#pragma once

#include "shader.h"
#define GLEW_STATIC

#include <GL/glew.h> // for opengl
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL
#include "external/SOIL2/include/SOIL2.h"

/* Wall Class*/
class Wall {
public:
	float WIDTH = 13.0f;
	float HEIGHT = 13.0f;
	float DEPTH = 30.0f;
	float VIEW_OFFSET;

	Wall() {
		createVAO();
		createTexture();
		setViewOffset();
	}

	void draw(Shader shader) {
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -DEPTH / 2 + VIEW_OFFSET));
		model = glm::scale(model, glm::vec3(WIDTH, HEIGHT, DEPTH));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(shader.program, "diffuseMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 30); // area of 2 triangles -> -6 points
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint VAO, texture;

	void createVAO() {
		
		GLfloat vertices[] = {
			// Positions           // Normals           // Texture Coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

			//-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			//0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			//0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			//0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			//-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			//-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  2.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  2.0f, 2.0f,
			-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
			-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
			-0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  2.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  2.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  2.0f, 2.0f,
			0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
			0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 2.0f,
			0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  2.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 2.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  2.0f, 2.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  2.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  2.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 2.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,  0.0f, 2.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,  2.0f, 2.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,  2.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,  2.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,  0.0f, 2.0f
		};

		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		this->VAO = VAO;
	}

	void createTexture() {
		int width, height;
		unsigned char* image;
		glGenTextures(1, &texture);
		//diffuse map
		image = SOIL_load_image("wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void setViewOffset() {
		// 45 degree angle
		float div = 2 * tan(22.5f);
		this->VIEW_OFFSET = (-HEIGHT / div);
	}
};