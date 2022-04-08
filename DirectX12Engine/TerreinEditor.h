#pragma once
#include "ImguiCore.h"
#include <vector>
#include <memory>
class TerreinEditor
{
public:
	TerreinEditor();
	std::vector<ImVec2> Nodes = {};
	unsigned int nodenum;
	unsigned int offset;
	ImDrawList* drawListR;
	float slope;
	
};

