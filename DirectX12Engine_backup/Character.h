#pragma once
#include "pch.h"
#include "Transform.h"
struct CharacterBase
{
	std::unique_ptr<DirectX::Model> m_Model;
	//btAABB charactercollision;
	Transform m_transform;
};
struct SkinnedCharacter :CharacterBase
{

	DirectX::ModelBone::TransformArray m_drawBones;
	//std::vector<btRigidBody> Ragdolls;
	//std::vector< btConeTwistConstraint> ragdollconstrait;
	//btConeTwistConstraint lastconstrait;
	bool iskinematic;//ラグドールを有効にする？

	void SetRagdolls();
};

struct StaticCharacter :CharacterBase
{


	void SetRigidbody();
};
