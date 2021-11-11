#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "Resourceworker.h"


struct DescriptorHeap {
	DescriptorHeap() {};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapSrv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapUav;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heapRtv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_HeapDsv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_HeapSampler;
	UINT  m_samplerDescriptorSize;
	D3D12_GPU_DESCRIPTOR_HANDLE m_sampler;
	D3D12_GPU_DESCRIPTOR_HANDLE m_srv;
	UINT m_RTVDescriptorSize=0;
	//D3D12_CPU_DESCRIPTOR_HANDLE m_RTVDescriptorSize;
	const UINT FrameBufferCount = 2;
	UINT m_srvDescriptorSize = 0;
	UINT m_cbvDescriptorSize = 0;
	



	void PrepareModelSRV_SamplerHeaps(ID3D12Device* p_device)
	{
		// CBV/SRV のディスクリプタヒープ
		//  0:シェーダーリソースビュー
		//  1,2 : 定数バッファビュー (FrameBufferCount数分使用)

		

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvHeapDesc.NodeMask = 0;

		p_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_heapSrv));
		
		// ダイナミックサンプラーのディスクリプタヒープ
		D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		  1,
		  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		  0
		};
		p_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_HeapSampler));
		//m_samplerDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	}

	void CreateSceneCBVHeaps(ID3D12Device* p_device)
	{
		// CBV/SRV のディスクリプタヒープ
		//  0:シェーダーリソースビュー
		//  1,2 : 定数バッファビュー (FrameBufferCount数分使用)

		UINT count = FrameBufferCount + 1;
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		  count,
		  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		  0
		};

		p_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_SamplerHeap));
		m_cbvDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	}
	


	void CreateRTVHeap(ID3D12Device* p_device, Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> rtvheap, UINT FrameBufferCount, UINT& m_rtvDescriptorSize);
	void CreateDSVHeap(ID3D12Device* p_device)
	{
		// CBV/SRV のディスクリプタヒープ
				//  0:シェーダーリソースビュー
				//  1,2 : 定数バッファビュー (FrameBufferCount数分使用)


		UINT count = FrameBufferCount + 1;
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		  count,
		  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		  0
		};
		p_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_HeapDsv));
	m_dsvDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);



	}
	
};