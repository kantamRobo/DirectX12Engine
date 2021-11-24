#pragma once
#include <d3dx12.h>
#include <wrl.h>



static Microsoft::WRL::ComPtr<ID3D12Resource1> CreateBuffer(
	ID3D12Device* p_device,
	D3D12_RESOURCE_DIMENSION in_dimension,
	UINT bufferSize, UINT width, UINT height, DXGI_FORMAT format,
	D3D12_TEXTURE_LAYOUT layout,
	void* initialData)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource1> buffer;
	auto heaprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = {};

	desc.Dimension = in_dimension;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.Format = format;
	desc.Layout = layout;
	hr = p_device->CreateCommittedResource(
		&heaprop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);

	// 初期データの指定があるときにはコピーする
	if (SUCCEEDED(hr) && initialData != nullptr)
	{
		void* mapped;
		CD3DX12_RANGE range(0, 0);
		hr = buffer->Map(0, &range, &mapped);
		if (SUCCEEDED(hr))
		{
			memcpy(mapped, initialData, bufferSize);
			buffer->Unmap(0, nullptr);
		}
	}

	return buffer;
}
