#include "DX12EngineCore.h"
#include <stdexcept>
#include <wtypes.h>



DX12EngineCore::DX12EngineCore(HWND hwnd)
{

	HRESULT hr;
	UINT dxgiFlags = 0;
#if defined(_DEBUG)
	Microsoft::WRL::ComPtr<ID3D12Debug> debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;

#if 0 // GBV を有効化する場合.
		Microsoft::WRL::ComPtr<ID3D12Debug3> debug3;
		debug.As(&debug3);
		if (debug3)
		{
			debug3->SetEnableGPUBasedValidation(true);
		}
#endif
	}
#endif
	Microsoft::WRL::ComPtr<IDXGIFactory3> factory;
	hr = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateDXGIFactory2 failed.");
	}

	// ハードウェアアダプタの検索
	Microsoft::WRL::ComPtr<IDXGIAdapter1> useAdapter;
	{
		UINT adapterIndex = 0;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		while (DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter))
		{
			DXGI_ADAPTER_DESC1 desc1{};
			adapter->GetDesc1(&desc1);
			++adapterIndex;
			if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			// D3D12は使用可能か
			hr = D3D12CreateDevice(
				adapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				__uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
				break;
		}
		adapter.As(&useAdapter); // 使用するアダプター

		hr = D3D12CreateDevice(useAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
		if (FAILED(hr))
		{
			throw new std::runtime_error("D3D12CreateDevice failed.");
		}


		// HWND からクライアント領域サイズを判定する。
	// (ウィンドウサイズをもらってそれを使用するのもよい)
		RECT rect;
		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		// スワップチェインの生成
		{
			DXGI_SWAP_CHAIN_DESC1 scDesc{};
			scDesc.BufferCount = FrameBufferCount;
			scDesc.Width = width;
			scDesc.Height = height;
			scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			scDesc.SampleDesc.Count = 1;

			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain;
			hr = factory->CreateSwapChainForHwnd(
				m_commandQueue.Get(),
				hwnd,
				&scDesc,
				nullptr,
				nullptr,
				&swapchain);
			if (FAILED(hr))
			{
				throw std::runtime_error("CreateSwapChainForHwnd failed.");
			}

			swapchain.As(&m_swapchain); // IDXGISwapChain4 取得

		}

		CreateCommandAllocators();
		CreateCommandLists();
		CreateCommandQueue();
		m_commandList->Close();

	// 各ディスクリプタヒープの準備.（ディスクリプタクラスから呼ぶ）
	//PrepareDescriptorHeaps();
	// レンダーターゲットビューの生成.
		
		//// デプスバッファ関連の準備.(デイスクリプタクラスからよぶ）
		//CreateDepthBuffer(width, height);

		// コマンドアロケータ－の準備（レンダーから呼ぶ）
		//CreateCommandAllocators();
		// 描画フレーム同期用フェンス生成.
		CreateFrameFences();
		// コマンドリストの生成.(レンダーから呼ぶ）
		/*
		
		hr = m_device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_commandAllocators[0].Get(),
			nullptr,
			IID_PPV_ARGS(&m_commandList)
		);
		m_commandList->Close();
		*/
		m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, float(width), float(height));
		m_scissorRect = CD3DX12_RECT(0, 0, LONG(width), LONG(height));
		m_fenceWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		
	}
}




HRESULT DX12EngineCore::CreateCommandQueue()
{
	// コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC queueDesc{
	  D3D12_COMMAND_LIST_TYPE_DIRECT,
	  0,
	  D3D12_COMMAND_QUEUE_FLAG_NONE,
	  0
	};

	HRESULT hr;
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateCommandQueue failed.");
	}

	return hr;
}


void DX12EngineCore::CreateCommandAllocators()
{
	HRESULT hr;
	m_commandAllocators.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocators[i])
		);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed CreateCommandAllocator");
		}
	}
}

void DX12EngineCore::CreateCommandLists()
{
	HRESULT hr;
	// コマンドリストの生成.
	hr = m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocators[0].Get(),
		nullptr,
		IID_PPV_ARGS(&m_commandList)
	);


}

void DX12EngineCore::CreateFrameFences()
{
	HRESULT hr;
	m_frameFences.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = m_device->CreateFence(
			0,  // 初期値
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_frameFences[i]));
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed CreateFence");
		}
	}
}


void DX12EngineCore::CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVheap)
{
	// スワップチェインイメージへのレンダーターゲットビュー生成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		RTVheap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
		// 参照するディスクリプタの変更
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}
}

void DX12EngineCore::CreateDepthBuffer(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvheap)
{
	HRESULT hr;
	// DSV のディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
	  1,
	  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	  0
	};
	hr = m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvheap));

	const int Width = 1280;
	const int Height = 720;
	// デプスバッファの生成
	auto depthBufferDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Width,
		Height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.Format = depthBufferDesc.Format;
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.DepthStencil.Stencil = 0;

	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	hr = m_device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthBufferDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&m_depthBuffer)
	);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed CreateCommittedResource(DepthBuffer)");
	}

	// デプスステンシルビュー生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc
	{
	  DXGI_FORMAT_D32_FLOAT,  // Format
	  D3D12_DSV_DIMENSION_TEXTURE2D,  // ViewDimension
	  D3D12_DSV_FLAG_NONE,    // Flags
	  { // D3D12_TEX2D_DSV
		0 // MipSlice
	  }
	};
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsvheap->GetCPUDescriptorHandleForHeapStart());
	m_device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, dsvHandle);
}
