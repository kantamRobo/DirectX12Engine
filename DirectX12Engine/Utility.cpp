#include "Utility.h"

void BULLET3CONVERTER::ConvertBtVector3toVector3(const btVector3& src, DirectX::SimpleMath::Vector3& dst)
{
	dst.x = src.x();
	dst.x = src.y();
	dst.x = src.z();
}

void BULLET3CONVERTER::ApplyCentralImpulse_DirectXTK12VECTOR3( btRigidBody* rigidbody,const btVector3& impulse, DirectX::SimpleMath::Vector3& dst)
{
	rigidbody->applyCentralImpulse(impulse);

	


}