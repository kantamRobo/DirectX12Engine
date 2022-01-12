#include "Character.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
void SkinnedCharacter::SetRagdolls()
{
	
}

void StaticCharacter::SetRigidbody()
{
	btVector3 pos = btVector3(-1, 0.5, 0);    //!< 中心座標
	btScalar mass = 0.03;                    //!< 質量
	btScalar restitution = 0.8;                //!< 反発係数

	// 衝突検出方法の選択(デフォルトを選択)
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);

	// ブロードフェーズ法の設定(Dynamic AABB tree method)
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	// 拘束(剛体間リンク)のソルバ設定
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Bulletのワールド作成
	auto g_pDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	g_pDynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	float g_frad = 1.0f;
	// 球体形状を設定
	btCollisionShape* sphere_shape = new btSphereShape(g_frad);

	// 球体の初期位置・姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));

	// 慣性モーメントの計算
	btVector3 inertia(0, 0, 0);
	sphere_shape->calculateLocalInertia(mass, inertia);

	// 剛体オブジェクト生成(質量，位置姿勢，形状，慣性モーメントを設定)
	auto g_pSphereBody = new btRigidBody(mass, motion_state, sphere_shape, inertia);

	// ワールドに剛体オブジェクトを追加
	g_pDynamicsWorld->addRigidBody(g_pSphereBody);


	btVector3 forward = btVector3(1, 0, 0);
	g_pSphereBody->applyCentralForce(inertia);
}
