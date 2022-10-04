#pragma once
#include <PxScene.h>
#include <characterkinematic/PxCapsuleController.h>
#include <characterkinematic/PxControllerManager.h>
class CharacterController
{
	void Move(float deltatime);
	physx::PxControllerManager* manager;
	physx::PxController* c;
};

