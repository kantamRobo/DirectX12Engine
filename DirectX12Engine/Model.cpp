#include "Model.h"
#include <atlstr.h>

void Model::Init(const std::string pFile)
{
	Assimp::Importer importer;
	m_pScene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_RemoveComponent |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ValidateDataStructure |
		//aiProcess_ImproveCacheLocality | // handled by optimizePostTransform()
		aiProcess_RemoveRedundantMaterials |
		aiProcess_SortByPType |
		aiProcess_FindInvalidData |
		aiProcess_GenUVCoords |
		aiProcess_TransformUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph);


	ProcessAssimpMesh(m_pScene);
}

// for: NumChildren





void Model::ProcessAssimpNode(aiNode* node ,const aiScene* scene)
{

}


void Model::ProcessAssimpMesh(


	const aiScene* pScene

)
{


	meshes.resize(pScene->mNumMeshes);
	modelmat = DirectX::XMMatrixTranspose(convertAImatrix4x4toXMMATRIX(pScene->mRootNode->mTransformation));

	for (unsigned int meshIndex = 0; meshIndex < pScene->mNumMeshes;
		meshIndex++) {
		const aiMesh* srcMesh = pScene->mMeshes[meshIndex];
		Mesh* dstMesh = meshes.data() + meshIndex;

		dstMesh->VertexCount = srcMesh->mNumVertices;
		dstMesh->IndexCount = srcMesh->mNumFaces * 3;

		for (unsigned int v = 0; v < dstMesh->VertexCount; v++)
		{
			Vertex vertex = {};
			vertex.Pos.x = srcMesh->mVertices[v].x;
			vertex.Pos.y = srcMesh->mVertices[v].y;
			vertex.Pos.z = srcMesh->mVertices[v].z;

			if (srcMesh->mTextureCoords[0])
			{
				vertex.UV.x = srcMesh->mTextureCoords[0][v].x;
				vertex.UV.y = srcMesh->mTextureCoords[0][v].y;
			}

			/*
			if (srcMesh->mNormals)
			{
				dstNormal[0] = srcMesh->mNormals[v].x;
				dstNormal[1] = srcMesh->mNormals[v].y;
				dstNormal[2] = srcMesh->mNormals[v].z;
			}

			 if (srcMesh->mTangents)
			{
				dstTangent[0] = srcMesh->mTangents[v].x;
				dstTangent[1] = srcMesh->mTangents[v].y;
				dstTangent[2] = srcMesh->mTangents[v].z;
			}

			  if (srcMesh->mBitangents)
			{
				dstBitangent[0] = srcMesh->mBitangents[v].x;
				dstBitangent[1] = srcMesh->mBitangents[v].y;
				dstBitangent[2] = srcMesh->mBitangents[v].z;
			}

			*/
			vertices.push_back(vertex);
			unsigned int tempindex = 0;
			for (unsigned int f = 0; f < srcMesh->mNumFaces; f++)
			{
				assert(srcMesh->mFaces[f].mNumIndices == 3);

				aiFace face = srcMesh->mFaces[f];
				for (UINT j = 0; j < face.mNumIndices; j++) {
					indices.push_back(tempindex);

				}
			}




		}
	}
	UINT Nummesh = pScene->mNumMeshes;
	const aiMesh* submesh = nullptr;
	aiBone* pBone = nullptr;
	UINT nBoneIndex = 0;
	UINT NumBones = 0;
	for (UINT i = 0; i < Nummesh; i++)
	{
		submesh = pScene->mMeshes[i];

		for (UINT j = 0; j < submesh->mNumBones; j++)
		{
			nBoneIndex = 0;
			pBone = submesh->mBones[j];

			nBoneIndex = NumBones++;

			//XMMATRIX
			bones[NumBones].boneoffsetmatrix = DirectX::XMMatrixTranspose(pBone->mOffsetMatrix);
			//aiMatrix4x4から、XMMATRIXへの変換関数を作成しておく
		}

		for (UINT k = 0; k < pBone->mNumWeights; k++)
		{
			mesh.mBone.VertexID[k] = pBone->mWeights[k].mVertexId;
			mesh.mBone.Weights.weight[k] = pBone->mWeights[k].mWeight;
			
		}
	}

}

	void Model::ProcessBoneNode(const aiAnimation* p_animation,const aiScene* pScene, const aiNode* node,
	const DirectX::XMMATRIX& ParentNodeTransform)
	{
		DirectX::XMMATRIX NodeTransformation = DirectX::XMMatrixIdentity();
		convertAImatrix4x4toXMMATRIX(NodeTransformation, node->mTransformation);

		CStringA strNodeName(node->mName.data);
		const aiNodeAnim* pNodeAnim =SearchNodeAnim(p_animation, strNodeName);
		//名前がstrNodeNameと一致するアニメーションが、p_Animationのチャンネルの中から存在していたら
		//それをpNodeAnimの内容とする。
          

		if (pNodeAnim)
		{
			//pnodeanimのデータを利用し、
			//スケーリング、回転、平行移動の計算を行う
			//計算した上記三つの結果を、NodeTransformationに代入する。
			NodeTransformation = calclatedScaling * calclatedRotation * calclatedTranslation;
		}
		UINT nBoneIndex = 0;
		//親ノード空間へと座標変換
		DirectX::XMMATRIX GlobalTransformation = NodeTransformation * ParentNodeTransform;
		//GPUに渡すボーン行列に座標変換
		bones[nBoneIndex].transformmatrixforGPU = bones[nBoneIndex].boneoffsetmatrix * GlobalTransformation *
			modelmat;
		for (UINT i = 0; i < node->mNumChildren; i++) {
			ProcessBoneNode(p_animation, pScene, node->mChildren[i], ParentNodeTransform);
		}
		
	}
