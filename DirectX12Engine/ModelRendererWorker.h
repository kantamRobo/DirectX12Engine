#pragma once
#include <d3dx12.h>
#include <wrl.h>
class ModelRendererWorker
{
public:
	
	

	ModelRendererWorker(ID3D12Device* p_device, UINT FrameBufferCount);
	HRESULT CreateCommandQueue(ID3D12Device* p_device);
	//こちらでコマンド系インターフェースを作成する。
	CD3DX12_VIEWPORT  m_viewport;
	CD3DX12_RECT m_scissorRect;
	
	
	
	void CreateCommandAllocators(ID3D12Device* p_device, UINT FrameBufferCount);
	void CreateCommandLists(ID3D12Device* p_device);
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1> m_commandList;

	Microsoft::WRL::ComPtr <ID3D12CommandQueue> m_commandQueue;

	std::vector< Microsoft::WRL::ComPtr <ID3D12CommandAllocator>> m_commandAllocators;
	
};

