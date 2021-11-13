#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include "DescriptorHeapWorker.h"

struct DescriptorHeapsContainer
{
	DescriptorHeapsContainer() {};
	DescriptorHeapsContainer(const DescriptorHeapWorker& worker)
	{
		heapRtv = worker.m_heapRtv;
		heapDsv = worker.m_HeapDsv;
		heapCbv = worker.m_heapCbv;
		heapUav = worker.m_heapUav;
		heapSrv = worker.m_heapSrv;

		heapSampler = worker.m_HeapSampler;
	}
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapRtv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapDsv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapSrv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapUav;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapSampler;



};