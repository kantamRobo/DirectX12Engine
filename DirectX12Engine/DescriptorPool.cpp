#include "pch.h"
#include "DescriptorPool.h"


DescriptorPool::DescriptorPool(ID3D12Device* in_device)
{
	auto device = in_device;
	m_SRVCAVUAVHEAPS = std::make_unique<DirectX::DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 512);

	m_Samplers = std::make_unique<DirectX::DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 256);

	device->Release();
}