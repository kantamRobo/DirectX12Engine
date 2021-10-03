#pragma once
#include "Shape.h"
#include "Contact.h"
#include "Body.h"
bool Intersect(const Body* bodyA, const Body* bodyB);


bool Intersect(Body* BodyA, Body* BodyB, const float dt, contact_t& contact);
bool RaySphere(const DirectX::XMVECTOR& rayStart, const DirectX::XMVECTOR& rayDir, const DirectX::XMVECTOR& sphereCenter, const float sphereRadius, float& t1, float& t2);
bool SphereSphereDynamic(const ShapeSphere* sphereA, const ShapeSphere* shapeB, const DirectX::XMVECTOR& posA,
	const DirectX::XMVECTOR posB, const DirectX::XMVECTOR velA, DirectX::XMVECTOR& velB, const float dt,
	DirectX::XMVECTOR& ptOnA, DirectX::XMVECTOR& ptOnB, float& toi);

int CompareContacts(const void* p1, const void* p2)
{
	contact_t a = *(contact_t*)p1;
	contact_t b = *(contact_t*)p2;

	if (a.timeOfImpact < b.timeOfImpact)
	{
		return -1;
	}
	if (a.timeOfImpact == b.timeOfImpact)
	{
		return 0;
	}

	return 1;
}

float GetLength(DirectX::XMVECTOR posB, DirectX::XMVECTOR posA)
{
	const float lengthSquare =
		sqrt((posB.m128_f32[0] - posA.m128_f32[0]) * (posB.m128_f32[0] - posA.m128_f32[0]) +
			(posB.m128_f32[1] - posA.m128_f32[1]) * (posB.m128_f32[1] - posA.m128_f32[1]) +
			(posB.m128_f32[2] - posA.m128_f32[2]) * (posB.m128_f32[2] - posA.m128_f32[2]));

	return lengthSquare;
}
float GetLength(const Body* BodyB,const Body* BodyA)
{
	float temp =sqrt((BodyB->m_position.x - BodyA->m_position.x) * (BodyB->m_position.x - bodyA->m_position.x) +
		(BodyB->m_position.y - bodyA->m_position.y) * (BodyB->m_position.y - bodyA->m_position.y) +
		(BodyB->m_position.z - bodyA->m_position.z) * (BodyB->m_position.z - bodyA->m_position.z));
	
	return temp;
}