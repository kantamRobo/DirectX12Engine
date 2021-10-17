#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "Model.h"
class Animator
{
	Animator(const Model& in_model)
	{
		m_model = in_model;
	}
	Model m_model;
	void CalculateBoneAnimation(const aiScene* pScene, const aiNode* pNode);
};

struct animationNode
{

};

struct AnimationClip 
{

	AnimationChannel channels[];
};

struct  AnimationChannel
{

};