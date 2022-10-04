#include "CharacterController.h"
using namespace physx;
void CharacterController::Move(float deltatime)
{
	PxVec3 targetKeyDisplacement(0);



	PxControllerCollisionFlags collisionFlags =
		c->move(targetKeyDisplacement, 0.0f, deltatime, physx::PxControllerFilters());
}
