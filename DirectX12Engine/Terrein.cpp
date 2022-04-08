#include "Terrein.h"
#include <cmath>

using namespace std;

Terrein::Terrein()
{
	m_terreineditor = std::make_unique<TerreinEditor>();
}
void Terrein::Preparegrayscale()
{
	//
	if ("terreinheightmap.png" != nullptr)
	{
		heightとwidthをロード


	}
	else
	{
		heightmap.m_grayscale.resize(heightmap.width);
		for (int i = 0; i < heightmap.width; i++) {
			heightmap.m_grayscale[i].resize(heightmap.height);
		
		}
		const int Defaultgrayscalevalue = 0;
		for (int currentwidth = 0; currentwidth < heightmap.width
			; currentwidth++)
		{
			for (int currentheight = 0; currentheight < heightmap.height; heightmap.height)
			{
				heightmap.m_grayscale[currentwidth][currentheight] = 0;
			}

		}
	}
	



}

void Terrein::SetGrayScale()
{
	int x = 0;
	int y = 0;
	x = x +m_terreineditor->Nodes[0].x;
	y = y + m_terreineditor->Nodes[0].y;
	m_terreineditor->Nodes.reserve(m_terreineditor->nodenum);
	
	for (x < m_terreineditor->nodenum; x += m_terreineditor->offset;)
	{
			
			m_terreineditor->Nodes[x].x = x;
			m_terreineditor->Nodes[x].y = -m_terreineditor->slope * (x * x);

		
	}
	ImU32 whiteColor = IM_COL32(255, 255, 255, 255);
for (int i = 0; i < m_terreineditor->nodenum; i += m_terreineditor->offset)
	{
	
	if (i + 1 != m_terreineditor->nodenum)
		m_terreineditor->drawListR->AddLine(m_terreineditor->Nodes[i], m_terreineditor->Nodes[i + 1], whiteColor);
	else
	{
		break;
	}
}

}


void Terrein::PrepareNormalMap(const grayscale* heightMap, Normalmap* normalMap, unsigned int width, unsigned int height)
{
	for (unsigned int y = 0; y < height; y++)
	{
		unsigned int ym1 = (y - 1) & (height - 1);
		unsigned int yp1 = (y + 1) & (height - 1);

		std::vector<float> centerRow = {};
		std::vector<float> upperRow = {};
		std::vector<float> lowerRow = {};
		centerRow.resize(width);
		upperRow.resize(width);
		lowerRow.resize(width);
           
		for (unsigned int x = 0; x < width; x++)
		{
			
			 centerRow[x]= heightMap->m_grayscale[x][y] + (y * width);
			upperRow[x] = heightMap->m_grayscale[x][y] + (ym1 * width);
			lowerRow[x]= heightMap->m_grayscale[x][y] + (yp1 * width);
			unsigned int xm1 = (x - 1) & (width - 1);
			unsigned int xp1 = (x + 1) & (width - 1);


			//Calculate slopes
			float dx = (centerRow[xp1] - centerRow[xm1] * 0.5f);
			float dy = (lowerRow[x] - upperRow[x]) * 0.5f;
		
		//Normalie and clamp.
			float nz = 1.0f / sqrt(dx * dx + dy * dy + 1.0f);
			float nx = fmin(fmax(-dx * nz, -1.0f), 1.0f);
			float ny = fmin(fmax(-dy * nz, -1.0f), 1.0f);
			//normalMap[x].Set(nx,ny,nz)
			//
		}
		normalMap->normalmap += width;
		//各要素に右辺値を加算するoperator +=を追加する
	}
}

void Terrein::Preparepatch()
{
	const int divide = 10;
	const float edge = 200.0f;
	for (int z = 0; z < divide + 1; ++z)
	{
		for (int x = 0; x < divide + 1; ++x)
		{
			DirectX::VertexPositionNormalTexture v;
			v.position = DirectX::SimpleMath::Vector3(
				edge * x / divide,
				0.0f,
				edge * z / divide
			);
			v.textureCoordinate = DirectX::SimpleMath::Vector2(
				v.position.x / edge,
				v.position.z / edge
			);
			vertices.push_back(v);
		}
	}
	std::vector<UINT> indices;
	for (int z = 0; z < divide; ++z) {
		for (int x = 0; x < divide; ++x) {
			const UINT rows = divide + 1;
			UINT v0 = x, v1 = x + 1;

			v0 = v0 + rows * z;
			v1 = v1 + rows * z;
			indices.push_back(v0 + rows);
			indices.push_back(v1 + rows);
			indices.push_back(v0);
			indices.push_back(v1);
		}
	}
	// 中心補正
	for (auto& v : vertices)
	{
		v.position.x -= edge * 0.5f;
		v.position.z -= edge * 0.5f;
	}
}