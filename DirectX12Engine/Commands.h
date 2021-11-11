#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
struct Commands
{
	
	
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocators;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> list;
	Microsoft::WRL::ComPtr <ID3D12CommandQueue> queue;


};