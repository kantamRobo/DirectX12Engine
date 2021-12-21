#pragma once
#include <../packages/directxtk12_uwp.2021.11.8.1/include/Model.h>
#include "bullet3/shared/src/src/btBulletCollisionCommon.h"
#include "bullet3/shared/src/src/btBulletDynamicsCommon.h"
#include "bullet3/shared/src/src/BulletCollision/Gimpact/btBoxCollision.h"
#include "Transform.h"


struct CharacterBase
{
	std::unique_ptr<DirectX::Model> m_Model;
	btAABB charactercollision;
	Transform m_transform;
};
struct SkinnedCharacter:CharacterBase
{
	
	DirectX::ModelBone::TransformArray m_drawBones;
	std::vector<btRigidBody> Ragdolls;
	std::vector< btConeTwistConstraint> ragdollconstrait;
	btConeTwistConstraint lastconstrait;
	bool iskinematic;//ラグドールを有効にする？

	void SetRagdolls();
};

struct StaticCharacter:CharacterBase
{
	
};
