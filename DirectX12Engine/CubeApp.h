#pragma once

#include "AppBase.h"
#include <DirectXMath.h>
#include "Model.h"
class Camera;
class CubeApp : public AppBase
{
public:
	CubeApp() : AppBase() { }

	virtual void Prepare() override;
	virtual void Cleanup() override;
	virtual void MakeCommand(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& command) override;

	

	struct ShaderParameters
	{
		DirectX::XMFLOAT4X4 mtxWorld;
		DirectX::XMFLOAT4X4 mtxView;
		DirectX::XMFLOAT4X4 mtxProj;
	};

	enum
	{
		TextureSrvDescriptorBase = 0,
		ConstantBufferDescriptorBase = 1,
		// サンプラーは別ヒープなので先頭を使用
		SamplerDescriptorBase = 0,
	};
private:
	Microsoft::WRL::ComPtr<ID3D12Resource1> CreateBuffer(UINT bufferSize, const void* initialData);
	Microsoft::WRL::ComPtr<ID3D12Resource1> CreateTexture(const std::string& fileName);
	void PrepareDescriptorHeapForCubeApp();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrvCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSampler;
	UINT  m_samplerDescriptorSize;

	Model model;
	std::shared_ptr<Camera> camera;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_texture;
	D3D12_VERTEX_BUFFER_VIEW  m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW   m_indexBufferView;
	UINT  m_indexCount;
	Microsoft::WRL::ComPtr<ID3DBlob>  m_vs, m_ps;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipeline;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_constantBuffers;

	D3D12_GPU_DESCRIPTOR_HANDLE m_sampler;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srv;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_cbViews;
};