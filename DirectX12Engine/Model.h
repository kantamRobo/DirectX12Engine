#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <string>
#include "Mesh.h"
#include <vector>
#include <windows.h>
#include <memory>
#include <cassert>
#include <wrl.h>
#include <d3dx12.h>
#include "Commands.h"
class DX12EngineCore;
enum
{
	TextureSrvDescriptorBase = 0,
	ConstantBufferDescriptorBase = 1,
	// サンプラーは別ヒープなので先頭を使用
	SamplerDescriptorBase = 0,
};
class Model
{

	struct ShaderParameters
	{
		DirectX::XMFLOAT4X4 mtxWorld;
		DirectX::XMFLOAT4X4 mtxView;
		DirectX::XMFLOAT4X4 mtxProj;
	};

public:
	
	Model(ID3D12Device* p_device, const Commands& in_commands, std::string pFile, const std::shared_ptr<DX12EngineCore> in_core);
	std::vector<Mesh>meshes;
	std::vector<Vertex>vertices;
	std::vector<unsigned int> indices;
	std::vector<Bone> bones;
	DirectX::XMFLOAT4X4 modelmat;
	
	
	void Init(ID3D12Device* p_device, const Commands& in_commands, std::string pFile, UINT frameIndex);
	UINT mcurrentAnimIndex;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_texture;
	UINT  m_indexCount;
	UINT m_frameIndex;
	D3D12_VERTEX_BUFFER_VIEW  m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW   m_indexBufferView;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrvCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSampler;
	UINT  m_samplerDescriptorSize;
	D3D12_GPU_DESCRIPTOR_HANDLE m_sampler;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srv;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_cbViews;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_constantBuffers;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	UINT m_srvcbvDescriptorSize;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipeline;
	Microsoft::WRL::ComPtr<ID3DBlob>  m_vs, m_ps;
	void ProcessBoneNode(const aiAnimation* p_animation, const aiScene* pScene, const aiNode* node, FLOAT AnimationTime, const DirectX::XMMATRIX& ParentNodeTransform);
	const aiScene* m_pScene;
private:
	
	//void ProcessAssimpNode();

	
	
	void ProcessAssimpMesh(const aiMesh* p_mesh);
	
	

	
	
	
	
	void CreateVertexIndexBuffer(ID3D12Device* p_device);
	
	
	void Prepare(ID3D12Device* p_device, const Commands& in_commands, UINT in_FrameIndex);
	Microsoft::WRL::ComPtr<ID3D12Resource1> CreateBuffer(ID3D12Device* p_device, UINT bufferSize, const void* initialData);
	
	void PrepareDescriptorHeapForCubeApp(ID3D12Device* p_device);
	
	
	
	Microsoft::WRL::ComPtr<ID3D12Resource1> CreateTexture(const std::wstring& fileName, ID3D12Device* p_device, const Commands& in_commands, UINT in_frameIndex);
	HRESULT CompileShaderFromFile(const std::wstring& fileName, const std::wstring& profile, Microsoft::WRL::ComPtr<ID3DBlob>& shaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob);
	const UINT FrameBufferCount = 2;
};

struct Bone
{
	char* bonename;
	std::vector<Bone> children;
	std::shared_ptr<Bone> parent;
	DirectX::XMMATRIX transformmatrixforGPU;
	DirectX::XMMATRIX boneoffsetmatrix;
};


