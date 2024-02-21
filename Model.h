/* from https://learnopengl.com/Model-Loading/Model */
#pragma once

#include "mesh.h"

// SOIL
#include "external/SOIL2/include/SOIL2.h"

/* Model Class */
class Model {
	public:
		Model();
		Model(GLchar* path);
		void draw(Shader shader);
	private:
		// Model data
		vector<Mesh> meshes;
		string directory;
		vector<Texture> textures_loaded;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
		GLuint TextureFromFile(const char* path, string directory);
};