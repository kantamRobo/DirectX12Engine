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
#include "DeviceResources.h"
#include "Camera.h"
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
public:
	Terrein(){};
	Terrein(ID3D12Device* device, const DirectX::RenderTargetState rtState, const std::shared_ptr<DX::DeviceResources> devicesresources);
	grayscale m_heightmap;
	std::vector<DirectX::VertexPosition> m_vertices;
	//ハイトマップを作成、またはロード、どちらか
	//を行う
	void Preparegrayscale();
	//ノーマルマップを作成、またはロード、どちらかを
	//行う
	//GUI側からAddlineに使ったImvec2インスタンス配列の全ての要素を使い、grayscaleの要素に代入する
	void SetGrayScale();
	void PrepareNormalMap(const grayscale* heightMap, Normalmap* normalMap, unsigned int width,unsigned int height)
		;
public:
	void Preparepatch(ID3D12Device* device, DirectX::RenderTargetState targetstate,
		const std::shared_ptr<DX::DeviceResources> devicesresources);
	void DrawTerrein(ID3D12GraphicsCommandList* command, const Camera in_camera);
	std::vector<UINT> indices;
	std::unique_ptr<TerreinEditor> m_terreineditor;
	Microsoft::WRL::ComPtr<ID3D12Resource>  m_patchvertexbuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>  m_patchindexbuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_normaltexture;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_heighttexture;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_patchrootsignature = nullptr;
	TesselationEffectPipelineDescription terreinpipeline;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_patchpipelinestate=nullptr;
	DirectX::SimpleMath::Matrix world;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::DescriptorHeap> m_heightmapheap = nullptr;
	std::unique_ptr<DirectX::DescriptorHeap> m_normalmapheap = nullptr;
	
};

