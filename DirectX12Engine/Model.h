#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <string>
#include "Mesh.h"
#include <vector>
#include <windows.h>
class Model
{

public:
	std::vector<Mesh>meshes;
	std::vector<Vertex>vertices;
	std::vector<unsigned int> indices;
	void Init(const std::string pFile);
private:
	//void ProcessAssimpNode();

	
	
	void ProcessAssimpMesh(const aiScene* pScene);
	void ProcessAssimpNode(aiNode* node, const aiScene* scene);
};

