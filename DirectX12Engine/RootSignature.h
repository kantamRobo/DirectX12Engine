#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <d3dcompiler.inl>
class RootSignature
{
	
	void CreateRootSignature(Microsoft::WRL::ComPtr<ID3D12Device> p_device);
	void CreateSampler();

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	Microsoft::WRL::ComPtr<ID3DBlob>errBlob;
};

