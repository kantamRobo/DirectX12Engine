#pragma once
#define NOMINMAX
#include "Commands.h"
#include "DescriptorHeapContainer.h"
#include "Model.h"
#include "Camera.h"
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <memory>
#include "ShaderParameters.h"
#include "Material.h"
class DX12EngineCore;
class ModelRenderer
{
public:

	
	ModelRenderer(const std::shared_ptr<DX12EngineCore> core, const Commands& commands, std::shared_ptr<Model> in_model, const DescriptorHeapsContainer& descheaps);
	ModelRenderer(const std::shared_ptr<DX12EngineCore> core, const Commands& commands, std::shared_ptr<Model> in_model, const DescriptorHeapsContainer& descheaps, const Material& in_mat);
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_cbSceneViews;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_SceneconstantBuffers;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_materialBuffers;
	UINT size = sizeof(Material) * sizeof(material);
	size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	UINT64 sizeAligned = (size + (align - 1)) & ~(align - 1); // alignに切り上げる.

	
	void CreateSceneView(Microsoft::WRL::ComPtr<ID3D12Device> p_device, const DescriptorHeapsContainer& SceneCBVheap);
	
	void CreateMaterialView(Microsoft::WRL::ComPtr<ID3D12Device> p_device, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> MaterialCBVheap);
	ModelRenderer() {};
	UINT m_cbvDescriptorSize;

	D3D12_CPU_DESCRIPTOR_HANDLE handleCBV;



	
	
	
	UINT rtvDescriptorSize;


	//コアクラスでスワップチェーン、レンダーターゲットビュー、デプスステンシルビュー。
// ワーカークラスでパイプラインオブジェクト（シェーダー、ルートシグネチャ、PSO）を作り、
//作成したこれらを、エンジンの初期化クラスでこのクラスのコンストラクタに渡して初期化する)



	
	
	void Render(std::shared_ptr<Camera> camera);
	void WaitPreviousFrame();
	void MakeCommand(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& command, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_constantBuffers, std::shared_ptr<Camera> camera);
	std::shared_ptr<DX12EngineCore> m_core;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapchain;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>>m_renderTargets;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences;
	std::shared_ptr<Model> m_model;
	
	std::vector<UINT64> m_frameFenceValues;
	Commands m_commands;
	
	ShaderParameters shaderParams;
	Material material;
	
	

	
	UINT m_frameIndex;
	const UINT FrameBufferCount = 2;
	HANDLE m_fenceWaitEvent;
	const UINT GpuWaitTimeout = (10 * 1000);  // 10s

	CD3DX12_VIEWPORT  m_viewport;
	CD3DX12_RECT m_scissorRect;
	

	D3D12_GPU_DESCRIPTOR_HANDLE m_sampler;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srv;
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSceneCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapmatCbv;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSampler;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapRTV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSRV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapDSV;
	UINT  m_samplerDescriptorSize;
};

