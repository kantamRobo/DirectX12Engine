#include"Utility.h"
#include "Transform.h"
#include "bullet3/shared/src/src/btBulletDynamicsCommon.h"
#include <memory>
Transform testtransform;
void SetImpulse_test(float deltatime)
{
	auto mass = 0.3f;
	btVector3 pos = btVector3(-1, 0.5, 0);
	btVector3 testimpulse = btVector3(testtransform.position.x, testtransform.position.y, testtransform.position.z);
	// 球体の初期位置・姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, ));
	// 球体形状を設定
	btSphereShape g_fRad = btSphereShape(1.0f);
	btCollisionShape* sphere_shape = new btSphereShape(g_fRad);
	// 慣性モーメントの計算
	btVector3 inertia(0, 0, 0);
	sphere_shape->calculateLocalInertia(mass, inertia);

	std::unique_ptr<btDbvtBroadphase> broadphase = std::make_unique<btDbvtBroadphase>();
	auto g_pSphereBody = std::make_unique<btRigidBody>(mass, motion_state, sphere_shape, inertia);
	//BULLET3の一部を改造した(ApplyCentralImpulse)
	BULLET3CONVERTER::ApplyCentralImpulse_DirectXTK12VECTOR3(g_pSphereBody.get(), testimpulse, testtransform.ConvertedImpulse);
	
	testtransform.position += testtransform.ConvertedImpulse*deltatime;

}
