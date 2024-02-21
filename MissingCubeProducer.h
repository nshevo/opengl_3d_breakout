#pragma once
#include "CubeProducer.h"

/* MissingCubeProducer class */
/* represents absence of a cube */
class MissingCubeProducer : public CubeProducer {

	public:
		MissingCubeProducer() {	}

		void draw(Shader shader, float x, float y, float z, GLuint VAO, GLuint diffuseMap, GLuint specularMap) { }
};
