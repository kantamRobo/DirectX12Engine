#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <string>
#include "Mesh.h"
#include <vector>
#include <windows.h>
#include <memory>
class Model
{

public:
	std::vector<Mesh>meshes;
	std::vector<Vertex>vertices;
	std::vector<unsigned int> indices;
	std::vector<Bone> bones;
	DirectX::XMMATRIX modelmat;
	void Init(const std::string pFile);
	void ProcessBoneNode(const aiAnimation* p_animation, const aiScene* pScene, const aiNode* node, const DirectX::XMMATRIX& ParentNodeTransform);
	const aiScene* m_pScene;
private:
	//void ProcessAssimpNode();

	
	
	void ProcessAssimpMesh(const aiScene* pScene);
	
	

	
	
};

struct Bone
{
	char* bonename;
	std::vector<Bone> children;
	std::shared_ptr<Bone> parent;
	DirectX::XMMATRIX transformmatrixforGPU;
};

