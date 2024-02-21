/* from https://learnopengl.com/Getting-started/Shaders */
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

class Shader {
public: 

	// programm id
	GLuint program;
	Shader();
	// constructor to read and write the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// use programm
	void use();
};

#endif // !SHADER_H
