#include "Scene.h"
#include "InterSect.h"
#include "Contact.h"
#include <search.h>
#include <stdlib.h>
void Scene::Update(const float dt_sec)
{
	std::unique_ptr<Body> body = std::make_unique<Body>();
	body->m_position = m_bodies[0].m_position;
	body->m_orientation = m_bodies[0].m_orientation;
	body->m_shape = m_bodies[0].m_shape;
	body->m_invMass = 1.0f;
	for (int i = -0; i < m_bodies.size(); i++)
	{
		//重力には衝撃が必要

		float mass = 1.0f / body->m_invMass;
		DirectX::XMFLOAT3 temp10 = DirectX::XMFLOAT3(0, 0, -10);
		DirectX::XMVECTOR temp10_xmvector = DirectX::XMLoadFloat3(&temp10);
		//重力加速度
		auto templinear = (DirectX::XMLoadFloat3(&m_bodies[i].m_linearVelocity));
		auto tempadd = DirectX::XMVectorAdd(
			templinear, temp10_xmvector);
		auto m_bodies_mLinearVelocity = DirectX::XMVectorScale(tempadd,dt_sec);
		DirectX::XMStoreFloat3(&m_bodies[i].m_angularVelocity, m_bodies_mLinearVelocity);
		//位置更新
		auto temppos =DirectX::XMLoadFloat3(&m_bodies[i].m_position);

		temppos = DirectX::XMVectorScale(DirectX::XMVectorAdd(temppos, templinear),dt_sec);
		DirectX::XMVECTOR multiply = DirectX::XMVectorSet(0, 0, 10,0.0f);
		DirectX::XMVECTOR impulseGravity = DirectX::XMVectorScale(multiply, mass * dt_sec);
		body->ApplyImpulseLinear(impulseGravity);
	}

	//ブロードスペース
	std::vector<collisionPair_t>collisionPairs;
	BroadPhase(m_bodies.data(), (int)m_bodies.size(), collisionPairs, dt_sec);

	for (int i = 0; i < m_bodies.size(); i++)
	{
		Body* body = &m_bodies[i];
		float mass = 1.0f / body->m_invMass;
		DirectX::XMVECTOR impulseGravity = DirectX::XMVectorScale(
			DirectX::XMVectorSet(0, 0, -10, 0), mass * dt_sec);
		body->ApplyImpulseLinear(impulseGravity);
	}

	//なろースペース
	int numContacts = 0;
	const int maxContacts = m_bodies.size() * m_bodies.size();
	contact_t* contacts = (contact_t*)alloca(sizeof(contact_t) * maxContacts);
	for (int i = 0; i < collisionPairs.size(); i++)
	{
		const collisionPair_t& pair = collisionPairs[i];
		Body* bodyA = &m_bodies[pair.a];
		Body* bodyB = &m_bodies[pair.b];

		//無限の重量をもつボディのペアは無視する
		if (0.0 == bodyA->m_invMass && 0.0f == bodyB->m_invMass)
		{
			continue;
		}

		contact_t contact;
		if (Intersect(bodyA, bodyB, dt_sec, contact))
		{
			contacts[numContacts] = contact;
			numContacts++;
		}
	}

	//最初の物から最後までの衝撃時間をソートする
	if (numContacts > 1)
	{
		std::qsort(contacts, numContacts, sizeof(contact_t), CompareContacts);
	}

	//弾道衝撃を適用する

	float accumlatedTime = 0.0f;
	for (int i = 0; i < numContacts; i++)
	{
		contact_t& contact = contacts[i];
		const float dt = contact.timeOfImpact - accumlatedTime;

		//位置更新
		for (int j = 0; j < m_bodies.size(); j++)
		{
			m_bodies[j].Update(dt);
		}

		ResolveContact(contact);
		accumlatedTime += dt;

		//このフレーム時間で残りの位置を更新する
		const float timeRemaining = dt_sec - accumlatedTime;
		if (timeRemaining > 0.0f)
		{
			for (int i = 0; i < m_bodies.size(); i++)
			{
				m_bodies[i].Update(timeRemaining);
			}
		}
	}
	//-----------------------------------------------------------------------------
	
	/*
	for (int i = 0; i < m_bodies.size(); i++)
	{
		for (int j = i + 1; m_bodies.size(); j++)
		{
			Body* bodyA = &m_bodies[i];
			Body* bodyB = &m_bodies[j];

			if (0.0f == bodyA->m_invMass && 0.0f == bodyB->m_invMass)
			{
				continue;
			}
			contact_t contact;
			if (Intersect(bodyA, bodyB, dt_sec, contact))
			{
				contacts[numContacts] = contact;
				numContacts++;
			}
		}
	}
	//最速から最遅までの衝突時間をソートする
	if (numContacts > 1)
	{
		std::qsort(contacts, numContacts, sizeof(contact_t), CompareContacts());
	}

	float accumlatedTime = 0.0f;
	for (int i = 0; i < numContacts; i++)
	{
		contact_t& contact = contacts[i];
		const float dt = contact.timeOfImpact - accumlatedTime;
		Body* bodyA = contact.bodyA;
		Body* bodyB = contact.bodyB;

			if (0.0f == bodyA->m_invMass && 0.0f == bodyB->m_invMass)
			{
				continue;
			}
			for (int j = 0; j < m_bodies.size(); j++) {
				m_bodies[j].Update(dt);
			}
				ResolveContact(contact);
				accumlatedTime += dt;
			}
	//残りのフレーム時間で位置を更新する
	const float timeRemaining = dt_sec - accumlatedTime;

	if(timeRemaining >0.0f)
	{
		for (int i = 0; i < m_bodies.size(); i++)
		{
			m_bodies[i].Update(timeRemaining);
		}
	}

	*/
}


void Scene::Initialize()
{
	Body body;
/*
	body.m_position = DirectX::XMFLOAT3(-3, 0, 3);
	body.m_orientation = DirectX::XMFLOAT4(0, 0, 0, 1);
	body.m_linearVelocity = DirectX::XMFLOAT3(1000, 0, 0);
	body.m_invMass = 1.0f;
	body.m_friction = 0.5f;
	body.m_shape = std::make_shared<Shape>(0.5f);
	m_bodies.push_back(body);
	*/
	//ダイナミックボディ
	for (int x = 0; x < 6; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			float radius = 0.5f;
			float xx = float(x - 1) * radius * 1.5f;
			float yy = float(y - 1) * radius * 1.5f;
			body.m_position = DirectX::XMFLOAT3(xx, yy, 10.0f);
			body.m_orientation = DirectX::XMFLOAT4(0, 0, 0, 1);
			body.m_linearVelocity = DirectX::XMFLOAT3(0,0,0);
			body.m_invMass = 1.0f;
			body.m_friction = 0.5f;
			body.m_shape = std::make_shared<Shape>(0.5f);
			m_bodies.push_back(body);
		}
		
	}

	//静的な床

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			float radius = 80.0f;
			float xx = float(x - 1) * radius * 0.25f;
			float yy = float(y - 1) * radius * 0.25f;
			body.m_position = DirectX::XMFLOAT3(xx,yy, -radius);
			body.m_orientation = DirectX::XMFLOAT4(0, 0, 0, 1);
			body.m_invMass = 0.0f;
			body.m_linearVelocity = DirectX::XMFLOAT3(0, 0, 0);
			body.m_elasticity = 1.0f;
			body.m_friction = 0.0f;
			body.m_shape = std::make_shared<Shape>(radius);
			m_bodies.push_back(body);
		}
	}
	//重量の影響下で落下しない地上にある球体を加える

	


	
}
