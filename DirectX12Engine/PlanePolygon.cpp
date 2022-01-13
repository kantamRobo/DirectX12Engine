#include "PlanePolygon.h"
#include <../packages/directxtk12_desktop_2017.2021.11.8.1/include/VertexTypes.h>
#include <xutility>

PlanePolygon::PlanePolygon(ID3D12Device* device)
{

	DirectX::VertexPosition planevertices[] = {DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f }, DirectX::XMFLOAT3{ 1.0f,0.0f,0.0f }, DirectX::XMFLOAT3{ 0.0f,-1.0f,0.0f }, DirectX::XMFLOAT3{ 1.0f,-1.0f,0.0f} };
	//index 0~3
	auto numindex = { 0,1,2,2,1,3 };
	graphicsmemory = std::make_unique<DirectX::GraphicsMemory>(device);
	vertexbuffer = graphicsmemory->Allocate(sizeof(DirectX::VertexPosition) * sizeof(planevertices));
	vertexbuffer.Resource()->SetName(L"PlanePoly vertexbuffer");
	indexbuffer = graphicsmemory->Allocate(sizeof(numindex));
	memcpy(vertexbuffer.Memory(), planevertices, sizeof(DirectX::VertexPosition) * sizeof(planevertices));
	std::copy(std::begin(numindex), std::end(numindex), indexbuffer.Memory());
	indexbuffer.Resource()->SetName(L"PlanePoly indexbuffer");

}
