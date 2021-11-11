#pragma once
#include <d3dx12.h>
#include <wrl.h>


struct DescriptorHeapsContainer
{
	DescriptorHeapsContainer() {};
	DescriptorHeapsContainer(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> in_heapRtv,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>in_heapDsv,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> in_heapSrv,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> in_heapCbv,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> in_heapUav,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> in_Sampler)
	{
		heapRtv = in_heapRtv;
		heapDsv = in_heapDsv;
		heapCbv = in_heapCbv;
		heapUav = in_heapUav;
		heapSrv = in_heapSrv;

		heapSampler = in_Sampler;
	}
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapRtv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapDsv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapSrv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapCbv;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapUav;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapSampler;



};