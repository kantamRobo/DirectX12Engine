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
#include "CustomIncludeHandler.h"
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
	Terrein(ID3D12RootSignature* p_rootsignature) { m_patchrootsignature = p_rootsignature; };
	grayscale m_heightmap;
	std::vector<DirectX::VertexPositionTexture> m_vertices;
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
	std::unique_ptr<CustomIncludeHandler> m_handler;
	void Preparepatch(ID3D12Device* device, DirectX::RenderTargetState targetstate,
		const std::shared_ptr<DX::DeviceResources> devicesresources,
		std::shared_ptr<DirectX::GraphicsMemory> graphicsMemory);
	void DrawTerrein(ID3D12GraphicsCommandList* command, const Camera in_camera,
		std::shared_ptr<DirectX::BasicEffect> in_effect);
	
	DirectX::SharedGraphicsResource vertexBuffer;
	DirectX::SharedGraphicsResource indexBuffer;
	std::vector<UINT> m_indices;
	std::unique_ptr<TerreinEditor> m_terreineditor;
	DirectX::SharedGraphicsResource  m_patchvertexbuffer;
	DirectX::SharedGraphicsResource m_patchindexbuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_staticpatchvertexbuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_staticpatchindexbuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_normaltexture;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_heighttexture;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_patchrootsignature = nullptr;
	TesselationEffectPipelineDescription terreinpipeline;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_patchpipelinestate=nullptr;
	DirectX::SimpleMath::Matrix world;
	
	std::unique_ptr<DirectX::DescriptorHeap> m_heightmapheap = nullptr;
	std::unique_ptr<DirectX::DescriptorHeap> m_normalmapheap = nullptr;
	
};

