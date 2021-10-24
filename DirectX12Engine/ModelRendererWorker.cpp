#include "ModelRendererWorker.h"
#include <stdexcept>



// コマンドキューの生成
D3D12_COMMAND_QUEUE_DESC queueDesc{
  D3D12_COMMAND_LIST_TYPE_DIRECT,
  0,
  D3D12_COMMAND_QUEUE_FLAG_NONE,
  0
};


ModelRendererWorker::ModelRendererWorker(ID3D12Device* p_device,UINT FrameBufferCount)
{
	CreateCommandAllocators(p_device,FrameBufferCount);
	CreateCommandQueue(p_device);
	CreateCommandLists(p_device);
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


