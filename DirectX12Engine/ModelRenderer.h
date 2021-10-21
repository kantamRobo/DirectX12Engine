#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include "Commands.h"
#include "Descriptor.h"
#include "PipelineObjectContainer.h"
class ModelRenderer
{
public:
	ModelRenderer() {};
	
	
	
	//コアクラスでスワップチェーン、レンダーターゲットビュー、デプスステンシルビュー。
// ワーカークラスでパイプラインオブジェクト（シェーダー、ルートシグネチャ、PSO）を作り、
//作成したこれらを、エンジンの初期化クラスでこのクラスのコンストラクタに渡して初期化する)
	
	ModelRenderer(IDXGISwapChain3* swapchain, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>renderTargets, const UINT& rtvDescriptorSize, const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences, const DescriptorHeapsContainer& DSV_RTV, const Commands& commands, const GraphicPipeLineObjectContainer& pipelineobjContainer, std::vector<UINT64> m_frameFenceValues);
	void Render(
		
		
		
	);
	const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> constantBuffers;
	void WaitPreviousFrame(ID3D12CommandQueue* queue, const std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences, std::vector<UINT64> m_frameFenceValues);
	void MakeCommand(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& command, std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_constantBuffers;);
	
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapchain;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>m_renderTargets;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences;
	
	std::vector<UINT64> m_frameFenceValues;
	Commands m_commands;
	DescriptorHeapsContainer m_DSV_RTV;
	GraphicPipeLineObjectContainer pipelineobjContainer;
	
	UINT m_frameIndex;
	const UINT FrameBufferCount = 2;
	HANDLE m_fenceWaitEvent;
	const UINT GpuWaitTimeout = (10 * 1000);  // 10s
};

