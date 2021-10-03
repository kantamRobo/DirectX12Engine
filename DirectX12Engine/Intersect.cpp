#include "InterSect.h"
#include <memory>
#include <cmath>

bool Intersect(const Body* bodyA, const Body* bodyB)
{
	const DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&bodyB->m_position)
		, DirectX::XMLoadFloat3(&bodyA->m_position));

	const std::shared_ptr<ShapeSphere> sphereA = std::make_shared<ShapeSphere>
		(bodyA->m_shape->GetCenterOfMass());
	
	const std::shared_ptr<ShapeSphere> sphereB = std::make_shared<ShapeSphere>
		(bodyB->m_shape->GetCenterOfMass());
	
		const float radiusAB = sphereA->m_radius + sphereB->m_radius;
		const float lengthSquare = 
			sqrt((bodyB->m_position.x - bodyA->m_position.x) * (bodyB->m_position.x - bodyA->m_position.x) +
                            (bodyB->m_position.y - bodyA->m_position.y) * (bodyB->m_position.y - bodyA->m_position.y) +
                            (bodyB->m_position.z - bodyA->m_position.z) * (bodyB->m_position.z - bodyA->m_position.z) );
		if (lengthSquare <= (radiusAB * radiusAB))
		{
			return true;
		}
		return false;

}

bool Intersect( Body* BodyA,  Body* BodyB, const float dt, contact_t& contact)
{
	contact.bodyA->m_invMass=BodyA->m_invMass;
	contact.bodyB->m_invMass = BodyB->m_invMass;

	if (BodyA->m_shape->GetType() == Shape::SHAPE_SPHERE && BodyB->m_shape->GetType() == Shape::SHAPE_SPHERE)
	{

		const DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&BodyB->m_position)
			, DirectX::XMLoadFloat3(&BodyA->m_position));

		const std::shared_ptr<ShapeSphere> sphereA = std::make_shared<ShapeSphere>
			(BodyA->m_shape->GetCenterOfMass());

		const std::shared_ptr<ShapeSphere> sphereB = std::make_shared<ShapeSphere>
			(BodyB->m_shape->GetCenterOfMass());

		DirectX::XMVECTOR posA = DirectX::XMLoadFloat3(& BodyA->m_position);
		DirectX::XMVECTOR posB = DirectX::XMLoadFloat3(&BodyB->m_position);

		DirectX::XMVECTOR velA = DirectX::XMLoadFloat3(&BodyA->m_linearVelocity);
		DirectX::XMVECTOR velB = DirectX::XMLoadFloat3(&BodyB->m_linearVelocity);

		if (SphereSphereDynamic(sphereA.get(), sphereB.get(), posA, posB,
			velA, velB, dt, contact.ptOnA_WorldSpace, contact.ptOnB_WorldSpace, contact.timeOfImpact))
		{
			BodyA->Update(contact.timeOfImpact);
			BodyB->Update(contact.timeOfImpact);
		
			//分離距離
			DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3
			(&BodyB->m_position), DirectX::XMLoadFloat3
			(&BodyA->m_position));
			float r = GetLength(BodyB, BodyA) - (sphereA->m_radius + sphereB->m_radius);
			contact.separationDistance = r;
			return true;
		}
		


		
	}


	
	return false;

}

bool RaySphere(const DirectX::XMVECTOR& rayStart, 
	const DirectX::XMVECTOR& rayDir, 
	const DirectX::XMVECTOR& sphereCenter, 
	const float sphereRadius, float& t1, float& t2)
{
	const DirectX::XMVECTOR m = DirectX::XMVectorSubtract(sphereCenter, rayStart);
	const float a = DirectX::XMVector3Dot(rayDir, rayDir).m128_f32[0];
	const float b = DirectX::XMVector3Dot(m, rayDir).m128_f32[0];
	const float c =
		DirectX::XMVector3Dot(m, m).m128_f32[0] - (sphereRadius * sphereRadius);
	const float delta = b * b - a * c;
	const float invA = 1.0f / a;

	if (delta < 0)
	{
		return false;
	}

	const float deltaRoot = sqrtf(delta);
	t1 = invA * (b - deltaRoot);
	t2 = invA * (b + deltaRoot);

	return true;
}

bool SphereSphereDynamic(const ShapeSphere* shapeA, const ShapeSphere* shapeB, 
	const DirectX::XMVECTOR& posA, const DirectX::XMVECTOR posB, 
	const DirectX::XMVECTOR velA, DirectX::XMVECTOR& velB, 
	const float dt,
	DirectX::XMVECTOR& ptOnA, DirectX::XMVECTOR& ptOnB, float& toi)
{
	const DirectX::XMVECTOR relativeVelocity = DirectX::XMVectorSubtract(velA, velB);

	const DirectX::XMVECTOR startPtA = posA;
	const DirectX::XMVECTOR endPtA = DirectX::XMVectorAdd(posA, DirectX::XMVectorScale
	(relativeVelocity, dt));
	 DirectX::XMVECTOR rayDir = DirectX::XMVectorSubtract(endPtA, startPtA);

	float t0 = 0;
	float t1 = 0;

	//レイが短すぎる。単に交差しているか確認する。
	DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(posB, posA);
	float radius = shapeA->m_radius + shapeB->m_radius + 0.001f;
	const float lengthSquare =
		GetLength(posB, posA);

	if (GetLength(posB, posA) > radius * radius)
	{
		return false;
	}
	else if (!RaySphere(posA, rayDir, posB, shapeA->m_radius + shapeB->m_radius, t0, t1))
	{
		return false;
	};

	t0 *= dt;
	t1 *= dt;

	//衝突が過去にしか起きてないなら、このフレームで衝突はない
	if (t1 < 0.0f)
	{
		return false;
	}
	//最も早い正の衝撃時間を取得
	toi = (t0 < 0.0f) ? 0.0f : t0;

	//最も速い衝突がかなりの未来なら、このフレームでも衝突はない
	if (toi > dt)
	{
		return false;
	}

	// 相対的な点における衝突点を取得し、trueを返す
	DirectX::XMVECTOR newPosA = DirectX::XMVectorAdd(posA, DirectX::XMVectorScale(velA, toi));
	DirectX::XMVECTOR newPosB = DirectX::XMVectorAdd(posB, DirectX::XMVectorScale(velB, toi));

	ptOnA = DirectX::XMVectorAdd(newPosA, DirectX::XMVectorScale(ab, shapeA->m_radius));

	ptOnB = DirectX::XMVectorAdd(newPosB, DirectX::XMVectorScale(ab, shapeB->m_radius));

	return true;
}


