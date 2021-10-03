#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bounds.h"
class Shape
{
public:
	enum shapeType_t {
		SHAPE_SPHERE,
	};
	virtual shapeType_t GetType()const = 0;
	virtual DirectX::XMFLOAT3 GetCenterOfMass()const { return m_centerOfMass; }
	virtual Bounds GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR orient)const = 0;
	virtual Bounds GetBounds()const = 0;
	DirectX::XMFLOAT3X3 InertiaTensor()const
	{
		DirectX::XMFLOAT3X3 tensor;

		tensor._11 = 2.0f * m_radius * m_radius / 5.0f;
		tensor._22 = 2.0f * m_radius * m_radius / 5.0f;
		tensor._33 = 2.0f * m_radius * m_radius / 5.0f;

		return tensor;
	}
	float m_radius;
	DirectX::XMFLOAT3 m_centerOfMass;
};

class ShapeSphere :public Shape
{
public:
	ShapeSphere(float radius) :m_radius(radius)
	{
		m_centerOfMass.x = 0;
		m_centerOfMass.y = 0;
		m_centerOfMass.z = 0;
	}
	shapeType_t GetType()const override { return SHAPE_SPHERE; }

	float m_radius;
	Bounds GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR orient)const override;
	 Bounds GetBounds()const override;
	

public:
	DirectX::XMFLOAT3X3 InertiaTensor()const
	{
		DirectX::XMFLOAT3X3 tensor;

		tensor._11 = 2.0f * m_radius * m_radius / 5.0f;
		tensor._22= 2.0f * m_radius * m_radius / 5.0f;
		tensor._33 = 2.0f * m_radius * m_radius / 5.0f;

		return tensor;
	}
};

Bounds ShapeSphere::GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR orient) const
{
	Bounds tmp;
	tmp.mins = DirectX::XMVectorAdd(DirectX::XMVectorSet(-m_radius, -m_radius, -m_radius,0.0f)
		, DirectX::XMVectorSet(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2], 0.0f));
	tmp.maxs = DirectX::XMVectorAdd(DirectX::XMVectorSet(m_radius, m_radius, m_radius, 0.0f)
		, DirectX::XMVectorSet(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2], 0.0f));
	return tmp;
}


Bounds ShapeSphere::GetBounds() const
{
	Bounds tmp;
	tmp.mins = DirectX::XMVectorSet(-m_radius, -m_radius, -m_radius, 0.0f);
		
	tmp.maxs = DirectX::XMVectorSet(m_radius, m_radius, m_radius, 0.0f);
		
	return tmp;
}



DirectX::XMVECTOR Body::GetCenterOfMassWorldSpace()const
{
	const DirectX::XMVECTOR centerOfMass = DirectX::XMLoadFloat3(&m_shape->GetCenterOfMass());
	
	const DirectX::XMVECTOR  pos =DirectX::XMVectorAdd( DirectX::XMLoadFloat3(&m_position),
		RotatePoint(XMLoadFloat4(&m_orientation),centerOfMass));
	return pos;
}

DirectX::XMVECTOR Body::GetCenterOfMassModelSpace()const
{
	const DirectX::XMVECTOR centerofMass =DirectX::XMLoadFloat3(&m_shape->GetCenterOfMass());
}

DirectX::XMVECTOR Body::WorldSpaceToBodySpace(const DirectX::XMFLOAT3& worldpt)const
{
	DirectX::XMVECTOR tmp = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&worldpt),
		GetCenterOfMassWorldSpace());
	DirectX::XMVECTOR inverseOrient = DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(&m_orientation));
	DirectX::XMVECTOR bodySpace = RotatePoint(inverseOrient, tmp);
      
}


DirectX::XMVECTOR Body::BodySpaceToWorldSpace(const DirectX::XMFLOAT3& pt) const
{

	DirectX::XMVECTOR worldSpace = DirectX::XMVectorAdd(
		GetCenterOfMassWorldSpace(),
		RotatePoint(DirectX::XMLoadFloat4(&m_orientation), DirectX::XMLoadFloat3(&pt)));
	return worldSpace;
}

void Body::ApplyImpulseLinear(const DirectX::XMVECTOR& impulse)
{
	if (0.0f == m_invMass)
	{
		return;
	}

	DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_linearVelocity), (DirectX::XMVectorScale
	(impulse, m_invMass)));
}
