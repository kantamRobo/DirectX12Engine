#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Bounds.h"
#include <DirectXMath.h>
#include <memory>
#include <malloc.h>

DirectX::XMVECTOR RotatePoint(const DirectX::XMVECTOR lhs, const DirectX::XMMATRIX rhs)const
{
	DirectX::XMVECTOR vector(DirectX::XMVectorSet(rhs.r->m128_f32[0], rhs.r->m128_f32[1], rhs.r->m128_f32[2], 0.0f));
	DirectX::XMVECTOR final = DirectX::XMQuaternionInverse(
		DirectX::XMVectorMultiply(lhs, vector));





	return final;
}


DirectX::XMVECTOR RotatePoint(const DirectX::XMVECTOR lhs, const DirectX::XMVECTOR rhs)const
{
	DirectX::XMVECTOR val(lhs);
	DirectX::XMVECTOR vector(DirectX::XMVectorSet(rhs.m128_f32[0], rhs.m128_f32[1], rhs.m128_f32[2], 0.0f));




	DirectX::XMVECTOR final = DirectX::XMQuaternionMultiply((
		DirectX::XMQuaternionMultiply(val, vector)),
		DirectX::XMQuaternionInverse(val));
	return final;
}

class Body
{
public:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_orientation;
	DirectX::XMFLOAT3 m_linearVelocity;
	DirectX::XMFLOAT3 m_angularVelocity;
	float m_elasticity;
	float m_friction;
	std::shared_ptr<Shape> m_shape;
	float m_invMass;

	DirectX::XMVECTOR  GetCenterOfMassWorldSpace()const;
	DirectX::XMVECTOR  GetCenterOfMassModelSpace()const;

	DirectX::XMVECTOR WorldSpaceToBodySpace(const DirectX::XMFLOAT3& pt)const;
	DirectX::XMVECTOR  BodySpaceToWorldSpace(const DirectX::XMFLOAT3& pt)const;
	void ApplyImpulse(const DirectX::XMVECTOR& impulsePoint, const DirectX::XMVECTOR impulse)
	{
		if (0.0f == m_invMass)
		{
			return;
		}
		ApplyImpulseLinear(impulse);

		DirectX::XMVECTOR position = GetCenterOfMassWorldSpace();
		DirectX::XMVECTOR r = DirectX::XMVectorSubtract(impulsePoint, position);
		DirectX::XMVECTOR dL = DirectX::XMVector3Cross(r, impulse);
		ApplyImpulseAngular(dL);
	}
	void ApplyImpulseLinear(const DirectX::XMVECTOR& impulse);
	void ApplyImpulseAngular(const DirectX::XMVECTOR& impulse)
	{
		if (0.0f == m_invMass)
		{
			return;
		}

		DirectX::XMVECTOR temp_angularvelocity = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_angularVelocity), DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_angularVelocity)
			, DirectX::XMLoadFloat3x3(&GetInverseInertiaTensorWorldSpace)));

		const float maxAngularSpeed = 30.0f;
		DirectX::XMStoreFloat3(&m_angularVelocity, temp_angularvelocity);
		const float lengthSquare =
			sqrt((m_angularVelocity.x * m_angularVelocity.x)) +
			(m_angularVelocity.y * (m_angularVelocity.y)) +
			(m_angularVelocity.z * (m_angularVelocity.z));

		if (lengthSquare > maxAngularSpeed * maxAngularSpeed)
		{
			DirectX::XMVECTOR temp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_angularVelocity));
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_angularVelocity)
				, maxAngularSpeed);

			DirectX::XMStoreFloat3(&m_angularVelocity, temp);
		}


	}




	DirectX::XMMATRIX GetInverseInertialTensorBodySpace()const
	{
		DirectX::XMFLOAT3X3 inertiaTensor = m_shape->InertiaTensor();
		DirectX::XMMATRIX invInertialTensor = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat3x3(&inertiaTensor) * m_invMass);
		return invInertialTensor;
	}

	DirectX::XMFLOAT3X3 GetInverseInertiaTensorWorldSpace()const
	{
		DirectX::XMFLOAT3X3 inetitalTensor = m_shape->InertiaTensor();
		DirectX::XMMATRIX invIntertialTensor = DirectX::XMMatrixMultiply(
			DirectX::XMMatrixTranspose(DirectX::XMLoadFloat3x3(&inetitalTensor)),
			DirectX::XMMatrixScaling(
				m_invMass, m_invMass, m_invMass)
		);
	}
	DirectX::XMVECTOR RotatePoint(const DirectX::XMVECTOR lhs, const DirectX::XMVECTOR rhs)const
	{
		DirectX::XMVECTOR val(lhs);
		DirectX::XMVECTOR vector(DirectX::XMVectorSet(rhs.m128_f32[0], rhs.m128_f32[1], rhs.m128_f32[2], 0.0f));




		DirectX::XMVECTOR final = DirectX::XMQuaternionMultiply((
			DirectX::XMQuaternionMultiply(val, vector)),
			DirectX::XMQuaternionInverse(val));
		return final;
	}

	DirectX::XMVECTOR RotatePoint(const DirectX::XMVECTOR lhs, const DirectX::XMMATRIX rhs)const
	{
		DirectX::XMVECTOR vector(DirectX::XMVectorSet(rhs.r->m128_f32[0], rhs.r->m128_f32[1], rhs.r->m128_f32[2], 0.0f));
		DirectX::XMVECTOR final = DirectX::XMQuaternionInverse(
			DirectX::XMVectorMultiply(lhs, vector));





		return final;
	}

	void Update(float dt_sec);
};

