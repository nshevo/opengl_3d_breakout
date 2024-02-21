/* from https://learnopengl.com/Model-Loading/Mesh */
#pragma	once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Shader.h"

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLEW_STATIC
#include <GL/glew.h> // for opengl headers

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

/* Mesh class */
class Mesh {
	public:	
		// mesh data
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
		void draw(Shader shader);

	private:
		// render data
		GLuint VAO, VBO, EBO;
		void setupMesh();
};