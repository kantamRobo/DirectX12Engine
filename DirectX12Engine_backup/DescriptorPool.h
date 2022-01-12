#pragma once


class DescriptorPool
{
	std::unique_ptr<DirectX::DescriptorHeap> m_SRVCAVUAVHEAPS;

	std::unique_ptr<DirectX::DescriptorHeap> m_Samplers;

	//RTV,DSVは全然知らないので勉強してから実装する。
public:
	DescriptorPool() {};
	DescriptorPool(ID3D12Device* device);
};

