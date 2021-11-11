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


		UINT count = FrameBufferCount + 1;
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		  count,
		  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		  0
		};
		p_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_heapSrv));
		m_srvDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// ダイナミックサンプラーのディスクリプタヒープ
		D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		  1,
		  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		  0
		};
		p_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_SamplerHeap));
		m_samplerDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
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
	

	void CreateRTVHeap(ID3D12Device* p_device)
	{
		// CBV/SRV のディスクリプタヒープ
				//  0:シェーダーリソースビュー
				//  1,2 : 定数バッファビュー (FrameBufferCount数分使用)


		UINT count = FrameBufferCount + 1;
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		  count,
		  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		  0
		};
		p_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVheap));
		m_RTVDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		
	    
	}

	void PrepareDSVHeap(ID3D12Device* p_device)
	{
		// CBV/SRV のディスクリプタヒープ
				//  0:シェーダーリソースビュー
				//  1,2 : 定数バッファビュー (FrameBufferCount数分使用)


		UINT count = FrameBufferCount + 1;
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		  D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		  count,
		  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		  0
		};
		p_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVheap));
	m_samplerDescriptorSize = p_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);



	}
};