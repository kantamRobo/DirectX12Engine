#pragma once
#include "pch.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "Transform.h"
#include "Transform.h"
class Rigidshape
{
public:
	Rigidshape();
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;

	std::unique_ptr<btRigidBody>  rigidbody;
	Transform m_transform;
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//tick

	void Tick();
	std::unique_ptr<DirectX::BasicEffect> m_effect;
};

