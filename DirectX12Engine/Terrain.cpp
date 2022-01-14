#include "Terrain.h"
#include <../directxtk12_desktop_2017.2021.11.8.1/include/VertexTypes.h>

Terrain::Terrain(ID3D12Device* p_device,  DirectX::GraphicsMemory* in_resource)
{
	m_vertexbuffer = in_resource->Allocate(sizeof(DirectX::VertexPositionNormalTexture));

	
}
