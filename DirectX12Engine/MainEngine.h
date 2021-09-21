#pragma once
#include <d3d12.h>
#include <memory>
class RenderEngine;
class MainEngine
{
public:
	void Tick();

	std::unique_ptr<RenderEngine> m_renderengine;
};

