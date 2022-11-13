#pragma once
#include <DirectXMath.h>

 struct alignas(256) TransformConstantBufferData
{
	 DirectX::XMMATRIX world;
	 DirectX::XMMATRIX view;
	 DirectX::XMMATRIX Projection;
}; 