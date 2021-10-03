#include <DirectXMath.h>
#include <memory>
#include <malloc.h>
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
		DirectX::XMVECTOR r = DirectX::XMVectorSubtract( impulsePoint,  position);
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
			sqrt((m_angularVelocity.x  * m_angularVelocity.x ) )+
				(m_angularVelocity.y  * (m_angularVelocity.y  ))+
				(m_angularVelocity.z  * (m_angularVelocity.z ));

		if (lengthSquare > maxAngularSpeed * maxAngularSpeed)
		{
			DirectX::XMVECTOR temp=  DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_angularVelocity));
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&m_angularVelocity)
				, maxAngularSpeed);
		
			DirectX::XMStoreFloat3(&m_angularVelocity, temp);
		}

		
	}

	


	DirectX::XMFLOAT3X3 GetInverseInertialTensorBodySpace()const
	{
		DirectX::XMFLOAT3X3 inertiaTensor = m_shape->InertiaTensor();
		DirectX::XMFLOAT3X3 invInertialTensor = inertiaTensor.Inverse() * m_invMass;
		return invInertialTensor;
	}

	DirectX::XMFLOAT3X3 GetInverseInertiaTensorWorldSpace()const
	{
		DirectX::XMFLOAT3X3 inetitalTensor = m_shape->InertialTensor();
		DirectX::XMMATRIX invIntertialTensor = DirectX::XMMatrixMultiply(DirectX::XMMatrixInverse(DirectX::XMLoadFloat3x3(&inetitalTensor),
			DirectX::XMMatrixScaling(
			m_invMass, m_invMass, m_invMass));
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
			DirectX::XMVectorMultiply(lhs,vector ));




		
		return final;
	}

	void Update(float dt_sec);
};

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
		orientationmat._13,0.0f);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSet(orientationmat._21, orientationmat._22,
		orientationmat._23, 0.0f);
	DirectX::XMVECTOR vec3 = DirectX::XMVectorSet(orientationmat._31, orientationmat._32,
		orientationmat._33, 0.0f);


	vec1= RotatePoint(vec1,DirectX::XMLoadFloat3x3(&orientationmat));
	vec2 = RotatePoint(vec2,DirectX::XMLoadFloat3x3(&orientationmat) );
	vec3 = RotatePoint(vec3,DirectX::XMLoadFloat3x3(&orientationmat) );

	orientationmat._11 = vec1.m128_f32[0];
	orientationmat._12 = vec1.m128_f32[1];
	orientationmat._13 = vec1.m128_f32[2];
	orientationmat._21 = vec2.m128_f32[0];
	orientationmat._22 = vec2.m128_f32[1];
	orientationmat._23 = vec2.m128_f32[2];
	orientationmat._31= vec3.m128_f32[0];
	orientationmat._32 = vec3.m128_f32[1];
	orientationmat._33 = vec3.m128_f32[2];


	auto inertiaTensor = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat3x3
	(&m_shape->InertiaTensor()), DirectX::XMMatrixTranspose(DirectX::XMLoadFloat3x3(&orientationmat)));
	
	auto cross = DirectX::XMVector3Cross(
		DirectX::XMLoadFloat3(&m_angularVelocity),
		DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_angularVelocity),inertiaTensor));
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
		DirectX::XMFLOAT4(dAngle.m128_f32[0], dAngle.m128_f32[1], dAngle.m128_f32[2], ((danglemag *3.14)/180));
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
		sortedArray[i * 2 + 0].value = DirectX::XMVector3Dot(axis,bounds.mins).m128_f32[0];
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
		if(!a.ismin)
		{continue;
		}

		collisionPair_t pair;
		pair.a = a.id;
		for (int j = i + 1; j < num * 2; j++)
		{
			const psuedoBody_t& b = sortedBodies[i];
			//もし要素の終わりに当たったら、aとのペア作りは完了している
			if(b.id ==a.id)
			{break;
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
	psuedoBody_t* sortedBodies = (psuedoBody_t *) alloca(sizeof(psuedoBody_t) * num * 2));

	SortBodiesBounds(bodies, num, sortedBodies, dt_sec);
	BuildPairs(finalPairs, sortedBodies, num);
}

void BroadPhase(const Body* bodies, const int num, std::vector < collisionPair_t> &finalPairs,
	const float dt_sec)
{
	finalPairs.clear();

	SweepAndPrune1D(bodies, num, finalPairs, dt_sec);
}