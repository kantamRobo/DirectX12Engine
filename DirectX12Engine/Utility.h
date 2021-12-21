#pragma once
#include "bullet3/shared/src/src/LinearMath/btVector3.h"
#include "SimpleMath.h"
#include "bullet3/shared/src/src/BulletDynamics/Dynamics/btRigidBody.h"

namespace BULLET3CONVERTER {
	void ConvertBtVector3toVector3(const btVector3& src, DirectX::SimpleMath::Vector3& dst);
		


	
	
	
	void ApplyCentralImpulse_DirectXTK12VECTOR3(btRigidBody* rigidbody, const btVector3& impulse, DirectX::SimpleMath::Vector3& dst);
}