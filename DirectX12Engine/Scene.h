#pragma once
#include "Shape.h"
#include <vector>
#include <memory>
#include "Body.h"
class Scene
{
	std::vector<Body>m_bodies;
	void Update(const float dt_sec);

		void Initialize();

};