class Shape
{
public:
	enum shapeType_t {
		SHAPE_SPHERE,
		SHAPE_BOX,
		SHAPE_CONVEX,
	};
	virtual shapeType_t GetType()const = 0;
	virtual DirectX::XMFLOAT3 GetCenterOfMass()const { return m_centerOfMass; }
	virtual Bounds GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR orient)const = 0;
	virtual Bounds GetBounds()const = 0;
	virtual DirectX::XMVECTOR Support(const DirectX::XMVECTOR& dir, const DirectX::XMVECTOR& pos,
		const DirectX::XMVECTOR& orient, const float bias)const = 0;
	DirectX::XMFLOAT3X3  InertiaTensor()const;
	float m_radius;
	DirectX::XMFLOAT3 m_centerOfMass;
};




DirectX::XMFLOAT3X3  Shape::InertiaTensor()const
{
	DirectX::XMFLOAT3X3 tensor;

	tensor._11 = 2.0f * m_radius * m_radius / 5.0f;
	tensor._22 = 2.0f * m_radius * m_radius / 5.0f;
	tensor._33 = 2.0f * m_radius * m_radius / 5.0f;

	return tensor;
}
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

	DirectX::XMVECTOR Support(const DirectX::XMVECTOR& dir, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient, const float bias) const;
	shapeType_t GetType()const override { return Shape::SHAPE_SPHERE; }
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

class ShapeBox :public Shape {
public:
	explicit ShapeBox(const DirectX::XMVECTOR* pts, const int num)
	{

	}
	void Build(const DirectX::XMVECTOR pts, const int num);

	DirectX::XMVECTOR Support(const DirectX::XMVECTOR& dir, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient, const float bias);

	Bounds GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient);
	

	Bounds GetBounds(const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient) const;
	Bounds GetBounds()const;
	float FastestLinearSpeed(const DirectX::XMVECTOR angularVelocity,
		const DirectX::XMVECTOR& orient);
	float FastestLinearSpeed(const DirectX::XMVECTOR angularVelocity, const DirectX::XMVECTOR& dir);
	DirectX::XMFLOAT3X3  InertiaTensor();
	shapeType_t GetType()const { return Shape::SHAPE_BOX; }

public:
	std::vector<DirectX::XMVECTOR> m_points;
	Bounds m_bounds;
};

void ShapeBox::Build(const DirectX::XMVECTOR pts, const int num)
{

	for (int i = 0; i < num; i++)
	{
		m_bounds.Expand(pts[i]);
	}
	m_points.clear();
	m_points.push_back(DirectX::XMVectorSet(m_bounds.mins.m128_f32[0], 
		m_bounds.mins.m128_f32[1], m_bounds.mins.m128_f32[2], 0));
	m_points.push_back(DirectX::XMVectorSet(m_bounds.maxs.m128_f32[0],
		m_bounds.maxs.m128_f32[1], m_bounds.maxs.m128_f32[2], 0));
	m_points.push_back(DirectX::XMVectorSet(m_bounds.mins.m128_f32[0],
		m_bounds.mins.m128_f32[1], m_bounds.mins.m128_f32[2], 0));
	m_points.push_back(DirectX::XMVectorSet(m_bounds.mins.m128_f32[0],
		m_bounds.mins.m128_f32[1], m_bounds.mins.m128_f32[2], 0));
	DirectX::XMVECTOR CenterOfmass=DirectX::XMLoadFloat3(&m_centerOfMass);
	CenterOfmass = DirectX::XMVectorScale(DirectX::XMVectorAdd(m_bounds.maxs , m_bounds.mins)
		, 0.5f);
	DirectX::XMStoreFloat3(&m_centerOfMass, CenterOfmass);
}

