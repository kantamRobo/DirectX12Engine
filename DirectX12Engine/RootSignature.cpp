#include "RootSignature.h"
#include <stdexcept>

void RootSignature::CreateRootSignature(Microsoft::WRL::ComPtr<ID3D12Device> p_device)
{
	CD3DX12_DESCRIPTOR_RANGE cbv, srv, sampler;
	cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0 レジスタ
	srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ
	sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0); // s0 レジスタ

	CD3DX12_ROOT_PARAMETER rootParams[3];
	rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_VERTEX);
	rootParams[1].InitAsDescriptorTable(1, &srv, D3D12_SHADER_VISIBILITY_PIXEL);
	rootParams[2].InitAsDescriptorTable(1, &sampler, D3D12_SHADER_VISIBILITY_PIXEL);

	// ルートシグネチャの構築
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.Init(
		_countof(rootParams), rootParams,   //pParameters
		0, nullptr,   //pStaticSamplers
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	auto hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
	if (FAILED(hr))
	{
		throw std::runtime_error("D3D12SerializeRootSignature faild.");
	}
	// RootSignature の生成
	hr = p_device->CreateRootSignature(
		0,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature)
	);
	if (FAILED(hr))
	{
		throw std::runtime_error("CrateRootSignature failed.");
	}

}

void RootSignature::CreateSampler()
{
	// サンプラーの生成
	D3D12_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D12_ENCODE_BASIC_FILTER(
		D3D12_FILTER_TYPE_LINEAR, // min
		D3D12_FILTER_TYPE_LINEAR, // mag
		D3D12_FILTER_TYPE_LINEAR, // mip
		D3D12_FILTER_REDUCTION_TYPE_STANDARD);
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

}
