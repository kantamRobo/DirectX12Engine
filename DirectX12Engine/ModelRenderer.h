#pragma once
#include "Commands.h"
#include "DescriptorHeapContainer.h"
#include "Model.h"
#include "Camera.h"
#include "DescriptorHeapWorker.h"
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <memory>
class DX12EngineCore;
class ModelRendererWorker;
class ModelRenderer
{
public:
	ModelRenderer() {};






	ModelRenderer(const std::shared_ptr<DX12EngineCore> core,
		const Commands& commands,
		std::shared_ptr<Model> in_model,
		const std::shared_ptr<ModelRendererWorker> in_modelRendererWorker,
		const DescriptorHeapsContainer& descheaps);
	
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

	
	


	UINT m_frameIndex;
	const UINT FrameBufferCount = 2;
	HANDLE m_fenceWaitEvent;
	const UINT GpuWaitTimeout = (10 * 1000);  // 10s

	CD3DX12_VIEWPORT  m_viewport;
	CD3DX12_RECT m_scissorRect;
	
	std::shared_ptr<ModelRendererWorker> m_Rendererworker;

	D3D12_GPU_DESCRIPTOR_HANDLE m_sampler;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srv;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_cbViews;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSampler;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapRTV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSRV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapDSV;
	UINT  m_samplerDescriptorSize;
};

