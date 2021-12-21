#include "BULLET3TEST.h"


btDynamicsWorld* g_pDynamicsWorld = 0;	//!< Bulletワールド
btRigidBody* g_pSphereBody = 0;		//!< 球体
void BULLET3TEST::InitBullet(void)
{
	btVector3 pos = btVector3(-1,0.5,0);
	btScalar mass =0.03;
	btScalar restitution = 0.0f;

	//衝突検出方法の選択（デフォルトを選択）
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);

	//ブロードフェーズ法の設定(Dynamic AABB　tree method)
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();

	//拘束(剛体間リンク）のソルバ設定
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	// Bulletのワールド作成
    g_pDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
    g_pDynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	btSphereShape g_fRad = btSphereShape(1.0f);
    // 球体形状を設定
    btCollisionShape *sphere_shape = new btSphereShape(g_fRad);

    // 球体の初期位置・姿勢
    btQuaternion qrot(0, 0, 0, 1);
    btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));

    // 慣性モーメントの計算
    btVector3 inertia(0, 0, 0);
    sphere_shape->calculateLocalInertia(mass, inertia);

    // 剛体オブジェクト生成(質量，位置姿勢，形状，慣性モーメントを設定)
    g_pSphereBody = new btRigidBody(mass, motion_state, sphere_shape, inertia);

    // ワールドに剛体オブジェクトを追加
    g_pDynamicsWorld->addRigidBody(g_pSphereBody);
}


void BULLET3TEST::CleanBullet(void)
{
	// オブジェクト破棄
	delete g_pSphereBody->getMotionState();
	g_pDynamicsWorld->removeRigidBody(g_pSphereBody);
	delete g_pSphereBody;

	// ワールド破棄
	delete g_pDynamicsWorld->getBroadphase();
	delete g_pDynamicsWorld;
}


void BULLET3TEST::Timer(int value)
{
	// 球体の放物線運動を計算
	if (g_bIdle) {
		if (g_iStep == 0) {
			g_pSphereBody->applyCentralImpulse(g_v3Frc * DT);
		}

		// bulletのステップを進める
		if (g_pDynamicsWorld) {
			g_pDynamicsWorld->stepSimulation(DT, 1);
		}

		g_iStep++;
		
	}

}