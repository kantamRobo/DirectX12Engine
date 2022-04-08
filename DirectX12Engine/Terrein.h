#pragma once
#include <vector>
#include "SimpleMath.h"
#include <memory>
#include "VertexTypes.h"
#include "ImguiCore.h"
#include "TerreinEditor.h"
struct grayscale
{
	unsigned int width=0;
	unsigned int height=0;
	std::vector<std::vector<unsigned int>> m_grayscale;
	unsigned int s_grayscale[12][12];
};

struct Normalmap
{
	unsigned int width;
	unsigned int height;
	std::shared_ptr< DirectX::SimpleMath::Vector3> normalmap;
};

class Terrein
{
	Terrein();
	grayscale heightmap;
	std::vector<DirectX::VertexPositionNormalTexture> vertices;
	//ハイトマップを作成、またはロード、どちらか
	//を行う
	void Preparegrayscale();
	//ノーマルマップを作成、またはロード、どちらかを
	//行う
	//GUI側からAddlineに使ったImvec2インスタンス配列の全ての要素を使い、grayscaleの要素に代入する
	void SetGrayScale();

	void PrepareNormalMap(const grayscale* heightMap, Normalmap* normalMap, unsigned int width,unsigned int height)
		;
	void Preparepatch();

	std::unique_ptr<TerreinEditor> m_terreineditor;
};

