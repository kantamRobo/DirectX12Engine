#pragma once
#include "Vertex.h"
#include <vector>
#include <windows.h>
class Mesh {
public:
	Mesh() {};
	Mesh(std::vector<Vertex> vertices, std::vector<UINT> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}
	unsigned int VertexCount;
	unsigned int IndexCount;
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;



};