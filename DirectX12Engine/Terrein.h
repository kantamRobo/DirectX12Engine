#pragma once
#include <vector>
#include "SimpleMath.h"
#include <memory>
#include "VertexTypes.h"
#include "ImguiCore.h"
#include "TerreinEditor.h"
#include "stb_image_write.h"
#include "stb_image.h"
#include <GraphicsMemory.h>
#include "TesselationEffectPipelineDescription.h"
#include "CommonStates.h"
#include <wrl.h>
struct grayscale
{
	unsigned int width=0;
	unsigned int height=0;
	std::vector<std::vector<unsigned int>> m_grayscale;
	unsigned int s_grayscale[12][12];
};

struct Normalmap
{
	unsigned int m_width;
	unsigned int m_height;
	std::shared_ptr< DirectX::SimpleMath::Vector3> m_normalmap;
};

class Terrein
{
	Terrein();
	grayscale m_heightmap;
	std::vector<DirectX::VertexPositionNormalTexture> m_vertices;
	//ハイトマップを作成、またはロード、どちらか
	//を行う
	void Preparegrayscale();
	//ノーマルマップを作成、またはロード、どちらかを
	//行う
	//GUI側からAddlineに使ったImvec2インスタンス配列の全ての要素を使い、grayscaleの要素に代入する
	void SetGrayScale();

	void PrepareNormalMap(const grayscale* heightMap, Normalmap* normalMap, unsigned int width,unsigned int height)
		;
	void Preparepatch(ID3D12Device* device, DirectX::RenderTargetState targetstate);

	void DrawTerrein(ID3D12GraphicsCommandList4* );
	std::vector<UINT> indices;
	std::unique_ptr<TerreinEditor> m_terreineditor;
	DirectX::GraphicsResource  m_patchvertexbuffer;
	DirectX::GraphicsResource  m_patchindexbuffer;
	std::unique_ptr<DirectX::GraphicsMemory> m_terreingraphicsmemory;
	TesselationEffectPipelineDescription terreinpipeline;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_patchpipelinestate=nullptr;


	
};

