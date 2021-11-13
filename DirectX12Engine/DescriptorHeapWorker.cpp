#include "DescriptorHeapWorker.h"
#include <stdexcept>

void DescriptorHeapWorker::CreateRTVHeap(ID3D12Device* p_device,UINT FrameBufferCount)
{
	// RTV のディスクリプタヒープ
	HRESULT hr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	  FrameBufferCount,
	  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	  0
	};
	hr = p_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_heapRtv));
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed CreateDescriptorHeap(RTV)");
	}
	
	
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed CreateDescriptorHeap(DSv)");
	}
}


