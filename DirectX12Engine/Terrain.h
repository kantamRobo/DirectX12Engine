#pragma once
#include <GraphicsMemory.h>
#include <../directxtk12_desktop_2017.2021.11.8.1/include/SimpleMath.h>
#include <wrl.h>
#include <Effects.h>
#include <d3dx12.h>
#include <array>
struct Terraintestrange
{
	DirectX::SimpleMath::Vector4 tessRange;
};
class Terrain
{
	

	
	
	
	Terrain(ID3D12Device* p_device, DirectX::GraphicsMemory* in_resource);
	DirectX::GraphicsResource m_vertexbuffer;
	DirectX::GraphicsResource m_indexbuffer;
	Terraintestrange terrainrange;
	DirectX::GraphicsResource terrainrangeCB;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_heightmap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_normalmap;

	std::unique_ptr<DirectX::NormalMapEffect> m_effect;

	std::array<CD3DX12_ROOT_PARAMETER, 2> terrainParameter;
}

;