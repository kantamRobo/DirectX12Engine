#pragma once
#include "Transform.h"
#include <DirectXMath.h>
class Camera
{
public:
	Camera() {
		
	}
	
	
	Transform m_transform;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	void Move();


};

