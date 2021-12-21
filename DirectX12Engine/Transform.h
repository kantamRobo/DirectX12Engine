#pragma once
#include <d3d12.h>
#include <SimpleMath.h>
class Transform
{
public:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector3 ConvertedImpulse;
};