DirectX::XMVECTOR ShapeBox::Support(const DirectX::XMVECTOR& dir, const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient, const float bias)
{

}


Bounds ShapeBox::GetBounds(const DirectX::XMVECTOR& pos,const DirectX::XMVECTOR& orient) const
{
	DirectX::XMVECTOR corners[8];
	corners[0] = DirectX::XMVectorSet(m_bounds.mins.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.mins.m128_f32[2], m_bounds.mins.m128_f32[3]);
	corners[1] = DirectX::XMVectorSet(
		m_bounds.mins.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);
	corners[2] = DirectX::XMVectorSet(
		m_bounds.mins.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.mins.m128_f32[2], 0);
	corners[3] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.mins.m128_f32[2], 0);
	
	
	corners[4] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);
	corners[5] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.mins.m128_f32[2], 0);
	corners[6] = DirectX::XMVectorSet(
		m_bounds.maxs.m128_f32[0],
		m_bounds.mins.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);
	corners[7] = DirectX::XMVectorSet(
		m_bounds.mins.m128_f32[0],
		m_bounds.maxs.m128_f32[1],
		m_bounds.maxs.m128_f32[2], 0);

	Bounds bounds;
	for (int i = 0; i < 8; i++)
	{
		corners[i] = RotatePoint(orient, DirectX::XMVectorAdd(corners[i], pos));
		bounds.Expand(corners[i]);
	}

	return bounds;
}

Bounds ShapeBox::GetBounds()const
{
	return m_bounds;
}

float ShapeBox::FastestLinearSpeed(const DirectX::XMVECTOR angularVelocity, 
	const DirectX::XMVECTOR& dir)
{
	float maxSpeed = 0.0f;
	for (int i = 0; i < m_points.size(); i++)
	{
		DirectX::XMVECTOR r = DirectX::XMVectorSubtract(m_points[i],DirectX::XMLoadFloat3
			(&m_centerOfMass));
		DirectX::XMVECTOR linearVelocity = DirectX::XMVector3Cross(angularVelocity, r);

		float speed = DirectX::XMVector3Dot(dir, linearVelocity).m128_f32[0];
		maxSpeed = speed;
	}
	return maxSpeed;
}

DirectX::XMFLOAT3X3 ShapeBox::InertiaTensor()
{
	const float dx = m_bounds.maxs.m128_f32[0] - m_bounds.mins.m128_f32[0];
	const float dy = m_bounds.maxs.m128_f32[1] - m_bounds.mins.m128_f32[1];
	float dz = m_bounds.maxs.m128_f32[2] - m_bounds.mins.m128_f32[2];

	DirectX::XMFLOAT3X3 tensor;

	tensor._11 = (dy * dy + dz * dz) / 12.0f;
	tensor._22 = (dx * dx + dz * dz) / 12.0f;
	tensor._33 = (dx * dx + dy * dy) / 12.0f;

	DirectX::XMVECTOR cm;

	cm.m128_f32[0] =  m_bounds.maxs.m128_f32[0] + m_bounds.mins.m128_f32[0] * 0.5f;
	cm.m128_f32[1] = m_bounds.maxs.m128_f32[1] + m_bounds.mins.m128_f32[1] * 0.5f;
	cm.m128_f32[2] = m_bounds.maxs.m128_f32[2] + m_bounds.mins.m128_f32[2] * 0.5f;
	

	const DirectX::XMVECTOR R = DirectX::XMVectorSubtract( DirectX::XMVectorSet(0, 0, 0,0), cm);
	const float R2 = DirectX::XMVector3Length(R).m128_f32[0];

	DirectX::XMFLOAT3X3 patTensor;
	patTensor._11 = R2 - R.m128_f32[0]* R.m128_f32[0];
	patTensor._12 = R.m128_f32[0] * R.m128_f32[1];
	patTensor._13 = R.m128_f32[0] * R.m128_f32[2];

	patTensor._21 = R.m128_f32[1] * R.m128_f32[0];
	patTensor._22 = R2 - R.m128_f32[1] * R.m128_f32[1];
	patTensor._23 = R.m128_f32[1] * R.m128_f32[2];

	patTensor._31 = R.m128_f32[2] * R.m128_f32[1];
	patTensor._32 = R2 - R.m128_f32[1] * R.m128_f32[1];
	patTensor._33 = R.m128_f32[2] - R.m128_f32[2] * R.m128_f32[2];
	
	DirectX::XMMATRIX temppatTensor = DirectX::XMLoadFloat3x3(&patTensor);
	tensor._11 = tensor._11 + patTensor._11;
	tensor._12 = tensor._12 + patTensor._12;
	tensor._13 = tensor._13 + patTensor._13;
	
	tensor._21 = tensor._21 + patTensor._21;
	tensor._22 = tensor._22 + patTensor._22;
	tensor._23 = tensor._23 + patTensor._23;
	
	tensor._31 = tensor._31 + patTensor._31;
	tensor._32 = tensor._32 + patTensor._32;
	tensor._33 = tensor._33 + patTensor._33;
 	
	return tensor;
}


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



