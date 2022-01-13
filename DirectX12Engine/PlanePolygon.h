#pragma once
#include <GraphicsMemory.h>
#include <wrl.h>]
#include <d3d12.h>
#include <memory>
#include <Effects.h>
#include <SimpleMath.h>

class PlanePolygon
{
public:
	PlanePolygon() {};
	PlanePolygon(ID3D12Device* device);
	std::unique_ptr<DirectX::GraphicsMemory> graphicsmemory;
	DirectX::GraphicsResource vertexbuffer;
	DirectX::GraphicsResource indexbuffer;
	
	std::unique_ptr<DirectX::BasicEffect> m_planepolyeffect;
	Transform m_transform;
	DirectX::SimpleMath::Matrix m_planepolyworld;
	DirectX::SimpleMath::Matrix m_planepolyview;
	DirectX::SimpleMath::Matrix m_planepolyproj;
};

