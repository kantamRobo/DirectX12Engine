#include "Model.h"

void Model::Init(const std::string pFile)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile,
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


	ProcessAssimpMesh(scene);
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