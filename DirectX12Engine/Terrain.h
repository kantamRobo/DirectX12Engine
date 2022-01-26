#pragma once
#include <GraphicsMemory.h>
#include <SimpleMath.h>
#include <wrl.h>
#include <Effects.h>
#include <d3dx12.h>
#include <array>
#include <Effects.h>
struct Terraintestrange
{
	DirectX::SimpleMath::Vector4 tessRange;
};

struct grayscale {

	const static UINT Width = 1280;
	const static UINT Height = 640;

	UINT grayscale[Width][Height];
};


struct NormalSRGB
{
	const static UINT Width = 1280;
	const static UINT Height = 640;
	DirectX::SimpleMath::Vector3 rgb[Width][Height];
};
class Terrain
{
	

	grayscale m_grayscale;
	
	Terrain(ID3D12Device* p_device, DirectX::GraphicsMemory* in_resource);
	DirectX::GraphicsResource m_vertexbuffer;
	DirectX::GraphicsResource m_indexbuffer;
	Terraintestrange terrainrange;
	DirectX::GraphicsResource terrainrangeCB;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_heightmap;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_normalmap;
	
	std::unique_ptr<DirectX::NormalMapEffect> m_effect;

	std::array<CD3DX12_ROOT_PARAMETER, 2> terrainParameter;

	void SetHeightMap();
};

