#pragma once
#include "shader.h"

#define GLEW_STATIC
#include "GL/glew.h"

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* CubeProducer abstract class */
class CubeProducer {
	public:
		CubeProducer() {	}

		virtual void draw(Shader shader, float x, float y, float z, GLuint VAO, GLuint diffuseMap, GLuint specularMap) = 0;
};