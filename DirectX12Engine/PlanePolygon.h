#pragma once
#include <GraphicsMemory.h>
#include <wrl.h>]
#include <d3d12.h>
#include <memory>

class PlanePolygon
{
public:
	PlanePolygon() {};
	PlanePolygon(ID3D12Device* device);
	std::unique_ptr<DirectX::GraphicsMemory> graphicsmemory;
	DirectX::GraphicsResource vertexbuffer;
	DirectX::GraphicsResource indexbuffer;
	

};

