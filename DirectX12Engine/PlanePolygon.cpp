#include "PlanePolygon.h"
#include <../packages/directxtk12_desktop_2017.2021.11.8.1/include/VertexTypes.h>
#include <xutility>


PlanePolygon::PlanePolygon(ID3D12Device* device)
{

	
	
	planePolygon.m_Model = DirectX::Model::CreateFromSDKMESH(device,L"planepoly.sdkmesh");
	
}


