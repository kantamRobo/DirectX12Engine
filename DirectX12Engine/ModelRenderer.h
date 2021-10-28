#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include "Commands.h"
#include "Descriptor.h"
#include "PipelineObjectContainer.h"
#include "Model.h"
#include "Camera.h"
class ModelRenderer
{
public:
	ModelRenderer() {};
	ModelRenderer(IDXGISwapChain3* swapchain, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>renderTargets, const UINT& rtvDescriptorSize, const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences, const DescriptorHeapsContainer& DSV_RTV, const Commands& commands, const GraphicPipeLineObjectContainer& pipelineobjContainer, std::vector<UINT64> m_frameFenceValues);
	UINT rtvDescriptorSize;
	
	
	//コアクラスでスワップチェーン、レンダーターゲットビュー、デプスステンシルビュー。
// ワーカークラスでパイプラインオブジェクト（シェーダー、ルートシグネチャ、PSO）を作り、
//作成したこれらを、エンジンの初期化クラスでこのクラスのコンストラクタに渡して初期化する)
	
	ModelRenderer();

	//コアクラスから、スワップチェーン、深度バッファ、レンダーターゲット、フェンスを受け取る
	void SetSwapchain(IDXGISwapChain4* swapchain)
	{
		m_swapchain = swapchain;
	}
	void SetRenderTargets(const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>& renderTargets)
	{
		m_renderTargets = renderTargets;
		}
		
	void SetFrameFences(const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> frameFences)
	{
		m_frameFences = frameFences;
		}
	void SetFrameDSV_RTV(const DescriptorHeapsContainer& DSV_RTV)
	{
		m_DSV_RTV = DSV_RTV;
	}
	void SetFrameFenceValues(const std::vector<UINT64>& frameFenceValues)
	{

		m_frameFenceValues = frameFenceValues;

	}

	void SetViewPort_Rect(const CD3DX12_VIEWPORT& viewport, CD3DX12_RECT scissorRect)
	{
		m_viewport = viewport;
		m_scissorRect = scissorRect;
	}

	//ワーカークラスから、コマンド、パイプラインを受け取る
	void SetCommands(const Commands& commands)
	{
		m_commands = commands;
	}
	void SetGraphicPipeLineStateContainer(const GraphicPipeLineObjectContainer& pipelineobjContainer);

	
	void Render();
	const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> constantBuffers;
	void WaitPreviousFrame(ID3D12CommandQueue* queue, const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences, std::vector<UINT64> m_frameFenceValues);
	
	void MakeCommand(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& command, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_constantBuffers, Camera* camera);
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapchain;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>m_renderTargets;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences;


	
	
	std::vector<UINT64> m_frameFenceValues;
	Commands m_commands;
	DescriptorHeapsContainer m_DSV_RTV;
	GraphicPipeLineObjectContainer pipelineobjContainer;
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrvCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSampler;

	UINT m_frameIndex;
	const UINT FrameBufferCount = 2;
	HANDLE m_fenceWaitEvent;
	const UINT GpuWaitTimeout = (10 * 1000);  // 10s

	CD3DX12_VIEWPORT  m_viewport;
	CD3DX12_RECT m_scissorRect;
	std::shared_ptr<Model> m_model;


	D3D12_GPU_DESCRIPTOR_HANDLE m_sampler;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srv;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_cbViews;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrvCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSampler;
	UINT  m_samplerDescriptorSize;
};