DirectX::XMVECTOR ShapeSphere::Support(const DirectX::XMVECTOR& dir,const DirectX::XMVECTOR& pos, const DirectX::XMVECTOR& orient, const float bias) const
{
	return(DirectX::XMVectorAdd(pos , DirectX::XMVectorScale(pos,m_radius + bias)));
}


void Body::Update(const float dt_sec)
{
	auto m_positionvec = DirectX::XMLoadFloat3(&m_position);
	m_positionvec = DirectX::XMVectorScale(DirectX::XMVectorAdd(m_positionvec, DirectX::XMLoadFloat3
	(&m_linearVelocity)), dt_sec);
	DirectX::XMStoreFloat3(&m_position, m_positionvec);

	auto positionCM = GetCenterOfMassWorldSpace();
	auto cmTpPos = DirectX::XMVectorSubtract(m_positionvec, positionCM);

	DirectX::XMFLOAT3X3 orientationmat;

	orientationmat._11 = 1;
	orientationmat._22 = 1;
	orientationmat._33 = 1;
	DirectX::XMVECTOR vec1 = DirectX::XMVectorSet(orientationmat._11, orientationmat._12,
		orientationmat._13, 0.0f);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSet(orientationmat._21, orientationmat._22,
		orientationmat._23, 0.0f);
	DirectX::XMVECTOR vec3 = DirectX::XMVectorSet(orientationmat._31, orientationmat._32,
		orientationmat._33, 0.0f);


	vec1 = RotatePoint(vec1, DirectX::XMLoadFloat3x3(&orientationmat));
	vec2 = RotatePoint(vec2, DirectX::XMLoadFloat3x3(&orientationmat));
	vec3 = RotatePoint(vec3, DirectX::XMLoadFloat3x3(&orientationmat));

	orientationmat._11 = vec1.m128_f32[0];
	orientationmat._12 = vec1.m128_f32[1];
	orientationmat._13 = vec1.m128_f32[2];
	orientationmat._21 = vec2.m128_f32[0];
	orientationmat._22 = vec2.m128_f32[1];
	orientationmat._23 = vec2.m128_f32[2];
	orientationmat._31 = vec3.m128_f32[0];
	orientationmat._32 = vec3.m128_f32[1];
	orientationmat._33 = vec3.m128_f32[2];


	auto inertiaTensor = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat3x3
	(&m_shape->InertiaTensor()), DirectX::XMMatrixTranspose(DirectX::XMLoadFloat3x3(&orientationmat)));

	auto cross = DirectX::XMVector3Cross(
		DirectX::XMLoadFloat3(&m_angularVelocity),
		DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_angularVelocity), inertiaTensor));
	;

	auto alpha = DirectX::XMVector3Transform(cross, DirectX::XMMatrixTranspose(inertiaTensor));



	auto angularvelocityvector = DirectX::XMLoadFloat3(&m_angularVelocity);

	angularvelocityvector = DirectX::XMVectorAdd(angularvelocityvector, DirectX::XMVectorScale(alpha, dt_sec));



	//オリエンテーションの更新

	auto dAngle = DirectX::XMVectorScale(angularvelocityvector, dt_sec);
	auto danglemag = sqrt(dAngle.m128_f32[0] * dAngle.m128_f32[0] +
		dAngle.m128_f32[1] * dAngle.m128_f32[1] +
		dAngle.m128_f32[2] * dAngle.m128_f32[2]);
	auto tempdq =
		DirectX::XMFLOAT4(dAngle.m128_f32[0], dAngle.m128_f32[1], dAngle.m128_f32[2], ((danglemag * 3.14) / 180));
	auto dq = DirectX::XMLoadFloat4(&tempdq);

	DirectX::XMStoreFloat3(&m_angularVelocity, angularvelocityvector);
	auto m_orientationVec = DirectX::XMLoadFloat4(&m_orientation);

	m_orientationVec = DirectX::XMVectorMultiply(dq, m_orientationVec);
	DirectX::XMVector4Normalize(m_orientationVec);

	DirectX::XMStoreFloat4(&m_orientation, m_orientationVec);

	m_positionvec = DirectX::XMVectorAdd(RotatePoint(dq, cmTpPos), positionCM);

	DirectX::XMStoreFloat3(&m_position, m_positionvec);


}

