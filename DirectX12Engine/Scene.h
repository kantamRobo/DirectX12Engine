#pragma once
#include "Shape.h"
#include <vector>
#include <memory>
class Scene
{
	std::vector<Body>m_bodies;
	void Update(const float dt_sec);

		void Initialize();

};

