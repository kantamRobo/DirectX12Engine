#pragma once
#include <GraphicsMemory.h>
#include <DeviceResources.h>
#include <DescriptorHeap.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Shader.h"
///////////////////////////////////////////////////////////////////////////////
// Transform structure
///////////////////////////////////////////////////////////////////////////////
struct alignas(256) Transform
{
    DirectX::XMMATRIX   World;      // ワールド行列です.
    DirectX::XMMATRIX   View;       // ビュー行列です.
    DirectX::XMMATRIX   Proj;       // 射影行列です.
};

class Renderer
{
public:
    std::shared_ptr<DirectX::DescriptorHeap> m_CBDescriptorHeaps[4];
    Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
    DirectX::GraphicsResource m_transformCBBuffer[4];
	Renderer(DX::DeviceResources* deviceresources,UINT width,UINT height);
    void Render(DX::DeviceResources* deviceresources);
    
    Microsoft::WRL::ComPtr<ID3D12PipelineState>            m_pPSO;                         // パイプラインステートです.

    Transform transform;
    Shader vertexshader;
    Shader pixelshader;
};

