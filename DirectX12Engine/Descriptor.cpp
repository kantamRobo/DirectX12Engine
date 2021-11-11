#include "DescriptorHeap.h"
#include <stdexcept>

void DescriptorHeap::CreateRTVHeap(ID3D12Device* p_device,Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> rtvheap,UINT FrameBufferCount ,UINT&  m_rtvDescriptorSize)
{
	// RTV のディスクリプタヒープ
	HRESULT hr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	  FrameBufferCount,
	  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	  0
	};
	hr = p_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvheap));
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed CreateDescriptorHeap(RTV)");
	}
	m_rtvDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed CreateDescriptorHeap(DSv)");
	}
}


void ::CreateDepthBuffer(ID3D12Device* p_device,int width, int height, Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvheap)
{
	HRESULT hr;
	// DSV のディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
	  1,
	  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	  0
	};
	hr = p_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvheap));

	// デプスバッファの生成
	auto depthBufferDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		width,
		height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.Format = depthBufferDesc.Format;
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.DepthStencil.Stencil = 0;

	HRESULT hr;
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	hr = p_device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthBufferDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer)
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
	p_device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHandle);
}