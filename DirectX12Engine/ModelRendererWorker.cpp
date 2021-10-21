#include "ModelRendererWorker.h"



// コマンドキューの生成
D3D12_COMMAND_QUEUE_DESC queueDesc{
  D3D12_COMMAND_LIST_TYPE_DIRECT,
  0,
  D3D12_COMMAND_QUEUE_FLAG_NONE,
  0
};


ModelRendererWorker::ModelRenderWorker()
{
	CreateCommandAllocators();
	CreateCommandQueue();
	CreateCommandLists();
	m_commandList->Close();
}

HRESULT ModelRendererWorker::CreateCommandQueue()
{
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr))
	{
		throw std::runtime_error("CreateCommandQueue failed.");
	}

	return hr;
}


void ModelRendererWorker::CreateCommandAllocators()
{
	HRESULT hr;
	m_commandAllocators.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocators[i])
		);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed CreateCommandAllocator");
		}
	}
}

void ModelRendererWorker::CreateCommandLists()
{
	// コマンドリストの生成.
	hr = m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocators[0].Get(),
		nullptr,
		IID_PPV_ARGS(&m_commandList)
	);
	

}
m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, float(width), float(height));
m_scissorRect = CD3DX12_RECT(0, 0, LONG(width)
