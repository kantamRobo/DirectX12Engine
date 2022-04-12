#include "PlanePolygon.h"
#include <VertexTypes.h>
#include <xutility>


PlanePolygon::PlanePolygon(ID3D12Device* device)
{

	
	
	planePolygon.m_Model = DirectX::Model::CreateFromSDKMESH(device,L"planepoly.sdkmesh");
	
}


