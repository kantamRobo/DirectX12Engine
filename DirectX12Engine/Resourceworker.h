#pragma once
#include <d3dx12.h>
#include <wrl.h>



Microsoft::WRL::ComPtr<ID3D12Resource1> CreateBuffer(ID3D12Device* p_device, UINT bufferSize, const void* initialData)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource1> buffer;
	auto heaprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto res = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	hr = p_device->CreateCommittedResource(
		&heaprop,
		D3D12_HEAP_FLAG_NONE,
		&res,
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
