#include "PlanePolygon.h"
#include <../packages/directxtk12_desktop_2017.2021.11.8.1/include/VertexTypes.h>
#include <xutility>


PlanePolygon::PlanePolygon(ID3D12Device* device)
{

	DirectX::VertexPosition planevertices[] = {DirectX::XMFLOAT3{ 0.0f,0.0f,0.0f }, DirectX::XMFLOAT3{ 1.0f,0.0f,0.0f }, DirectX::XMFLOAT3{ 0.0f,-1.0f,0.0f }, DirectX::XMFLOAT3{ 1.0f,-1.0f,0.0f} };
	//index 0~3
	
	indexbuffer.Resource()->SetName(L"PlanePoly indexbuffer");
	
	planePolygon.m_Model = DirectX::Model::CreateFromSDKMESH(device,L"planepoly.sdkmesh");
	
}


