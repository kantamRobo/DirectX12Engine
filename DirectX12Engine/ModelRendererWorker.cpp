#include "ModelRendererWorker.h"
#include <stdexcept>
#include "Resourceworker.h"

#include "DescriptorHeapWorker.h"
//レンダラーに、コマンドとシーンビューを渡す為のクラス

// コマンドキューの生成
D3D12_COMMAND_QUEUE_DESC queueDesc{
  D3D12_COMMAND_LIST_TYPE_DIRECT,
  0,
  D3D12_COMMAND_QUEUE_FLAG_NONE,
  0
};


ModelRendererWorker::ModelRendererWorker(Microsoft::WRL::ComPtr<ID3D12Device> p_device,UINT FrameBufferCount, const DescriptorHeapWorker& SceneCBVheap)
{

	CreateCommandAllocators(p_device.Get(),FrameBufferCount);
	CreateCommandQueue(p_device.Get());
	CreateCommandLists(p_device.Get());
	
	
	CreateSceneView(p_device, SceneCBVheap);
	m_commandList->Close();
}

HRESULT ModelRendererWorker::CreateCommandQueue(ID3D12Device* p_device)
{
	HRESULT hr;
	hr = p_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateCommandQueue failed.");
	}

	return hr;
}


void ModelRendererWorker::CreateCommandAllocators(ID3D12Device* p_device,UINT FrameBufferCount)
{
	HRESULT hr;
	m_commandAllocators.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = p_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocators[i])
		);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed CreateCommandAllocator");
		}
	}
}

void ModelRendererWorker::CreateCommandLists(ID3D12Device* p_device)
{
	HRESULT hr;
	// コマンドリストの生成.
	hr = p_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocators[0].Get(),
		nullptr,
		IID_PPV_ARGS(&m_commandList)
	);


}

void ModelRendererWorker::CreateSceneView(Microsoft::WRL::ComPtr<ID3D12Device> p_device, const DescriptorHeapWorker& SceneCBVheap)
{
	

	// 定数バッファ/定数バッファビューの生成
	m_constantBuffers.resize(FrameBufferCount);
	m_cbViews.resize(FrameBufferCount);
	

	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
		m_constantBuffers[i] = CreateBuffer(p_device.Get(),bufferSize, nullptr);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
		cbDesc.BufferLocation = m_constantBuffers[i]->GetGPUVirtualAddress();
		cbDesc.SizeInBytes = bufferSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(SceneCBVheap.m_heapCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
		p_device->CreateConstantBufferView(&cbDesc, handleCBV);

		m_cbViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(SceneCBVheap.m_heapCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
	}

}


