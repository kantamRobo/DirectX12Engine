#pragma once
#include <GraphicsMemory.h>
#include <DeviceResources.h>
#include "VertexTypes.h"
#include <DescriptorHeap.h>
#include <vector>
#include <memory>
#include "TransformConstantBufferData.h"
#include "ResourceUploadBatch.h"
class AssimpModel
{
	DirectX::GraphicsResource m_vertexBuffer;
	DirectX::GraphicsResource m_indexBuffer;
	DirectX::GraphicsResource m_transformCBBuffer[4];
	std::shared_ptr<DirectX::DescriptorHeap> m_CBDescriptorHeaps[4];
	std::vector<DirectX::VertexPosition> vertices;
	D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;

	HRESULT Init(DX::DeviceResources* deviceresources, DirectX::GraphicsMemory* graphicsmemory);

	void Render(DX::DeviceResources* deviceresources, DirectX::GraphicsMemory* graphicsmemory);
};

