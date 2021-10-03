#pragma once
#include "Shape.h"

struct contact_t {
	DirectX::XMVECTOR ptOnA_WorldSpace;
	DirectX::XMVECTOR ptOnB_WorldSpace;
	DirectX::XMVECTOR ptOnA_LocalSpace;
	DirectX::XMVECTOR ptOnB_LocalSpace;

	DirectX::XMVECTOR normal;	// In World Space coordinates
	float separationDistance;	// positive when non-penetrating, negative when penetrating
	float timeOfImpact;

	Body* bodyA;
	Body* bodyB;

	
};

void ResolveContact(contact_t& contact)
{
	//ローカルで生ポインタは使いたくないので、ここは関係するもの含め
	//スマートポインタに直す。
	Body* bodyA = contact.bodyA;
	Body* bodyB = contact.bodyB;

	bodyA->m_linearVelocity.x = bodyA->m_linearVelocity.y
		= bodyA->m_linearVelocity.z = bodyB->m_linearVelocity.x
		= bodyB->m_linearVelocity.y = bodyB->m_linearVelocity.z = 0;

	const DirectX::XMVECTOR ptOnA = contact.ptOnA_WorldSpace;
	const DirectX::XMVECTOR ptOnB = contact.ptOnB_WorldSpace;

	const float invMassA = bodyA->m_invMass;
	const float invMassB = bodyB->m_invMass;

	auto invWorldInertiaA = bodyA->GetInverseInertiaTensorWorldSpace();
	auto invWorldInertiaB = bodyB->GetInverseInertiaTensorWorldSpace();

	const DirectX::XMVECTOR n = contact.normal;

	const DirectX::XMVECTOR ra = DirectX::XMVectorSubtract(ptOnA, bodyA->GetCenterOfMassWorldSpace());
	const DirectX::XMVECTOR rb = DirectX::XMVectorSubtract(ptOnB, bodyB->GetCenterOfMassWorldSpace());

	const DirectX::XMVECTOR angulerJA = DirectX::XMVector3Transform(DirectX::XMVector3Cross(ra, n), DirectX::XMLoadFloat3x3(&invWorldInertiaA));
	const DirectX::XMVECTOR angulerJB = DirectX::XMVector3Transform(DirectX::XMVector3Cross(rb, n), DirectX::XMLoadFloat3x3(&invWorldInertiaB));



	//移動と回転のワールド空間における速度を取得する
	const DirectX::XMVECTOR velA = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bodyA->m_linearVelocity), DirectX::XMVector3Cross(DirectX::XMLoadFloat3
	(&bodyA->m_angularVelocity), ra));
	const DirectX::XMVECTOR velB = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bodyB->m_linearVelocity), DirectX::XMVector3Cross(DirectX::XMLoadFloat3
	(&bodyB->m_angularVelocity), rb));
	const DirectX::XMVECTOR& vab = DirectX::XMVectorSubtract(velA
		, velB);
	//衝突の衝撃を計算

	const  DirectX::XMVECTOR InpulseJ = DirectX::XMVectorScale(DirectX::XMVectorScale(DirectX::XMVector3Dot(vab, n), -(1.0f + elasticity))
		, 1 / (invMassA + invMassB));
	const DirectX::XMVECTOR vectorImpulseJ = DirectX::XMVectorMultiply(n, InpulseJ);

	bodyA->ApplyImpulse(ptOnA, DirectX::XMVectorScale(vectorImpulseJ, -1.0f));
	bodyB->ApplyImpulse(ptOnB, DirectX::XMVectorScale(vectorImpulseJ, 1.0f));


	const float angularFactor = DirectX::XMVector3Dot(DirectX::XMVectorAdd(angulerJA, angulerJB), n).m128_f32[0];

	
	const float elasticityA = bodyA->m_elasticity;
	const float elasticityB = bodyB->m_elasticity;
	const float elasticity = elasticityA * elasticityB;

   
	

	//摩擦による衝撃を計算する
	const float frictionA = bodyA->m_friction;
	const float frictionB = bodyB->m_friction;
	const float friction = frictionA * frictionB;

	//衝突の法線に対する速度の方向の法線を見つける
	const DirectX::XMVECTOR velNorm = DirectX::XMVectorMultiply(n, DirectX::XMVector3Dot(n, vab));

	//衝突の法線に対する速度の方向の接線を見つける
	const DirectX::XMVECTOR velTang = DirectX::XMVectorSubtract(vab , velNorm);

	//他のボディに対する旋回速度を取得する
	DirectX::XMVECTOR relativeVelTang = velTang;
	DirectX::XMVector3Normalize(relativeVelTang);

	const auto inertiaA = DirectX::XMVector3Cross(ra, DirectX::XMVector3Transform(DirectX::XMVector3Cross(
		ra, relativeVelTang), DirectX::XMLoadFloat3x3(&invWorldInertiaA)));
	const auto inertiaB = DirectX::XMVector3Cross(rb, DirectX::XMVector3Transform(DirectX::XMVector3Cross(
		ra, relativeVelTang), DirectX::XMLoadFloat3x3(&invWorldInertiaB)));

	const float invInertia = DirectX::XMVector3Dot(
		DirectX::XMVectorAdd(inertiaA, inertiaB), (relativeVelTang)).m128_f32[0];

	//摩擦の接線力積を計算する
	const float redecedMass = 1.0f / (bodyA->m_invMass + bodyB->m_invMass + invInertia);
	const DirectX::XMVECTOR impulseFriction = DirectX::XMVectorScale(
		DirectX::XMVectorScale(velTang, redecedMass), friction);
	//力学的摩擦を適用する
	bodyA->ApplyImpulse(ptOnA, DirectX::XMVectorScale(impulseFriction, -1.0f));
	bodyB->ApplyImpulse(ptOnB, DirectX::XMVectorScale(impulseFriction, -1.0f));


	//衝突している複数のオブジェクトをそれぞれのオブジェクトの外側へと動かす。
	if (0.0f == contact.timeOfImpact) {
		const float tA = bodyA->m_invMass / (bodyA->m_invMass + bodyB->m_invMass);
		const float tB = bodyB->m_invMass / (bodyA->m_invMass + bodyB->m_invMass);

		const DirectX::XMVECTOR  ds = DirectX::XMVectorSubtract(contact.ptOnA_WorldSpace, contact.ptOnA_WorldSpace);
		DirectX::XMVECTOR bodyAmpos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bodyA->m_position), DirectX::XMVectorScale(ds, tA));
		DirectX::XMVECTOR bodyBmpos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&bodyB->m_position), DirectX::XMVectorScale(ds, tB));
	}


}