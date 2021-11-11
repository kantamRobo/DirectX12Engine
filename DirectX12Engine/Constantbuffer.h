#pragma once

#include <d3dx12.h>
#include "Resourceworker.h"
#include  "DescriptorHeap.h"
#include <wrl.h>
#include <vector>

 
class Constantbuffer
{
public:
	//framebuffer分作成する
	std::vector<Microsoft::WRL ::ComPtr<ID3D12Resource1>> m_constbuffers;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_cbViews;
	template<class T>
	void CreateConstBuffers(UINT FrameBufferCount, T src,const DescriptorHeap* cbvDescHeap)
	{
		//定数バッファ/定数バッファビューの作成
		m.resize(FrameBufferCount);
		m_cbViews.resize(FrameBufferCount);
		for (UINT i = 0; i < FrameBufferCount; ++i)
		{
			UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
			m_constantBuffers[i] = CreateBuffer(bufferSize, nullptr);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
			cbDesc.BufferLocation = m_constantBuffers[i]->GetGPUVirtualAddress();
			cbDesc.SizeInBytes = bufferSize;
			CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(cbvDescHeap.m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
			m_device->CreateConstantBufferView(&cbDesc, handleCBV);

			m_cbViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
		}
	}
	
};




