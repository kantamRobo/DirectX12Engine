#pragma once
#include <GraphicsMemory.h>
#include <DeviceResources.h>
#include "VertexTypes.h"
#include <DescriptorHeap.h>
#include <vector>
#include <memory>
#include "TransformConstantBufferData.h"
#include "ResourceUploadBatch.h"
#include <wrl.h>
#include <d3dx12.h>
#include <Shader.h>
class AssimpModel
{
	DirectX::GraphicsResource m_vertexBuffer;
	DirectX::GraphicsResource m_indexBuffer;
	DirectX::GraphicsResource m_transformCBBuffer[4];
	std::shared_ptr<DirectX::DescriptorHeap> m_CBDescriptorHeaps[4];
	std::vector<DirectX::VertexPosition> vertices;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>			m_pipelineState;
	D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;
	Shader vertexshader;
	Shader pixelshader;
	HRESULT Init(DX::DeviceResources* deviceresources, DirectX::GraphicsMemory* graphicsmemory);

	void Render(DX::DeviceResources* deviceresources, DirectX::GraphicsMemory* graphicsmemory);
};

