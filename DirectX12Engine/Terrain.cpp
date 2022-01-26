#include "Terrain.h"
#include <../directxtk12_desktop_2017.2021.11.8.1/include/VertexTypes.h>
#include <ResourceUploadBatch.h>
#include <DDSTextureLoader.h>
#include "Animation.h"
#include "pch.h"
#include <random>
#include <stb_image_write.h>
Terrain::Terrain(ID3D12Device* p_device,  DirectX::GraphicsMemory* in_resource)
{
	m_vertexbuffer = in_resource->Allocate(sizeof(DirectX::VertexPositionNormalTexture));

	terrainrangeCB = in_resource->AllocateConstant(terrainrange);

	DirectX::ResourceUploadBatch resourceUpload(p_device);
	
	
	/*
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(p_device, resourceUpload, L"normalMap.dds",
			m_normalmap.ReleaseAndGetAddressOf(), false));DX::ThrowIfFailed(
				
				CreateDDSTextureFromFile(p_device, resourceUpload, L"heightMap.dds",
			m_normalmap.ReleaseAndGetAddressOf(), false));
			*/
}

void Terrain::SetHeightMap()
{
	for (UINT i = 0; i < m_grayscale.Width; i++)
	{
		for (UINT j = 0; i < m_grayscale.Height; j++) {
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			m_grayscale.grayscale[i][j] = engine();
		}
	}
	stbi_write_png("heightmap.png", m_grayscale.Width, m_grayscale.Height
		, static_cast<int>(sizeof(grayscale)), &m_grayscale,0);
}