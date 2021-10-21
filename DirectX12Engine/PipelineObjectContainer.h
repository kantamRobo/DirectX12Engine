#pragma once
#include <wrl.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
//レンダリングパイプラインに必要な、シェーダー、ルートシグネチャとパイプラインステートオブジェクトを格納する

struct GraphicPipeLineObjectContainer
{

	Microsoft::WRL::ComPtr<ID3DBlob>  m_vs, m_ps;
	//ハル、ジオメトリ等も入れておく。
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipeline;
};