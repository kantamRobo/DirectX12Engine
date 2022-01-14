#pragma once
#include <GraphicsMemory.h>
class Terrain
{
	

	
	
	
	Terrain(ID3D12Device* p_device, DirectX::GraphicsMemory* in_resource);
	DirectX::GraphicsResource m_vertexbuffer;
	DirectX::GraphicsResource m_indexbuffer;

	


};

