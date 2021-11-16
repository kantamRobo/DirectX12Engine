#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>


class DX12EngineCore
{
public:
	DX12EngineCore(HWND hwnd);
	void CreateFrameFences();
	void PrepareRenderTargetView();


	void CreateDepthBuffer(ID3D12Device* p_device, int width, int height, Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvheap);
	const UINT GpuWaitTimeout = (10 * 1000);  // 10s
	const UINT FrameBufferCount = 2;

	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapchain;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapRtv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapDsv;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource1>> m_renderTargets;
	Microsoft::WRL::ComPtr<ID3D12Resource1> m_depthBuffer;

	CD3DX12_VIEWPORT  m_viewport;
	CD3DX12_RECT m_scissorRect;

	UINT m_rtvDescriptorSize;
	UINT m_srvcbvDescriptorSize;

	HANDLE m_fenceWaitEvent;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Fence1>> m_frameFences;
	std::vector<UINT64> m_frameFenceValues;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1> m_commandList;

	Microsoft::WRL::ComPtr <ID3D12CommandQueue> m_commandQueue;

	std::vector< Microsoft::WRL::ComPtr <ID3D12CommandAllocator>> m_commandAllocators;

	
	HRESULT CreateCommandQueue();
	void CreateCommandAllocators();
	

	void CreateCommandLists();
	const UINT FrameBufferCount = 2;
};

