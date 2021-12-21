#include "pch.h"
#include "Character.h"

void SkinnedCharacter::SetRagdolls()
{
	
	//boneにrigidbodyを入れる
	m_drawBones = DirectX::ModelBone::MakeArray(Model->bones.size());
	UINT bonesize = Model->bones.size();
	

	for (int i = 0; i <bonesize ; i++)
	{
	
		//btRigidBodyConstructionInfo(btScalar mass, btMotionState* motionState, btCollisionShape* collisionShape, const btVector3& localInertia = btVector3(0, 0, 0)) : m_mass(mass),
		float bonemass = 1.0f;
		std::unique_ptr<btMotionState> boneMotionState = std::make_unique<btMotionState>();
		boneMotionState->setWorldTransform(btTransform(
			btQuaternion(Model->boneMatrices[i].r->m128_f32[0], Model->boneMatrices[i].r->m128_f32[1],
				Model->boneMatrices[i].r->m128_f32[2], Model->boneMatrices[i].r->m128_f32[3]), btVector3(Model->meshes[i]->boundingSphere.Center.x
					, Model->meshes[i]->boundingSphere.Center.y,
					Model->meshes[i]->boundingSphere.Center.z)));
			//btquaternionの設定を直す（どうすればいいのかわからないのでとりあえずボーン行列を入れただけ。ボーン行列の用途を調べる)
		std::unique_ptr<btCollisionShape> boneCollsion = std::make_unique<btSphereShape>(1.0f);
		btRigidBody bonerigidbody = btRigidBody(bonemass, boneMotionState.get(), boneCollsion.get());
		Ragdolls[i] = bonerigidbody;
	}
	
	for (int i = 0; i < Ragdolls.size(); i++)
	{
		//複数
		if (i != Ragdolls.size()) {
			btConeTwistConstraint temps(Ragdolls[i], Ragdolls[i + 1],Ragdolls[i].getCenterOfMassTransform(),Ragdolls[i+1].getCenterOfMassTransform());
			ragdollconstrait.push_back(temps);
		}
		else if (i == Ragdolls.size())
		{

			//単体
			btConeTwistConstraint last (Ragdolls[i],Ragdolls[i].getCenterOfMassTransform());
			ragdollconstrait.push_back(last);
			break;
		}
	}
}