struct psuedoBody_t
{
	int id;
	float value;
	bool ismin;
};

int CompareSAP(const void* a, const void* b)
{
	const psuedoBody_t* ea = (const psuedoBody_t*)a;
	const psuedoBody_t* eb = (const psuedoBody_t*)b;

	if (ea->value < eb->value)
	{
		return -1;
	}
	return 1;
}

void SortBodiesBounds(const Body* bodies, const int num, psuedoBody_t* sortedArray,
	const float dt_sec)
{
	DirectX::XMVECTOR axis = DirectX::XMVectorSet(1, 1, 1, 0);
	DirectX::XMVector3Normalize(axis);

	for (int i = 0; i < num; i++)
	{
		const Body& body = bodies[i];
		Bounds bounds = body.m_shape->GetBounds(DirectX::XMLoadFloat3(&body.m_position)
			, DirectX::XMLoadFloat4(&body.m_orientation));
		//線形速度でバウンズを拡張する
		bounds.Expand(DirectX::XMVectorScale(DirectX::XMVectorAdd(bounds.mins,
			DirectX::XMLoadFloat3(&body.m_linearVelocity)), dt_sec));
		bounds.Expand(DirectX::XMVectorScale(DirectX::XMVectorAdd(bounds.maxs,
			DirectX::XMLoadFloat3(&body.m_linearVelocity)), dt_sec));

		const float epsilon = 0.01f;
		bounds.Expand(DirectX::XMVectorScale(DirectX::XMVectorAdd(bounds.mins,
			DirectX::XMVectorSet(-1, -1, -1, 0)), epsilon));

		bounds.Expand(DirectX::XMVectorScale(DirectX::XMVectorAdd(bounds.maxs,
			DirectX::XMVectorSet(1, 1, 1, 0)), epsilon));

		sortedArray[i * 2 + 0].id = i;
		sortedArray[i * 2 + 0].value = DirectX::XMVector3Dot(axis, bounds.mins).m128_f32[0];
		sortedArray[i * 2 + 0].ismin = true;

		sortedArray[i * 2 + 0].id = i;
		sortedArray[i * 2 + 0].value = DirectX::XMVector3Dot(axis, bounds.maxs).m128_f32[0];
		sortedArray[i * 2 + 0].ismin = false;
	}
	std::qsort(sortedArray, num * 2, sizeof(psuedoBody_t), CompareSAP);
}

struct collisionPair_t
{
	int a;
	int b;

	bool operator == (const collisionPair_t& rhs)const {
		return (((a == rhs.a) && (b == rhs.b)) || ((a == rhs.b) && (b == rhs.a)));
	}
	bool operator !=(const collisionPair_t& rhs)const {
		return !(*this == rhs);
	}
};

void BuildPairs(std::vector<collisionPair_t>& collisionPairs, const psuedoBody_t*
	sortedBodies, const int num)
{
	collisionPairs.clear();

	//ボディはソートしたので、コリジョンのペアを作る
	for (int i = 0; i < num * 2; i++)
	{
		const psuedoBody_t& a = sortedBodies[i];
		if (!a.ismin)
		{
			continue;
		}

		collisionPair_t pair;
		pair.a = a.id;
		for (int j = i + 1; j < num * 2; j++)
		{
			const psuedoBody_t& b = sortedBodies[i];
			//もし要素の終わりに当たったら、aとのペア作りは完了している
			if (b.id == a.id)
			{
				break;
			}

			if (!b.ismin)
			{
				continue;
			}
			pair.b = b.id;
			collisionPairs.push_back(pair);
		}
	}
}

void SweepAndPrune1D(const Body* bodies, const int num, std::vector<collisionPair_t>&
	finalPairs, const float dt_sec)
{
	psuedoBody_t* sortedBodies = (psuedoBody_t*)alloca(sizeof(psuedoBody_t) * num * 2);

		SortBodiesBounds(bodies, num, sortedBodies, dt_sec);
		BuildPairs(finalPairs, sortedBodies, num);
}

void BroadPhase(const Body* bodies, const int num, std::vector < collisionPair_t>& finalPairs,
	const float dt_sec)
{
	finalPairs.clear();

	SweepAndPrune1D(bodies, num, finalPairs, dt_sec);
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
